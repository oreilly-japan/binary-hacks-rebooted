#include <stdlib.h>
#include <x86intrin.h>

void add16(const float *a, const float *b, float *restrict c) {
  const __m512 va = _mm512_load_ps(a);
  const __m512 vb = _mm512_load_ps(b);
  const __m512 vc = _mm512_add_ps(va, vb);
  _mm512_store_ps(c, vc);
}
