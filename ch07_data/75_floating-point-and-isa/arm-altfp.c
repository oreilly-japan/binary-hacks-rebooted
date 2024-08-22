#include <fenv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

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

void underflow_after_rounding(void) {
  volatile double x = 0x1.0000001p-1022;
  volatile double y = 0x0.fffffffp0;
  // x * y = 0x0.ffffffffffffffp-1022 -(rounding)-> 0x1.0000000000000p-1022
  feclearexcept(FE_UNDERFLOW);
  volatile double z = x * y;
  bool underflow = fetestexcept(FE_UNDERFLOW) != 0;
  printf(
      "%a * %a, which yields %a, %s\n", x, y, z,
      underflow
          ? "raises UNDERFLOW; underflow is detected before rounding"
          : "does not raise UNDERFLOW; underflow is detected after rounding");
}

int main(void) {
  underflow_after_rounding();
  uint64_t fpcr = get_fpcr();
  set_fpcr(fpcr | 0x2u);  // AH
  fpcr = get_fpcr();
  if (fpcr & 0x2u) {
    puts("FPCR.AH set.");
  } else {
    puts("FPCR.AH not set.");
  }
  underflow_after_rounding();
}
