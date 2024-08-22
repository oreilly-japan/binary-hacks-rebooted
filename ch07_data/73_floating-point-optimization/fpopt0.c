#include <fenv.h>
#include <stdio.h>

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
  printf("up: %a\n", r.up);      // 0x1.0000000000001p+0 になってほしい
  printf("down: %a\n", r.down);  // 0x1p+0 になってほしい
}
