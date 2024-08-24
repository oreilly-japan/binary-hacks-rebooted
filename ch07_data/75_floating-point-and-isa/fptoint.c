#include <fenv.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

void test_cast(double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  int32_t i = (int32_t)x;
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("(int32_t)%g = %" PRId32 " (0x%08" PRIx32
         "), %s FE_INVALID, %s FE_INEXACT\n",
         x, i, (uint32_t)i, invalid ? "with" : "without",
         inexact ? "with" : "without");
}

void test_lrint(double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  long i = lrint(x);
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("lrint(%g) = %ld (0x%08lx), %s FE_INVALID, %s FE_INEXACT\n", x, i,
         (unsigned long)i, invalid ? "with" : "without",
         inexact ? "with" : "without");
}

int main(void) {
  test_cast(-3.14);
  test_cast(-3.99);
  test_cast(1e300);
  test_cast(INFINITY);
  test_cast(-INFINITY);
  test_cast(NAN);
  test_lrint(-3.14);
  test_lrint(-3.99);
  test_lrint(1e300);
  test_lrint(INFINITY);
  test_lrint(-INFINITY);
  test_lrint(NAN);
}
