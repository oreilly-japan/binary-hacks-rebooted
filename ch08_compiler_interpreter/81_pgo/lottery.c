#include <stdio.h>
#include <stdlib.h>

__attribute__((noinline)) void daikichi(void) { puts("大吉"); }
__attribute__((noinline)) void kichi(void) { puts("吉"); }
__attribute__((noinline)) void kyo(void) { puts("凶"); }

int main(void) {
  for (int i = 0; i < 10000; i++) {
    double r = (double)rand() / RAND_MAX;
    if (r < 0.2) {
      daikichi();
    } else if (r < 0.9) {
      kichi();
    } else {
      kyo();
    }
  }
}
