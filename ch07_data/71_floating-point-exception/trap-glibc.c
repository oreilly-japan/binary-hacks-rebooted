#define _GNU_SOURCE
#include <fenv.h>
#include <stdint.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

int main(void) {
  int result = feenableexcept(FE_ALL_EXCEPT);
  if (result == -1) {
    puts("feenableexcept failed");
  } else {
    puts("feenableexcept succeeded");
  }
  volatile double zero = 0.0, one = 1.0;
  printf("1.0 / 0.0 = %g\n", one / zero);
}
