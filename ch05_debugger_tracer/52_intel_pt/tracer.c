#include <assert.h>
#include <err.h>
#include <intel-pt.h>
#include <linux/perf_event.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

long perf_event_open(struct perf_event_attr *attr, pid_t pid, int cpu,
                     int group_fd, unsigned long flags) {
  return syscall(__NR_perf_event_open, attr, pid, cpu, group_fd,
                 flags);
}

const size_t AUX_SIZE = 16 * 1024 * 1024;
char *PROG_PATH = "./check_password";

int get_perf_fd(pid_t child_pid) {
  // Intel PT をサポートしているなら存在するパス
  FILE *fp =
      fopen("/sys/bus/event_source/devices/intel_pt/type", "r");
  if (!fp) {
    err(1, "fopen");
  }

  char buf[20] = "";
  fgets(buf, sizeof(buf), fp);
  fclose(fp);

  uint32_t intel_pt_type = strtoul(buf, NULL, 10);

  struct perf_event_attr attr;
  memset(&attr, 0, sizeof(attr));
  attr.size = sizeof(attr);
  attr.type = intel_pt_type;

  attr.disabled = 1;
  attr.enable_on_exec = 1; // excev 後にトレース開始
  attr.exclude_kernel = 1; // カーネルの命令は対象外

  return perf_event_open(&attr, child_pid, -1, -1,
                         PERF_FLAG_FD_CLOEXEC);
}

uint8_t *
prepare_packet_buffer(int fd,
                      struct perf_event_mmap_page **header_ref) {
  void *base = mmap(NULL, getpagesize() * 2, PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, 0);
  if (base == MAP_FAILED) {
    err(1, "mmap(base)");
  }

  struct perf_event_mmap_page *header = *header_ref = base;
  header->aux_offset = header->data_offset + header->data_size;
  header->aux_size = AUX_SIZE;
  return mmap(NULL, AUX_SIZE, PROT_READ, MAP_SHARED, fd,
              header->aux_offset);
}

void add_images_for_disas(struct pt_insn_decoder *decoder) {
  struct pt_image *image = pt_image_alloc(NULL);
  // 本来はセクションごとに追加すべきだが、面倒なので .text
  // まで一括で追加
  pt_image_add_file(image, PROG_PATH, 0, 0x2000, NULL,
                    0x555555554000);
  pt_image_add_file(image, "/lib/x86_64-linux-gnu/libc.so.6", 0,
                    0x17b000, NULL, 0x7ffff7dbd000);
  // vdso はメモリダンプして持ってくるのが手っ取り早い
  pt_image_add_file(image, "./vdso.bin", 0, 0x2000, NULL,
                    0x7ffff7fc9000);
  pt_image_add_file(image, "/lib64/ld-linux-x86-64.so.2", 0, 0x26000,
                    NULL, 0x7ffff7fcb000);
  pt_insn_set_image(decoder, image);
}

uint64_t trace_and_count(char *exe_path, pid_t child_pid) {
  int fd = get_perf_fd(child_pid);
  if (fd == -1) {
    err(1, "get_perf_fd");
  }

  struct perf_event_mmap_page *header;
  uint8_t *aux = prepare_packet_buffer(fd, &header);
  if (aux == MAP_FAILED) {
    err(1, "prepare_packet_buffer");
  }

  if (kill(child_pid, SIGCONT) == -1) { // 子プロセスを再開
    err(1, "kill");
  }

  struct pt_config pt_conf;
  pt_config_init(&pt_conf);
  pt_conf.begin = &aux[0];
  pt_conf.end = &aux[AUX_SIZE];
  pt_conf.cpu.vendor = pcv_unknown;

  struct pt_insn_decoder *decoder = pt_insn_alloc_decoder(&pt_conf);
  if (decoder == NULL) {
    err(1, "pt_insn_alloc_decoder");
  }

  add_images_for_disas(decoder);

  uint64_t inst_count = 0;
  bool started = false;
  bool child_alive = true; // 非同期なので、falseでもパケットは来る
  bool packet_incoming = true;
  bool need_sync = true;
  while (packet_incoming) {
    if (child_alive) {
      int res = waitpid(child_pid, NULL, WNOHANG);
      if (res == child_pid) {
        child_alive = false;
      }
    }

    uint64_t head =
        header->aux_head; // パケットバッファの末尾のインデックス
    if (head != 0) started = true;
    if (!started) continue;

    int decoder_status = 0;

    if (need_sync) {
      uint64_t old_sync;
      pt_insn_get_sync_offset(decoder, &old_sync);

      // Packet Stream Boundaryパケットで同期してからでないと使えない
      // が、次のPSBパケットがまだバッファに吐き出されてない可能性がある
      // その場合は unwind してやりなおす
      decoder_status = pt_insn_sync_forward(decoder);
      if (decoder_status < 0) {
        if (child_alive) {
          pt_insn_sync_set(decoder, old_sync);
          continue;
        }
        err(1, "pt_insn_sync_forward");
      }

      if (child_alive) {
        uint64_t cur_off;
        pt_insn_get_offset(decoder, &cur_off);
        if (cur_off > head) {
          pt_insn_sync_set(decoder, old_sync);
          continue;
        }
      }

      need_sync = false;
    }

    while (1) {
      uint64_t cur_off;
      pt_insn_get_offset(decoder, &cur_off);
      // decoder が head を超えて参照するのが怖いので、
      // データが十分なバイト数なければデコードしない
      if (child_alive && cur_off < head && head < cur_off + 0x40)
        break;

      while (decoder_status & pts_event_pending) {
        struct pt_event event;
        decoder_status =
            pt_insn_event(decoder, &event, sizeof(event));
        if (decoder_status < 0) {
          err(1, "pt_insn_event");
        }
      }

      struct pt_insn insn;
      decoder_status = pt_insn_next(decoder, &insn, sizeof(insn));
      if (decoder_status == -pte_eos) { // 命令を読み切った
        if (child_alive) {
          err(1, "ring buffer is full"); // 今回は対応しない
        } else {
          packet_incoming = false;
          break;
        }
      } else if (decoder_status < 0) {
        need_sync = true;
        break;
      } else {
        inst_count++;
      }
    }
  }
  return inst_count;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s pass_to_check\n", argv[0]);
    return 0;
  }

  pid_t pid = fork();
  if (!pid) {
    raise(SIGSTOP); // 準備完了まで子プロセスを止める

    char *ch_argv[] = {PROG_PATH, argv[1], NULL};
    execv(PROG_PATH, ch_argv);
    exit(0);
  } else if (pid == -1) {
    err(1, "fork");
  }

  printf("inst_count: %llu\n", trace_and_count(argv[1], pid));
}
