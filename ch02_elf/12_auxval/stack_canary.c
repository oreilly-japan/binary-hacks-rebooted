#include <stdio.h>
#include <sys/auxv.h>

void func_with_stack_buffer_over_flow(void) {
  char buf[16];
  printf("Enter a string: ");
  scanf("%s", buf);
  printf("You entered: %s\n", buf);

  unsigned long *stack = (unsigned long *)buf;
  for (int i = 0; i < 4; i++) {
    printf("*(stack + %d): 0x%016lx\n", i, *(stack + i));
  }
}

int main(void) {
  unsigned long *at_random = (unsigned long *)getauxval(AT_RANDOM);
  printf("*at_random: 0x%lx\n", *at_random);

  func_with_stack_buffer_over_flow();
}
