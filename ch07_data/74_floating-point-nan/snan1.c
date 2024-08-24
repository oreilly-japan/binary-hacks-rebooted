#include <fenv.h>
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// signaling NaN を構築する関数
void make_signaling_nan(float *a) {
  uint32_t pattern = UINT32_C(0x7f800001);  // IEEE 754-2008 準拠を仮定する
  memcpy(a, &pattern, 4);
}

#pragma STDC FENV_ACCESS ON

int main(void) {
  feclearexcept(FE_INVALID);
  float x;
  make_signaling_nan(&x);  // x には sNaN が代入される
  int e = fetestexcept(FE_INVALID);
  if (e & FE_INVALID)
    puts("FE_INVALID is set");
  else
    puts("FE_INVALID is not set");
  printf("%g\n", x);
  feclearexcept(FE_INVALID);
  float y = x * 1.5f;  // sNaN に対して演算を行う
  e = fetestexcept(FE_INVALID);
  if (e & FE_INVALID)
    puts("FE_INVALID is set");
  else
    puts("FE_INVALID is not set");
  printf("%g\n", y);
}
