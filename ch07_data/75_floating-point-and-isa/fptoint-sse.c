// fptoint-sse.c
#include <fenv.h>
#include <immintrin.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

void test_cvtss2si(float x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  int32_t i = _mm_cvtss_i32(_mm_set_ss(x));
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("_mm_cvtss_i32(%g) = %" PRId32 " (0x%08" PRIx32
         "), %s FE_INVALID, %s FE_INEXACT\n",
         x, i, (uint32_t)i, invalid ? "with" : "without",
         inexact ? "with" : "without");
}

void test_cvtsd2si(double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  int32_t i = _mm_cvtsd_i32(_mm_set_sd(x));
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("_mm_cvtsd_i32(%g) = %" PRId32 " (0x%08" PRIx32
         "), %s FE_INVALID, %s FE_INEXACT\n",
         x, i, (uint32_t)i, invalid ? "with" : "without",
         inexact ? "with" : "without");
}

#if defined(__AVX512F__)
void test_vcvtsd2usi(double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  uint32_t i = _mm_cvtsd_u32(_mm_set_sd(x));
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("_mm_cvtsd_u32(%g) = %" PRIu32 " (0x%08" PRIx32
         "), %s FE_INVALID, %s FE_INEXACT\n",
         x, i, i, invalid ? "with" : "without", inexact ? "with" : "without");
}
#endif

int main(void) {
  test_cvtss2si(-3.14f);
  test_cvtss2si(-3.99f);
  test_cvtss2si(1e20f);
  test_cvtss2si(INFINITY);
  test_cvtss2si(-INFINITY);
  test_cvtss2si(NAN);
  test_cvtsd2si(-3.14);
  test_cvtsd2si(-3.99);
  test_cvtsd2si(1e300);
  test_cvtsd2si(INFINITY);
  test_cvtsd2si(-INFINITY);
  test_cvtsd2si(NAN);
#if defined(__AVX512F__)
  test_vcvtsd2usi(-3.14);
  test_vcvtsd2usi(-3.99);
  test_vcvtsd2usi(1e300);
  test_vcvtsd2usi(INFINITY);
  test_vcvtsd2usi(-INFINITY);
  test_vcvtsd2usi(NAN);
#endif
}
