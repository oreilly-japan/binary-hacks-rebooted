#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int));
  *p = 1;
  free(p);
  int c = *p;
  printf("%d\n", c);
  return 0;
}
