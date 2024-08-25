#include <arm_acle.h>
#include <fenv.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON

enum direction { TONEAREST, TONEARESTFROMZERO, UPWARD, DOWNWARD, TOWARDZERO };

void test_signed(enum direction d, double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  int32_t i;
  switch (d) {
    case TONEAREST:
      asm("fcvtns %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case TONEARESTFROMZERO:
      asm("fcvtas %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case UPWARD:
      asm("fcvtps %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case DOWNWARD:
      asm("fcvtms %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case TOWARDZERO:
      asm("fcvtzs %w0, %d1" : "=r"(i) : "w"(x));
      break;
  }
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("%g to int32_t = %" PRId32 " (0x%08" PRIx32
         "), %s FE_INVALID, %s FE_INEXACT\n",
         x, i, (uint32_t)i, invalid ? "with" : "without",
         inexact ? "with" : "without");
}

void test_unsigned(enum direction d, double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  uint32_t i;
  switch (d) {
    case TONEAREST:
      asm("fcvtnu %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case TONEARESTFROMZERO:
      asm("fcvtau %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case UPWARD:
      asm("fcvtpu %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case DOWNWARD:
      asm("fcvtmu %w0, %d1" : "=r"(i) : "w"(x));
      break;
    case TOWARDZERO:
      asm("fcvtzu %w0, %d1" : "=r"(i) : "w"(x));
      break;
  }
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("%g to uint32_t = %" PRIu32 " (0x%08" PRIx32
         "), %s FE_INVALID, %s FE_INEXACT\n",
         x, i, i, invalid ? "with" : "without", inexact ? "with" : "without");
}

#if defined(__ARM_FEATURE_JCVT)
void test_jcvt(double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  int32_t i = __jcvt(x);
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("__jcvt(%g) = %" PRId32 " (0x%08" PRIx32
         "), %s FE_INVALID, %s FE_INEXACT\n",
         x, i, (uint32_t)i, invalid ? "with" : "without",
         inexact ? "with" : "without");
}
#endif

#if defined(__ARM_FEATURE_FRINT)
void test_rint32z(double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  double i = __rint32z(x);
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("__rint32z(%g) = %f (%a), %s FE_INVALID, %s FE_INEXACT\n", x, i, i,
         invalid ? "with" : "without", inexact ? "with" : "without");
}

void test_rint32x(double x) {
  feclearexcept(FE_INVALID | FE_INEXACT);
  double i = __rint32x(x);
  int invalid = fetestexcept(FE_INVALID) != 0;
  int inexact = fetestexcept(FE_INEXACT) != 0;
  printf("__rint32x(%g) = %f (%a), %s FE_INVALID, %s FE_INEXACT\n", x, i, i,
         invalid ? "with" : "without", inexact ? "with" : "without");
}
#endif

int main(int argc, char *argv[]) {
  enum direction d = TONEAREST;
  if (argc > 1) {
    if (strcmp(argv[1], "tonearest") == 0)
      d = TONEAREST;
    else if (strcmp(argv[1], "tonearestfromzero") == 0)
      d = TONEARESTFROMZERO;
    else if (strcmp(argv[1], "upward") == 0)
      d = UPWARD;
    else if (strcmp(argv[1], "downward") == 0)
      d = DOWNWARD;
    else if (strcmp(argv[1], "towardzero") == 0)
      d = TOWARDZERO;
    else if (strcmp(argv[1], "jcvt") == 0) {
#if defined(__ARM_FEATURE_JCVT)
      test_jcvt(-2.5);
      test_jcvt(-3.14);
      test_jcvt(-3.99);
      test_jcvt(1e300);
      test_jcvt(INFINITY);
      test_jcvt(-INFINITY);
      test_jcvt(NAN);
      return 0;
#else
      puts("JCVT is not available");
      return 1;
#endif
    } else if (strcmp(argv[1], "rint32z") == 0) {
#if defined(__ARM_FEATURE_FRINT)
      test_rint32z(-2.5);
      test_rint32z(-3.14);
      test_rint32z(-3.99);
      test_rint32z(1e300);
      test_rint32z(INFINITY);
      test_rint32z(-INFINITY);
      test_rint32z(NAN);
      return 0;
#else
      puts("FRINT is not available");
      return 1;
#endif
    } else if (strcmp(argv[1], "rint32x") == 0) {
#if defined(__ARM_FEATURE_FRINT)
      test_rint32x(-2.5);
      test_rint32x(-3.14);
      test_rint32x(-3.99);
      test_rint32x(1e300);
      test_rint32x(INFINITY);
      test_rint32x(-INFINITY);
      test_rint32x(NAN);
      return 0;
#else
      puts("FRINT is not available");
      return 1;
#endif
    }
  }
  test_signed(d, -2.5);
  test_signed(d, -3.14);
  test_signed(d, -3.99);
  test_signed(d, 1e300);
  test_signed(d, INFINITY);
  test_signed(d, -INFINITY);
  test_signed(d, NAN);
  test_unsigned(d, 2.5);
  test_unsigned(d, -3.14);
  test_unsigned(d, -3.99);
  test_unsigned(d, 1e300);
  test_unsigned(d, INFINITY);
  test_unsigned(d, -INFINITY);
  test_unsigned(d, NAN);
}
