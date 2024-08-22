#include <stdint.h>
#include <stdio.h>

int32_t add_int8_as_int32(const int8_t *a, const int8_t *b) {
  return *a + *b;
}

int main(void) {
  int8_t a = 2, b = -5;
  int32_t c = add_int8_as_int32(&a, &b);
  printf("%08x\n", c);
}
