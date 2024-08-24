#include <stdint.h>
#include <stdio.h>

_Thread_local uint64_t tls_variable = 0xabcdabcdabcdabcd;

int main(void) {
  puts("Hello, World!\n");
  return 0;
}
