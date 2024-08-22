#include <dlfcn.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint64_t thunk(void (*func)(), uint64_t regargs[6],
                      size_t nbytes, void *stackargs);

int main(int argc, char *argv[]) {
  void *lib = dlopen("./mylib.so", RTLD_LAZY);
  if (lib == NULL) {
    perror("failed to open mylib.so");
    return 1;
  }

  if (argc < 4 || argc > 8) {
    fprintf(stderr, "Usage: %s <function name> <arg1> <arg2> ...\n",
            argv[0]);
    return 1;
  }

  char *func_name = argv[1];
  printf("func_name: %s\n", func_name);

  void *func = dlsym(lib, func_name);
  if (func == NULL) {
    return 1;
  }

  uint64_t args[6] = {0};
  for (int i = 2; i < argc; i++) {
    args[i - 2] = atoi(argv[i]);
    printf("args[%d] = %ld\n", i - 2, args[i - 2]);
  }

  int32_t result = (int32_t)thunk(func, args, 0, NULL);

  printf("result: %d\n", result);

  return 0;
}
