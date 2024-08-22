#include <stdio.h>
int main(void) {
  FILE *fp;
  char buf[1024];
  fp = fopen("/proc/self/maps", "r");
  while (fgets(buf, 1024, fp)) {
    printf("%s", buf);
  }
  fclose(fp);
  return 0;
}
