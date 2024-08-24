#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h> // pthread_jit_write_protect_np
#include <libkern/OSCacheControl.h> // sys_icache_invalidate

typedef int (*FUNC)(int);
FUNC bind1st(int (*f)(void *, int), void *context) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  // MAP_JITを指定することでRWXを同時に指定できる
  void *mem = mmap(NULL, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_ANONYMOUS | MAP_PRIVATE | MAP_JIT, -1, 0);
  if (mem == MAP_FAILED) {
    perror("mmap");
    abort();
  }
  pthread_jit_write_protect_np(0); // MAP_JIT領域は書き込み可能、実行不能になる
  uint64_t *Lcontext = (uint64_t *)((char *)mem + 16);
  uint64_t *Lfunc = (uint64_t *)((char *)mem + 24);
  uint32_t *instr = mem;  // Assume little-endian
  /* mov w1, w0 */
  *instr++ = 0x2a0003e0 | (0 << 16) | 1;
  /* ldr x0, Lcontext */
  *instr = 0x58000000 |
           (((((char *)Lcontext - (char *)instr) >> 2) & 0x7ffff) << 5) | 0;
  ++instr;
  /* ldr x17, Lfunc */
  *instr = 0x58000000 |
           (((((char *)Lfunc - (char *)instr) >> 2) & 0x7ffff) << 5) | 17;
  ++instr;
  /* br x17 */
  *instr++ = 0xd61f0000 | (17 << 5);
  /* Lcontext: .qword <context> */
  *Lcontext = (uint64_t)context;
  /* Lfunc: .qword <f> */
  *Lfunc = (uint64_t)f;
  sys_icache_invalidate(mem, (char *)instr - (char *)mem); // 命令キャッシュをクリアするmacOS用の関数
  pthread_jit_write_protect_np(1); // MAP_JIT領域は書き込み不能、実行可能になる
  return (FUNC)mem;
}

int foo(void *context, int b) {
  int a = *(int *)context;
  printf("foo: a=%d, b=%d\n", a, b);
  return a + b;
}

int main(void) {
  int context_f = 2;
  int context_g = 3;
  FUNC f = bind1st(foo, &context_f);
  FUNC g = bind1st(foo, &context_g);
  printf("f(5) = %d\n", f(5));
  printf("g(7) = %d\n", g(7));
}
