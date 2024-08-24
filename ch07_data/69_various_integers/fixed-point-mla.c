#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int32_t inner_product(const int32_t *a, const int32_t *b, size_t n) {
  int32_t accum = 0;
  for (size_t i = 0; i < n; i++) {
    accum += (a[i] * b[i] + 0x80)>>8;
  }
  return accum;
}

int main(void) {
  // 0.25を小数部8ビットの固定小数点数にエンコードすると、
  // 0.25 * 2^8 = 2^(-2) * 2^8 = 2^6 = 0x40となる。
  int32_t a[2] = {0x0040, 0x0280};    // 0.25, 2.5
  int32_t b[2] = {0x0400, 0x0080};    // 4.0, 0.5
  int32_t p = inner_product(a, b, 2); // 0.25 * 4.0 + 2.5 * 0.5
                                      //   = 1.0 + 1.25 = 2.25
  printf("0x%08x\n", p);
}
