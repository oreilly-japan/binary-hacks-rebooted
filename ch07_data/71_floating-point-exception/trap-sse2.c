#include <fenv.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

#pragma STDC FENV_ACCESS ON

int main(void) {
  unsigned int csr = _mm_getcsr();
  _mm_setcsr(csr & ~0x1f80u);  // ~(PM | UM | OM | ZM | DM | IM)
  volatile double zero = 0.0, one = 1.0;
  printf("1.0 / 0.0 = %g\n", one / zero);
}
