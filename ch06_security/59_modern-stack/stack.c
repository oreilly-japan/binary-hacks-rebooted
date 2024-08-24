#include <stdint.h>
#include <stdio.h>

uint64_t add(void) {
  uint64_t ar[2] = {0xaaaa, 0xbbbb};
  return ar[0] + ar[1];
}

int main(void) { add(); }
