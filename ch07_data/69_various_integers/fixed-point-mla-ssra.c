#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int32_t inner_product(const int32_t *a, const int32_t *b, size_t n) {
  int32_t sum;
  __asm__ volatile (
    "  mov  x3, 0\n"                // ループカウンタx3を0で初期化
    "  movi v2.2s, 0\n"             // アキュムレータv2を0で初期化
    "  lsr  %3, %3, #1\n"           // n >>= 1; 簡単のため、nが奇数のとき
                                    // 末尾の1要素を切り捨てる
    // SIMD命令で2要素ずつ処理するループ
    "1:\n"
    "  cmp  x3, %3\n"               // if (x3 >= n) break;
    "  bge  2f\n"
    "  ldr  d0, [%1, x3, lsl #3]\n" // a[x3]からv0の下位2要素にロード
    "  ldr  d1, [%2, x3, lsl #3]\n" // b[x3]からv1の下位2要素にロード
    "  mul  v0.2s, v0.2s, v1.2s\n"  // v0[i] <- v0[i] * v1[i]
    "  ssra v2.2s, v0.2s, 8\n"      // v2[i] <-
                                    //   v2[i] + (v0[i] + 0x80)>>8
    "  add  x3, x3, 1\n"            // x3++
    "  b    1b\n"
    "2:\n"
    "  addp v2.2s, v2.2s, v2.2s\n"
    "  fmov %w0, s2\n"
    : "=r" (sum)
    : "r" (a), "r" (b), "r" (n)
    : "x3", "v0", "v1", "v2"
  );
  return sum;
}

int main(void) {
  // 0.25を小数部8ビットの固定小数点数にエンコードすると、
  // 0.25 * 2^8 = 2^(-2) * 2^8 = 2^6 = 0x40となる。
  int32_t a[2] = {0x0040, 0x0280};    // 0.25, 2.5
  int32_t b[2] = {0x0400, 0x0080};    // 4.0, 0.5
  int32_t p = inner_product(a, b, 2); // 0.25 * 4.0 + 2.5 * 0.5
                                      //   = 1.0 + 1.25 = 2.25
  printf("0x%08x\n", p);
}
