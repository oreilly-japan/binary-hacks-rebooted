/* jump_at_entry.c */
#include <elf.h>
#include <stdio.h>
#include <sys/auxv.h>
int main(void) {
  puts("Jumping to entry point...");
  void (*entry)(void) = (void *)getauxval(AT_ENTRY);
  entry();
}
