#include <stdint.h>
#include <stdio.h>
uint64_t array[10];
int main(void) {
  // 上の例で生成されるアセンブリがarray+redzoneのために
  // 128バイトを確保していたので、それを超えるindexを指定してみる
  int index = 20;
  array[index] = 42;
  printf("%llu\n", array[index]);
  return 0;
}
