#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <libkern/OSCacheControl.h>
#endif

typedef int (*FUNC)(int);
FUNC bind1st(int (*f)(void *, int), void *context) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void *mem = mmap(NULL, pagesize, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (mem == MAP_FAILED) {
    perror("mmap");
    abort();
  }
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
  if (mprotect(mem, pagesize, PROT_READ | PROT_EXEC) != 0) {
    perror("mprotect");
    abort();
  }
#if defined(__APPLE__)
  sys_icache_invalidate(mem, (char *)instr - (char *)mem + 16);
#else
  __builtin___clear_cache(mem, (void *)((char *)instr + 16));
#endif
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
