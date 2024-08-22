#include <stdio.h>

int f_int_int_1(int x) {
  puts("int_int_1");
  return 0;
}

int f_int_int_2(int x) {
  puts("int_int_2");
  return 0;
}

char f_char_int(int x) {
  puts("char_int");
  return 0;
}

int main(void) {
  int (*ptr)(int);

  printf("int_int_1 = %p\n", f_int_int_1);
  printf("int_int_2 = %p\n", f_int_int_2);
  printf(" char_int = %p\n", f_char_int);

  printf("\nInput ptr: ");
  scanf("%p", &ptr);
  ptr(0);
}
