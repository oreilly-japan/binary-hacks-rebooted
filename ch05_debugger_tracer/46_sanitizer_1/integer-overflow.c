#include <stdint.h>
#include <stdio.h>
int main(int argc, char** argv) {
  // コンパイラの最適化によって消されないように入力に依存した計算をおこなう
  int32_t a = INT32_MAX;
  printf("a = %d\n", a);
  printf("argc = %d\n", argc);

  // INT32_MAX + argc => オーバーフロー!!
  int32_t b = a + argc;
  printf("a + argc = %d\n", b);
  return 0;
}
