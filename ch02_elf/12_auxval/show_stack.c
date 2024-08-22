#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[], char *envp[]) {
  printf("========== argv ==========\n");
  for (int i = 0; i < argc; i++) {
    printf("(argv + %d)=%p, argv[%d]=%s\n", i, argv + i, i, argv[i]);
  }

  printf("========== envp ==========\n");
  int j;
  for (j = 0; envp[j] != NULL; j++) {
    printf("(envp + %2d)=%p, envp[%d]=%s\n", j, envp + j, j, envp[j]);
  }

  Elf64_auxv_t *auxv = (Elf64_auxv_t *)(envp + j + 1);
  printf("========== auxv ==========\n");
  for (int i = 0; (auxv + i)->a_type != AT_NULL; i++) {
    printf(
        "(auxv + %2d)=%p, (auxv + %2d)->a_type=%2lu,\n",
        i, (auxv + i), i, (auxv + i)->a_type);
    printf(
        "(auxv + %2d)->a_un.a_val=%16lx\n",
        i, (auxv + i)->a_un.a_val);
  }
}
