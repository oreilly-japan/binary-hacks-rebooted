#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int));
  *p = 1;
  free(p);
  printf("%d\n", *p);  // 解放済みのpにアクセス!!!
  return 0;
}
