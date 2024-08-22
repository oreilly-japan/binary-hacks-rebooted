#include <stdint.h>

void add_int8(const int8_t *a, const int8_t *b, int8_t *restrict c) {
  *c = *a + *b;
}
