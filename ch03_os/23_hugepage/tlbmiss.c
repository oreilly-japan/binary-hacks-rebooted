#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  const size_t PAGE_SIZE = 4096;
  if (argc < 2) {
    puts("tlbmiss <num_page>");
    return 1;
  }
  size_t num_page = atoi(argv[1]);
  size_t region_size = PAGE_SIZE * num_page;
  char *data = malloc(region_size);
  memset(data, 0, region_size);
  int nloop = 4096;

  /* 最適化で消えないようにvolatileにする */
  volatile char *vdata = data;
  for (int x = 0; x < nloop; x++) {
    for (size_t i = 0; i < num_page; i++) {
      vdata[i * PAGE_SIZE] = 1;
    }
  }
  free(data);

  return 0;
}
