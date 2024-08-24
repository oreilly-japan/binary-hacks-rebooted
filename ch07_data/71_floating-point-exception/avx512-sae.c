#include <fenv.h>
#include <immintrin.h>
#include <stdio.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON

int main(int argc, char *argv[]) {
  volatile __m128 a = _mm_set_ss(1.0f), b = _mm_set_ss(0x1p-100f), c;
  feclearexcept(FE_INEXACT);
  if (argc > 1 && strcmp(argv[1], "sae") == 0) {
    // vaddss xmm, xmm, xmm, {rn-sae}
    c = _mm_add_round_ss(
        a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
  } else {
    // 従来のSSE命令
    c = _mm_add_ss(a, b);
  }
  int e = fetestexcept(FE_INEXACT);
  if (e & FE_INEXACT)
    puts("FE_INEXACT");
  else
    puts("No FE_INEXACT");
  float result;
  _mm_store_ss(&result, c);
  printf("result: %a\n", result);
}
