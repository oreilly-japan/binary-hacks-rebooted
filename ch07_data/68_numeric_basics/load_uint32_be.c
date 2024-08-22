#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

uint32_t load_uint32(const uint8_t *p) {
  return *((const uint32_t *)p);  // x86-64 はリトルエンディアン
}

uint32_t load_uint32_be(const uint8_t *p) {
  uint32_t raw = *((const uint32_t *)p);
  return _bswap(raw);
}

int main(void) {
  uint8_t bytes[4] = {0, 1, 2, 3};  // ビッグエンディアンで解釈すると
                                    // 0x00010203
  printf("%08x, %08x\n", load_uint32(bytes), load_uint32_be(bytes));
}
