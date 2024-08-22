#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int (*real_main)(int, char **, char **) = NULL;

int wrapped_main(int argc, char **argv, char **envp) {
  puts("Hello from wrapped_main()!");
  return real_main(argc, argv, envp);
}

int __libc_start_main(int (*main)(int, char **, char **), int argc,
                      char **ubp_av, void (*init)(void),
                      void (*fini)(void), void (*rtld_fini)(void),
                      void(*stack_end)) {
  // __libc_start_mainへのポインタを宣言
  int (*real_libc_start_main)(int (*)(int, char **, char **), int,
                              char **, void (*)(void), void (*)(void),
                              void (*)(void), void(*));

  real_libc_start_main = dlsym(RTLD_NEXT, "__libc_start_main");
  if (real_libc_start_main == NULL) {
    fprintf(stderr, "dlsym error: %s\n", dlerror());
    exit(1);
  }

  // 引数のmainをグローバル変数に保存しておくことで、
  // wrapped_mainがあとから呼び出せる
  real_main = main;

  // 本物の __libc_start_mainを呼び出す
  // このとき、第一引数に自前のwrapped_mainを渡す
  return real_libc_start_main(wrapped_main, argc, ubp_av, init, fini,
                              rtld_fini, stack_end);
}
