#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
  int fd = open("./file.c", O_RDONLY);
  assert(fd != -1);

  int num_read = 0;
  while (1) {
    char c;

    ssize_t res = read(fd, &c, sizeof(char));
    if (res == 0) break;
    if (res == -1) continue;

    write(STDOUT_FILENO, &c, sizeof(char));
    num_read += res;
  }

  close(fd);
}
