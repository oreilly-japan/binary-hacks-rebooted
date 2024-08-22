#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

size_t encode_vbyte(uint32_t x, uint8_t *b) {
  size_t i = 0;
  while (x > 0x7f) {
    b[i++] = (x & 0x7f) | 0x80;   // ビット8に「継続ビット」を付加
    x >>= 7;
  }
  b[i++] = x;                     // 最後のバイトの継続ビットは0
  return i;
}

size_t decode_vbyte(uint8_t *b, uint32_t *x) {
  size_t i = 0;
  uint32_t t = 0;
  while ((b[i] & 0x80) != 0) {
    t |= (b[i] & 0x7f)<<(i * 7);  // 継続ビットが1のバイトを連結
    i++;
  }
  *x = t | (b[i]<<(i * 7));       // 最後のバイトの継続ビットは0
  return i;
}

int main(void) {
  uint8_t buf[16] = {0};
  uint32_t original = 300;
  printf("original: %u\n", original);

  size_t bytes = encode_vbyte(original, buf);
  printf("encoded: ");
  for (size_t i = 0; i < bytes; i++) {
    printf("%02x%c", buf[i], i == bytes - 1 ? '\n' : ',');
  }

  uint32_t decoded;
  decode_vbyte(buf, &decoded);
  printf("decoded: %u\n", decoded);
  return 0;
}
