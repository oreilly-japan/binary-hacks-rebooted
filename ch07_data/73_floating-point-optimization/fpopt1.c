#include <fenv.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

struct result {
  double up, down;
};
struct result add(double x, double y) {
  fesetround(FE_UPWARD);
  double up = x + y;
  fesetround(FE_DOWNWARD);
  double down = x + y;
  fesetround(FE_TONEAREST);
  return (struct result){.up = up, .down = down};
}

int main(void) {
  struct result r = add(1.0, 0x1p-53);
  printf("up: %a\n", r.up);
  printf("down: %a\n", r.down);
}
