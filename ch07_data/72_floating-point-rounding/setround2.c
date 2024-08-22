#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#if defined(__SSE2__)
#include <immintrin.h>
#endif

#pragma STDC FENV_ACCESS ON

#if defined(__aarch64__)
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

double add_up(double x, double y) {
#if defined(__SSE2__)
  unsigned int csr = _mm_getcsr();
  _mm_setcsr((csr & ~(3u << 13)) | (2u << 13));  // Set RC=upward
  double z = x + y;
  _mm_setcsr(csr);  // Restore
  return z;
#elif defined(__aarch64__)
  uint64_t fpcr = get_fpcr();
  // Set RMode=RP (upward)
  set_fpcr((fpcr & ~(3u << 22)) | (1u << 22));
  double z = x + y;
  set_fpcr(fpcr);
  return z;
#else
#error not supported
#endif
}

int main(void) {
  double z = add_up(1.0, 0x1p-1022);
  printf("%a\n", z);
}
