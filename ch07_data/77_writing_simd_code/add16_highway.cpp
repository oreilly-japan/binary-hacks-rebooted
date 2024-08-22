#include "hwy/highway.h"

void add16(const float *a, const float *b, float *c) {
  const HWY_FULL(float) d;
  for (int i = 0; i < 16; i += Lanes(d)) {
    const auto va = Load(d, a + i);
    const auto vb = Load(d, b + i);
    const auto vc = va + vb;
    Store(vc, d, c + i);
  }
}
