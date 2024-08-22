#include <stdio.h>
unsigned long int data_var = 0xaabbccddaabbccdd;
unsigned long int bss_var;
int main(void) {
  printf("data_var: %p\n", &data_var);
  printf("bss_var: %lu\n", bss_var);
}
