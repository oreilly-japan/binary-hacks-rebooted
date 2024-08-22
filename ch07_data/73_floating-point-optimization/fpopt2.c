#include <fenv.h>
#include <stdio.h>

struct result {
  double up, down;
};
struct result add(volatile double x, volatile double y) {
  fesetround(FE_UPWARD);
  volatile double up = x + y;
  fesetround(FE_DOWNWARD);
  volatile double down = x + y;
  fesetround(FE_TONEAREST);
  return (struct result){.up = up, .down = down};
}

int main(void) {
  struct result r = add(1.0, 0x1p-53);
  printf("up: %a\n", r.up);
  printf("down: %a\n", r.down);
}
