/* read_auxv.c */
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[], char *envp[]) {
  printf("========== argv ==========\n");
  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }

  printf("========== envp ==========\n");
  char *last_envp = NULL;
  for (int i = 0; envp[i] != NULL; i++) {
    printf("%p %s\n", envp[i], envp[i]);
    last_envp = envp[i];
  }

  printf("========== auxv ==========\n");
  Elf64_auxv_t *auxv = (Elf64_auxv_t *)(last_envp + strlen(last_envp) + 8);
  while (auxv->a_type != AT_NULL) {
    printf("Type: %lu, Value: %lx\n", auxv->a_type, auxv->a_un.a_val);
    auxv++;
  }
}
