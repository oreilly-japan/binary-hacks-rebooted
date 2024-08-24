use core::time::Duration;
use std::path::PathBuf;
use serde::{Deserialize, Serialize};
use core::marker::PhantomData;

use libafl::{
    Error,
    corpus::{Corpus, InMemoryCorpus, OnDiskCorpus},
    events::{SimpleEventManager, EventFirer},
    executors::{CommandExecutor, ExitKind},
    feedback_and_fast,
    feedbacks::{CrashFeedback, MaxMapFeedback},
    fuzzer::{Fuzzer, StdFuzzer},
    inputs::BytesInput,
    monitors::SimplePrintingMonitor,
    mutators::{scheduled::havoc_mutations, tokens_mutations, StdScheduledMutator, Tokens},
    observers::{HitcountsMapObserver, StdMapObserver, ObserversTuple},
    schedulers::{WeightedScheduler, ProbabilitySamplingScheduler},
    stages::mutational::StdMutationalStage,
    state::{HasCorpus, HasMetadata, StdState, State},
    corpus::Testcase,
    schedulers::{TestcaseScore},
    feedbacks::Feedback,
};
use libafl_bolts::{
    impl_serdeany,
    current_nanos,
    rands::StdRand,
    shmem::{ShMem, ShMemProvider, UnixShMemProvider},
    tuples::{tuple_list, Merge},
    AsMutSlice,
    Named,
    HasLen,
};

#[derive(Serialize, Deserialize, Clone, Debug)]
struct HeuristicMetadata {
    num_selected: u64,
    num_succeeded: u64
}
impl_serdeany!(HeuristicMetadata);

impl HeuristicMetadata {
    pub fn new() -> Self {
        Self {
          num_selected: 2,
          num_succeeded: 1,
        }
    }

    pub fn add_fail(&mut self) {
        self.num_selected += 1;
    }

    pub fn add_succ(&mut self) {
        self.num_selected += 1;
        self.num_succeeded += 1;
    }

    pub fn get_succ_rate(&self) -> f64 {
      return (self.num_succeeded as f64) / (self.num_selected as f64);
    }
}

#[derive(Debug, Clone)]
struct HeuristicTestcaseScore<S> {
    phantom: PhantomData<S>,
}

impl<S> TestcaseScore<S> for HeuristicTestcaseScore<S>
where
    S: HasCorpus + HasMetadata,
    S::Input: HasLen,
{
    fn compute(state: &S, entry: &mut Testcase<S::Input>) -> Result<f64, Error> {
        let exec_time = entry.exec_time().map_or(1, |v| v.as_millis()) as u32;
        let len = entry.cached_len().unwrap_or(1) as u32;
        let hmeta = entry.metadata::<HeuristicMetadata>()?;
        return Ok(hmeta.get_succ_rate() / (len * exec_time) as f64);
    }
}

type HeuristicScheduler<O, S> = WeightedScheduler<HeuristicTestcaseScore<S>, O, S>;

#[derive(Debug)]
struct WrapperFeedback<S, F> {
    base_feedback: F,
    phantom: PhantomData<S>
}

impl<S, F> WrapperFeedback<S, F>
  where S: State + HasCorpus,
        F: Feedback<S>,
{
    pub fn new(feedback : F) -> Self {
        Self {
          base_feedback: feedback,
          phantom: PhantomData,
        }
    }
}

impl<S, F> Named for WrapperFeedback<S, F>
  where S: State + HasCorpus,
        F: Feedback<S>,
{
    fn name(&self) -> &str {
        self.base_feedback.name()
    }
}

impl<S, F> Feedback<S> for WrapperFeedback<S, F>
  where S: State + HasCorpus,
        F: Feedback<S>,
{
    fn init_state(&mut self, state: &mut S) -> Result<(), Error> {
        self.base_feedback.init_state(state)
    }

    fn is_interesting<EM, OT>(
        &mut self,
        state: &mut S,
        manager: &mut EM,
        input: &S::Input,
        observers: &OT,
        exit_kind: &ExitKind,
    ) -> Result<bool, Error>
    where
        EM: EventFirer<State = S>,
        OT: ObserversTuple<S>,
    {
        let result = self.base_feedback.is_interesting(
                        state, manager, input, observers, exit_kind);

        let corpus = state.corpus();
        if corpus.current().is_none() {
          return result;
        }

        let cur_seed_idx = corpus.current().unwrap();
        let mut cur_seed = corpus.get(cur_seed_idx).unwrap().borrow_mut();
        let hmeta = cur_seed.metadata_mut::<HeuristicMetadata>().unwrap();

        match result {
          Ok(true) => { hmeta.add_succ(); }
                 _ => { hmeta.add_fail(); }
        }

        return result;
    }

    fn append_metadata<OT>(
        &mut self,
        state: &mut S,
        observers: &OT,
        testcase: &mut Testcase<S::Input>,
        ) -> Result<(), Error>
    where
        OT: ObserversTuple<S>,
    {
        self.base_feedback.append_metadata(state, observers, testcase)?;

        let hmeta = HeuristicMetadata::new();
        testcase.add_metadata(hmeta);
        Ok(())
    }

    fn discard_metadata(&mut self, state: &mut S, input: &S::Input)
        -> Result<(), Error>
    {
        self.base_feedback.discard_metadata(state, input)
    }
}

pub fn main() {
    // カバレッジを取得するための共有メモリの用意
    const MAP_SIZE: usize = 65536;
    let mut shmem_provider = UnixShMemProvider::new().unwrap();
    let mut shmem = shmem_provider.new_shmem(MAP_SIZE).unwrap();
    let shmem_id = shmem.id();
    let shmem_buf = shmem.as_mut_slice();
    let edges_observer =
        unsafe {
          HitcountsMapObserver::new(StdMapObserver::new("shared_mem", shmem_buf))
        };

    let mut feedback = WrapperFeedback::new(
                          MaxMapFeedback::tracking(&edges_observer, false, true));

    // シードのディスクへの保存条件。クラッシュだけを条件にすると
    // 似たような入力が大量に保存されうるのでカバレッジにも条件を付ける
    let mut objective = feedback_and_fast!(
        CrashFeedback::new(),
        MaxMapFeedback::with_name("mapfeedback_metadata_objective", &edges_observer)
    );

    let mut state = StdState::new(
        StdRand::with_seed(current_nanos()),
        InMemoryCorpus::<BytesInput>::new(), // シード集合はメモリに保存
        OnDiskCorpus::new(PathBuf::from("./crashes")).unwrap(),
        &mut feedback,
        &mut objective,
    )
    .unwrap();

    // シードを選択する戦略（シードスケジューリング）
    // シードに重みを付け、確率的に選択
    let scheduler = HeuristicScheduler::new(&mut state, &edges_observer);
    let mut fuzzer = StdFuzzer::new(scheduler, feedback, objective);

    let monitor = SimplePrintingMonitor::new();
    let mut mgr = SimpleEventManager::new(monitor);

    // 辞書の読み込み
    let tokens = Tokens::from_file("png.dict").unwrap();

    // 対象プログラムの実行環境
    let mut executor = CommandExecutor::builder()
        .program("./libpng_read_fuzzer")
        .arg_input_file_std() // コマンドライン引数に生成した入力を渡す
        .env("__AFL_SHM_ID", shmem_id.to_string()) // 共有メモリを与える
        .timeout(Duration::from_millis(1000))
        .build(tuple_list!(edges_observer))
        .unwrap();

    // 初期シードの読み込み
    let corpus_dirs: Vec<PathBuf> = vec!["initial_seeds".into()];
    state
        .load_initial_inputs(&mut fuzzer, &mut executor, &mut mgr, &corpus_dirs)
        .unwrap_or_else(|err| {
            panic!(
                "Failed to load initial corpus at {:?}: {:?}",
                &corpus_dirs, err
            )
        });
    println!("We imported {} inputs from disk.", state.corpus().count());

    // ミューテーションでは Token を State のメタデータから取得する
    state.add_metadata(tokens);

    let mutator = StdScheduledMutator::with_max_stack_pow(
                      havoc_mutations().merge(tokens_mutations()), 6);
    let mut stages = tuple_list!(StdMutationalStage::new(mutator));

    fuzzer
        .fuzz_loop(&mut stages, &mut executor, &mut state, &mut mgr)
        .expect("Error in the fuzzing loop");
}
