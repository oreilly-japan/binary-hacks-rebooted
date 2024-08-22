#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t add_alloca(int n) {
  alloca(sizeof(uint64_t) * n);
  uint64_t ar[2] = {0xaaaa, 0xbbbb};
  return ar[0] + ar[1];
}

int main(void) { add_alloca(3); }
