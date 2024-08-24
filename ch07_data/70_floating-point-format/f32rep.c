#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void print_f32rep(const char *label, float a) {
  // uint32_t pattern = *(uint32_t *)&a;
  // と書くとstrict aliasing ruleに反するので注意
  uint32_t pattern;
  memcpy(&pattern, &a, 4);
  printf("%11s: 0x%08" PRIx32 "\n", label, pattern);
}

int main(void) {
  print_f32rep("-1.0", -1.0f);
  print_f32rep("0.0", 0.0f);
  print_f32rep("0xcafep-149", 0xcafep-149f);
  // ↑16進リテラルのp以降は底2の指数部
  // つまりこれは0xcafe * 2^(-149)を表す
  print_f32rep("1.0/0.0", 1.0f / 0.0f);
  print_f32rep("0.0/0.0", 0.0f / 0.0f);
}
