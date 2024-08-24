#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  char buf[16];
  int fd = open("seq.txt", O_RDONLY);

  while (1) {
    read(fd, buf, 6);
    printf("pid:%d seq.txt:%s", getpid(), buf);
    sleep(1);
  }
}
