#include <fenv.h>
#include <float.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

int main(void) {
  feclearexcept(FE_INVALID);
  float x = FLT_SNAN;
  int e = fetestexcept(FE_INVALID);
  if (e & FE_INVALID)
    puts("FE_INVALID is set");
  else
    puts("FE_INVALID is not set");
  printf("%g\n", x);
  feclearexcept(FE_INVALID);
  float y = x * 1.5f;
  e = fetestexcept(FE_INVALID);
  if (e & FE_INVALID)
    puts("FE_INVALID is set");
  else
    puts("FE_INVALID is not set");
  printf("%g\n", y);
}
