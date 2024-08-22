use core::time::Duration;
use std::path::PathBuf;

use libafl::{
    corpus::{Corpus, InMemoryCorpus, OnDiskCorpus},
    events::SimpleEventManager,
    executors::CommandExecutor,
    feedback_and_fast,
    feedbacks::{CrashFeedback, MaxMapFeedback},
    fuzzer::{Fuzzer, StdFuzzer},
    inputs::BytesInput,
    monitors::SimplePrintingMonitor,
    mutators::{scheduled::havoc_mutations, tokens_mutations, StdScheduledMutator, Tokens},
    observers::{HitcountsMapObserver, StdMapObserver},
    schedulers::QueueScheduler,
    stages::mutational::StdMutationalStage,
    state::{HasCorpus, HasMetadata, StdState},
};
use libafl_bolts::{
    current_nanos,
    rands::StdRand,
    shmem::{ShMem, ShMemProvider, UnixShMemProvider},
    tuples::{tuple_list, Merge},
    AsMutSlice
};

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

    let mut feedback = MaxMapFeedback::tracking(&edges_observer, false, true);

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
    // 単純に順番に選ぶことにする
    let scheduler = QueueScheduler::new();
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
