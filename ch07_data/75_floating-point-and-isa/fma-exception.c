#include <fenv.h>
#include <math.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

int main(void) {
#if defined(FP_FAST_FMA)
  puts("FP_FAST_FMA is defined");
#else
  puts("FP_FAST_FMA is not defined");
#endif
  feclearexcept(FE_INVALID);
  double r = fma(0, INFINITY, NAN);
  printf("fma(0, INFINITY, NAN) = %g, %s INVALID\n", r,
         fetestexcept(FE_INVALID) ? "raises" : "does not raise");
}
