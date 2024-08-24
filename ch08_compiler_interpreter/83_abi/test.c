#include <stdio.h>

#if defined(__GNUC__)
__attribute__((noinline))
#endif
int foo(const char *a0, int a1, long a2, short a3, int a4,
        unsigned char a5, int a6, long a7, long long a8,
        signed char a9) {
  printf("%s, %d, %ld, %d, %d, %d, %d, %ld, %lld, %d\n", a0, a1, a2,
         a3, a4, a5, a6, a7, a8, a9);
  return 42;
}

int main(void) {
  foo("Binary Hacks", 11, -22, 33, 44, 55, 66, 77, 88, 99);
}
