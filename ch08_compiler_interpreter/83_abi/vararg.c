#include <stdarg.h>
#include <stdio.h>

void foo(int n, ...) {
  va_list va;
  va_start(va, n);
  int a = va_arg(va, int);
  int b = va_arg(va, int);
  printf("a*b=%d\n", a * b);
  int c = va_arg(va, int);
  printf("c=%d\n", c);
  va_end(va);
}

int main(void) {
  printf("sizeof(va_list)=%zu\n", sizeof(va_list));
  foo(42, 2, 7, 5);
  foo(0, 11, 22, 33);
}
