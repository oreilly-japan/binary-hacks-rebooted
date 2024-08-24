#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/*
 * x64 Windows の呼び出し規約は
 *   x64 calling convention | Microsoft Learn
 *   https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention
 * を参照
 */

typedef int (*FUNC)(int);
FUNC bind1st(int (*f)(void *, int), void *context) {
  size_t pagesize;
  {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    pagesize = si.dwPageSize;
  }
  void *mem = VirtualAlloc(NULL, pagesize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (mem == NULL) {
    fprintf(stderr, "VirtualAlloc: %u\n", (unsigned int)GetLastError());
    abort();
  }
  unsigned char *instr = mem;
  /* mov edx, ecx */
  *instr++ = 0x89;
  *instr++ = 0xc0 | (1 << 3) | 2;
  /* mov rcx, <context> */
  *instr++ = 0x48;
  *instr++ = 0xb8 | 1;
  memcpy(instr, &context, 8);
  instr += 8;
  /* mov rax, <func> */
  *instr++ = 0x48;
  *instr++ = 0xb8 | 0;
  memcpy(instr, &f, 8);
  instr += 8;
  /* jmp rax */
  *instr++ = 0xff;
  *instr++ = 0xc0 | (4 << 3) | 0;
  FlushInstructionCache(GetCurrentProcess(), mem, instr - (unsigned char *)mem);
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
