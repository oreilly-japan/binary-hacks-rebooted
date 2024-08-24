#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void print_f32bits(float x) {
  uint32_t i;
  memcpy(&i, &x, 4);
  printf("%08" PRIx32 "\n", i);
}

void print_f64bits(double x) {
  uint64_t i;
  memcpy(&i, &x, 8);
  printf("%016" PRIx64 "\n", i);
}

extern double float_as_double(float x);
extern float as_float(double x);

int main(void) {
  double x = float_as_double(0x0.c0ffeep0f);
  printf("%g\n", x);
  print_f64bits(x);
  float y = as_float(0x1.cafecafecafeap0);
  printf("%g\n", y);
  print_f32bits(y);
}
