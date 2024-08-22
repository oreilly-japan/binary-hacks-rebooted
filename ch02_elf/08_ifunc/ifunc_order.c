#include <stdio.h>

void foo(void);
__attribute__((constructor)) void hoge(void) { printf("hoge\n"); }

int main(void) {
  printf("main\n");
  foo();
  return 0;
}
