#include <fenv.h>
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

int main(void) {
  uint64_t fpcr = get_fpcr();
  set_fpcr(fpcr | 0x9f00u);  // 0x9f00 = IDE+IXE+UFE+OFE+DZE+IOE
  fpcr = get_fpcr();
  if ((fpcr & 0x9f00u) == 0x9f00u) {
    puts("Traps are supported:");
  } else if ((fpcr & 0x9f00u) != 0) {
    puts("Traps are partially supported:");
  } else {
    puts("Traps are not supported:");
  }
  printf("  Input Denormal exception (IDE) trap %s.\n",
         (fpcr & (1 << 15)) ? "enabled" : "disabled");
  printf("  Inexact exception (IXE) trap %s.\n",
         (fpcr & (1 << 12)) ? "enabled" : "disabled");
  printf("  Underflow exception (UFE) trap %s.\n",
         (fpcr & (1 << 11)) ? "enabled" : "disabled");
  printf("  Overflow exception (OFE) trap %s.\n",
         (fpcr & (1 << 10)) ? "enabled" : "disabled");
  printf("  Divide by Zero exception (DZE) trap %s.\n",
         (fpcr & (1 << 9)) ? "enabled" : "disabled");
  printf("  Invalid Operation exception (IOE) trap %s.\n",
         (fpcr & (1 << 8)) ? "enabled" : "disabled");
  volatile double zero = 0.0, one = 1.0;
  printf("1.0 / 0.0 = %g\n", one / zero);
}
