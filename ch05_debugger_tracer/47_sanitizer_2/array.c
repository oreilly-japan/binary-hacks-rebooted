#include <stdint.h>
#include <stdio.h>
uint64_t array[10];
int main(void) {
  int index = 12;
  array[index] = 42;
  printf("%llu\n", array[index]);
  return 0;
}
