#include <fcntl.h>
void untrusted_func(void) { int fd = open("./secret_token.txt", O_RDONLY); }
