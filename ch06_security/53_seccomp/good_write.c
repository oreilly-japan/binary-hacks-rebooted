#include <string.h>
#include <unistd.h>
void untrusted_func(void) {
  char msg[] = "Hello!\n";
  write(1, msg, strlen(msg));
}
