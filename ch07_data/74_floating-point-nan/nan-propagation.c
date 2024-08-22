#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

float make_nan(uint32_t payload) {
  float a;
  uint32_t pattern = UINT32_C(0x7fc00000) | payload;
  memcpy(&a, &pattern, 4);
  return a;
}

void print_f32rep(const char *label, float a) {
  uint32_t pattern;
  memcpy(&pattern, &a, 4);
  printf("%7s: 0x%08" PRIx32 "\n", label, pattern);
}

int main(void) {
  float a = make_nan(0xcafe);
  float b = make_nan(0x80001234);
  print_f32rep("a", a);
  print_f32rep("b", b);
  print_f32rep("- a", -a);
  print_f32rep("fabs(b)", fabs(b));
  print_f32rep("a + 1.0", a + 1.0);
  print_f32rep("1.0 + b", 1.0 + b);
  print_f32rep("a + b", a + b);
  print_f32rep("b + a", b + a);
  print_f32rep("a * 2.0", a * 2.0);
  print_f32rep("2.0 * b", 2.0 * b);
  print_f32rep("a * b", a * b);
  print_f32rep("b * a", b * a);
}
