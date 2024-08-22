#include <stdio.h>

struct result {
  double nearesteven, nearestaway, up, down, zero;
};
struct result add(double x, double y) {
  double even, away, up, down, zero;
  asm("fadd.d %0, %1, %2, rne" : "=f"(even) : "f"(x), "f"(y));
  asm("fadd.d %0, %1, %2, rmm" : "=f"(away) : "f"(x), "f"(y));
  asm("fadd.d %0, %1, %2, rup" : "=f"(up) : "f"(x), "f"(y));
  asm("fadd.d %0, %1, %2, rdn" : "=f"(down) : "f"(x), "f"(y));
  asm("fadd.d %0, %1, %2, rtz" : "=f"(zero) : "f"(x), "f"(y));
  return (struct result){
      .nearesteven = even,
      .nearestaway = away,
      .up = up,
      .down = down,
      .zero = zero,
  };
}

int main(void) {
  struct result result = add(1.0, 0x1p-53);
  printf("nearesteven: %a\n", result.nearesteven);
  printf("nearestaway: %a\n", result.nearestaway);
  printf("up: %a\n", result.up);
  printf("down: %a\n", result.down);
  printf("zero: %a\n", result.zero);
}
