#include <stdio.h>

int main(void) {
  register long c asm("r12") = 0x1234567812345678;
  for (size_t i = 0;; i++) {
    printf("i=%3zu c=0x%lx\n", i, c);
    for (size_t j = 0; j < 3000000000; j++)
      ;
  }
}
