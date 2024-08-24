#include <immintrin.h>
#include <stdio.h>

struct result {
  double nearesteven, up, down, zero;
};
struct result add(double x, double y) {
  __m128d X = _mm_set_sd(x), Y = _mm_set_sd(y);
  __m128d nearesteven = _mm_add_round_sd(
      X, Y, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
  __m128d up = _mm_add_round_sd(
      X, Y, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
  __m128d down = _mm_add_round_sd(
      X, Y, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
  __m128d zero =
      _mm_add_round_sd(X, Y, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
  struct result result;
  _mm_store_sd(&result.nearesteven, nearesteven);
  _mm_store_sd(&result.up, up);
  _mm_store_sd(&result.down, down);
  _mm_store_sd(&result.zero, zero);
  return result;
}

int main(void) {
  struct result result = add(1.0, 0x1p-53);
  printf("nearesteven: %a\n", result.nearesteven);
  printf("up: %a\n", result.up);
  printf("down: %a\n", result.down);
  printf("zero: %a\n", result.zero);
}
