#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

void *malloc(size_t size) {
  write(1, "malloc in too_simple_malloc\n", 29);
  return mmap(NULL, size, PROT_READ | PROT_WRITE,
              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void free(void *p) {}
