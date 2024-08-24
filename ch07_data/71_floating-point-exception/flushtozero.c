#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__SSE2__)
#include <immintrin.h>
#elif defined(__aarch64__)
#if defined(__clang__)
#include <arm_acle.h>
#define get_fpcr() __arm_rsr64("fpcr")
#define set_fpcr(x) __arm_wsr64("fpcr", x)
#elif __GNUC__ >= 11
#define get_fpcr() __builtin_aarch64_get_fpcr64()
#define set_fpcr(x) __builtin_aarch64_set_fpcr64(x)
#else
#error unsupported compiler
#endif
#endif

#pragma STDC FENV_ACCESS ON

int main(int argc, char *argv[]) {
  volatile double a = 0x1p-1022;
  volatile double b = 0x0.deadbeefp0;
  volatile double c = 0x0.deadbeefp-1022;
  if (argc > 1 && strcmp(argv[1], "FZ") == 0) {
#if defined(__SSE2__)
    unsigned int csr = _mm_getcsr();
    _mm_setcsr(csr | (1u << 15)); // Set FTZ (Flush to Zero)
#elif defined(__aarch64__)
    // Set FZ (Flushing denormalized numbers to zero)
    uint64_t fpcr = get_fpcr();
    set_fpcr(fpcr | (1u << 24));
#endif
  }
  feclearexcept(FE_UNDERFLOW);
  volatile double x = a * b;
  printf("FE_UNDERFLOW is %sset.\n",
         fetestexcept(FE_UNDERFLOW) ? "" : "not ");
  printf("%a\n", x);
  feclearexcept(FE_UNDERFLOW);
  volatile double y = a - c;
  printf("FE_UNDERFLOW is %sset.\n",
         fetestexcept(FE_UNDERFLOW) ? "" : "not ");
  printf("%a\n", y);
}
