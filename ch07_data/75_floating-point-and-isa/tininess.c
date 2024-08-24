#include <fenv.h>
#include <stdbool.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

int main(void) {
  volatile double x = 0x1.0000001p-1022;
  volatile double y = 0x0.fffffffp0;
  // x * y = 0x0.ffffffffffffffp-1022 --> 0x1.0000000000000p-1022
  feclearexcept(FE_UNDERFLOW);
  volatile double z = x * y;
  bool underflow = fetestexcept(FE_UNDERFLOW) != 0;
  printf(
      "%a * %a = %a, %s\n", x, y, z,
      underflow
          ? "with UNDERFLOW\nTininess is detected before rounding"
          : "without UNDERFLOW\nTininess is detected after rounding");
}
