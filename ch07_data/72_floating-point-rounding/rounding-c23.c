#include <fenv.h>
#include <math.h>
#include <stdio.h>

struct result {
  double nearesteven, nearestaway, up, down, zero;
};
struct result add(double x, double y) {
  struct result result;
  {
#pragma STDC FENV_ROUND FE_TONEAREST
    result.nearesteven = x + y;
  }
  {
#if defined(FE_TONEARESTFROMZERO)
#pragma STDC FENV_ROUND FE_TONEARESTFROMZERO
    result.nearestaway = x + y;
#else
    result.nearestaway = NAN;
#endif
  }
  {
#pragma STDC FENV_ROUND FE_UPWARD
    result.up = x + y;
  }
  {
#pragma STDC FENV_ROUND FE_DOWNWARD
    result.down = x + y;
  }
  {
#pragma STDC FENV_ROUND FE_TOWARDZERO
    result.zero = x + y;
  }
  return result;
}

int main(void) {
  struct result result = add(1.0, 0x1p-53);
  printf("nearesteven: %a\n", result.nearesteven);
  printf("nearestaway: %a\n", result.nearestaway);
  printf("up: %a\n", result.up);
  printf("down: %a\n", result.down);
  printf("zero: %a\n", result.zero);
}
