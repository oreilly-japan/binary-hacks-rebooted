#include <stdint.h>
int main(void) {
  uint64_t a = 0xaaaabbbbccccdddd;
  for (int i = 0;; i++) {
    *(&a + i) = 0xdeadbeefdeadbeef;
  }
}
