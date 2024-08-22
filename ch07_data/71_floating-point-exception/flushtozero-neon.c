#include <arm_neon.h>
#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON

int main(int argc, char *argv[]) {
  if (argc > 1 && strcmp(argv[1], "NEON") == 0) {
    puts("NEON");
    volatile float32x4_t a = vdupq_n_f32(0x1p-126f);
    volatile float32x4_t b = vdupq_n_f32(0x0.c0ffeep0f);
    volatile float32x4_t c = vdupq_n_f32(0x0.c0ffeep-126f);
    feclearexcept(FE_UNDERFLOW);
    volatile float32x4_t x = vmulq_f32(a, b);
    printf("FE_UNDERFLOW is %sset.\n",
           fetestexcept(FE_UNDERFLOW) ? "" : "not ");
    printf("%a\n", vgetq_lane_f32(x, 0));
    feclearexcept(FE_UNDERFLOW);
    volatile float32x4_t y = vsubq_f32(a, c);
    printf("FE_UNDERFLOW is %sset.\n",
           fetestexcept(FE_UNDERFLOW) ? "" : "not ");
    printf("%a\n", vgetq_lane_f32(y, 0));
  } else {
    puts("standard");
    volatile float a = 0x1p-126f;
    volatile float b = 0x0.c0ffeep0f;
    volatile float c = 0x0.c0ffeep-126f;
    feclearexcept(FE_UNDERFLOW);
    volatile float x = a * b;
    printf("FE_UNDERFLOW is %sset.\n",
           fetestexcept(FE_UNDERFLOW) ? "" : "not ");
    printf("%a\n", x);
    feclearexcept(FE_UNDERFLOW);
    volatile float y = a - c;
    printf("FE_UNDERFLOW is %sset.\n",
           fetestexcept(FE_UNDERFLOW) ? "" : "not ");
    printf("%a\n", y);
  }
}
