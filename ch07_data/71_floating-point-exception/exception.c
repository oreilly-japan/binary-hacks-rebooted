#include <fenv.h>
#include <math.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

void print_except(void) {
  int e = fetestexcept(FE_ALL_EXCEPT);
  if (e == 0) {
    puts("No exception");
  } else {
    if (e & FE_INVALID) printf("FE_INVALID, ");
    if (e & FE_DIVBYZERO) printf("FE_DIVBYZERO, ");
    if (e & FE_OVERFLOW) printf("FE_OVERFLOW, ");
    if (e & FE_UNDERFLOW) printf("FE_UNDERFLOW, ");
    if (e & FE_INEXACT) printf("FE_INEXACT, ");
    puts("");
  }
}

int main(void) {
  feclearexcept(FE_ALL_EXCEPT);
  print_except();
  volatile double tmp = sqrt(-1.0);
  print_except();
  feclearexcept(FE_ALL_EXCEPT);
  tmp = 1.0 / 0.0;
  print_except();
  feclearexcept(FE_ALL_EXCEPT);
  tmp = 1e300 * 1e300;
  print_except();
  feclearexcept(FE_ALL_EXCEPT);
  tmp = 1e-300 * 1e-300;
  print_except();
  (void)tmp;
}
