#include <stdio.h>
#include <sys/auxv.h>

void func_with_stack_buffer_overflow(void) {
  char buf[16];
  printf("Enter a string: ");
  scanf("%s", buf);
  printf("You entered: %s\n", buf);
}

int main(void) { func_with_stack_buffer_overflow(); }
