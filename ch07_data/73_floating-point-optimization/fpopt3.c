#include <fenv.h>
#include <stdio.h>

// FORCE_REREAD(x)
// は「変数の値が変わったかもしれない」とコンパイラに思わせるマクロ
// FORCE_EVAL(x)
// は「変数の値が利用されるかもしれない」とコンパイラに思わせるマクロ
#if defined(__x86_64__)
// x: any SSE register
#define FORCE_REREAD(x) asm volatile("" : "+x"(x))
#define FORCE_EVAL(x) asm volatile("" : : "x"(x))
#elif defined(__aarch64__)
// w: Floating point register
#define FORCE_REREAD(x) asm volatile("" : "+w"(x))
#define FORCE_EVAL(x) asm volatile("" : : "w"(x))
#else
#error not supported
#endif

struct result {
  double up, down;
};
struct result add(double x, double y) {
  fesetround(FE_UPWARD);
  FORCE_REREAD(x);  // 以後の x を使った計算はこの行の後に行われることを保証する
  FORCE_REREAD(y);  // 以後の y を使った計算はこの行の後に行われることを保証する
  double up = x + y;
  FORCE_EVAL(up);  // up の計算がこの行の前に行われることを保証する
  fesetround(FE_DOWNWARD);
  FORCE_REREAD(x);  // 以後の x を使った計算はこの行の後に行われることを保証する
  FORCE_REREAD(y);  // 以後の y を使った計算はこの行の後に行われることを保証する
  double down = x + y;
  FORCE_EVAL(down);  // down の計算がこの行の前に行われることを保証する
  fesetround(FE_TONEAREST);
  return (struct result){.up = up, .down = down};
}

int main(void) {
  struct result r = add(1.0, 0x1p-53);
  printf("up: %a\n", r.up);
  printf("down: %a\n", r.down);
}
