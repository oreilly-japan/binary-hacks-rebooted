#include <assert.h>
#include <fenv.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

double add_up(double x, double y) {
  int r = fesetround(FE_UPWARD);
  assert(r == 0);  // 変更に成功した場合は 0 が返る
  double z = x + y;
  fesetround(FE_TONEAREST);
  return z;
}

int main(void) {
  double z = add_up(1.0, 0x1p-1022);
  printf("%a\n", z);
}
