#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

typedef int (*FUNC)(int);
FUNC bind1st(int (*f)(void *, int), void *context) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void *mem = mmap(NULL, pagesize, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (mem == MAP_FAILED) {
    perror("mmap");
    abort();
  }
  unsigned char *instr = mem;
  /* mov esi, edi */
  *instr++ = 0x89;
  *instr++ = 0xc0 | (7 << 3) | 6;
  /* mov rdi, <context> */
  *instr++ = 0x48;
  *instr++ = 0xb8 | 7;
  memcpy(instr, &context, 8);
  instr += 8;
  /* mov rax, <f> */
  *instr++ = 0x48;
  *instr++ = 0xb8 | 0;
  memcpy(instr, &f, 8);
  instr += 8;
  /* jmp rax */
  *instr++ = 0xff;
  *instr++ = 0xc0 | (4 << 3) | 0;
  __builtin___clear_cache(mem, (void *)instr);
  if (mprotect(mem, pagesize, PROT_READ | PROT_EXEC) != 0) {
    perror("mprotect");
    abort();
  }
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
