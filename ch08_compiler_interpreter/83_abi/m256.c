#include <immintrin.h>
#include <stdarg.h>
#include <stdio.h>

void f(int dummy, ...) {
  va_list va;
  va_start(va, dummy);
  __m256d x = va_arg(va, __m256d);
  double a[4];
  _mm256_storeu_pd(a, x);
  printf("%g, %g, %g, %g\n", a[0], a[1], a[2], a[3]);
  va_end(va);
}

int main(void) {
  __m256d x = _mm256_set_pd(1.1, 2.2, 3.3, 4.4);
  __m256d y = _mm256_set_pd(111.0, 222.0, 333.0, 444.0);
  f(0, x);
  void (*g)() = (void (*)())f;
  g(0, y);
}
