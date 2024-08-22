#include <stdio.h>
#include <unistd.h>

FILE *map_open(void) {
  int pid = getpid();
  char name[128];
  snprintf(name, sizeof(name), "/tmp/perf-%d.map", pid);
  return fopen(name, "a+");
}

void map_add(FILE *fp, const char *name, const void *begin, size_t size) {
  fprintf(fp, "%llx %zx %s\n", (long long)begin, size, name);
}
