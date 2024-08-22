#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

void test_mprotect(void) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void *mem = mmap(NULL, pagesize, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  clock_t start = clock();
  for (int i = 0; i < 100000; ++i) {
    if (mprotect(mem, pagesize, PROT_READ | PROT_EXEC) != 0) {
      perror("mprotect");
      abort();
    }
    if (mprotect(mem, pagesize, PROT_READ | PROT_WRITE) != 0) {
      perror("mprotect");
      abort();
    }
    *(volatile int *)mem = i;
  }
  clock_t end = clock();
  printf("mprotect: %.3f ms\n",
         (double)(end - start) / (double)CLOCKS_PER_SEC * 1e3);
  munmap(mem, pagesize);
}

void test_jit(void) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void *mem = mmap(NULL, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_ANONYMOUS | MAP_PRIVATE | MAP_JIT, -1, 0);
  clock_t start = clock();
  for (int i = 0; i < 100000; ++i) {
    pthread_jit_write_protect_np(1);
    pthread_jit_write_protect_np(0);
    *(volatile int *)mem = i;
  }
  clock_t end = clock();
  printf("pthread_jit_write_protect_np: %.3f ms\n",
         (double)(end - start) / (double)CLOCKS_PER_SEC * 1e3);
  munmap(mem, pagesize);
}

int main(void) {
  printf("pthread_jit_write_protect_supported_np() = %d\n",
         pthread_jit_write_protect_supported_np());
  test_mprotect();
  test_jit();
}
