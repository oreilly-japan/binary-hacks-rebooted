#include <dlfcn.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern uint64_t thunk(void (*func)(), uint64_t regargs[6],
                      size_t nbytes, void *stackargs);

int foo(int a0, const char *a1, unsigned long a2, long long a3,
        unsigned char a4, int a5, int a6, int a7, int a8) {
  printf("foo: %d, %s, %lu, %lld, %d, %d, %d, %d, %d\n", a0, a1, a2,
         a3, a4, a5, a6, a7, a8);
  return 42;
}

int main(void) {
  uint64_t args[6] = {(uint64_t)42, (uint64_t) "Hello!",
                      (uint64_t)33, (uint64_t)(-44LL),
                      (uint64_t)55, (uint64_t)-66};
  char stackargs[32];
  int a6 = 77, a7 = -88, a8 = 99;
  memcpy(stackargs, &a6, 4);
  memcpy(stackargs + 8, &a7, 4);
  memcpy(stackargs + 16, &a8, 4);
  thunk((void (*)())foo, args, 32, stackargs);
  return 0;
}
