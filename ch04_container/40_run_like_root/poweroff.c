#include <stdio.h>
#include <sys/reboot.h>
#include <unistd.h>

int main(void) {
  sync();
  if (reboot(RB_POWER_OFF) == -1) {
    perror("Failed to power off the system");
    return 1;
  }
  fprintf(stderr, "The system did not power off as expected.\n");
  return 1;
}
