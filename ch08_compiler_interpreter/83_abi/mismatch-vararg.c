#include <stdarg.h>
#include <stdio.h>

void foo(int n, ...) {
  va_list va;
  va_start(va, n);
  for (int i = 0; i < n; ++i) {
    int x = va_arg(va, int);
    printf("%d%s", x, i == n - 1 ? "\n" : ", ");
  }
  va_end(va);
}

int main(void) {
  foo(3, 11, 22, 33);
  void (*f)() = (void (*)())foo;
  f(4, 111, 222, 333, 4444);
}
