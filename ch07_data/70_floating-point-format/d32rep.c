#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void print_d32rep(const char *label, _Decimal32 a) {
  uint32_t pattern;
  memcpy(&pattern, &a, 4);
  printf("%9s: 0x%08" PRIx32 "\n", label, pattern);
}

int main(void) {
  print_d32rep("1.", 1.df);
  print_d32rep("1.0", 1.0df);
  print_d32rep("1.000000", 1.000000df);
  print_d32rep("1.0000000", 1.0000000df);
  print_d32rep("0.0", 0.0df);
  print_d32rep("42e-101", 42e-101df);
  print_d32rep("1.0/0.0", 1.0df / 0.0df);
  print_d32rep("0.0/0.0", 0.0df / 0.0df);
}
