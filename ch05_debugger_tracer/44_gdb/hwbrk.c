#include <stdio.h>

int main(void) {
  static int r = 10;
  static int w = 20;
  static int rw = 30;

  printf("r: %p, w: %p, rw: %p\n", &r, &w, &rw);

  printf("%d\n", r);
  w++;
  printf("%d\n", rw);
  rw++;
}
