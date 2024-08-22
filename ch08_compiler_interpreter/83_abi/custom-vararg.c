#include <stdarg.h>
#include <stdio.h>

int main(void) {
  va_list va;
  char reg_save_area[176];
  va->gp_offset = 8;
  va->fp_offset = 48;
  va->reg_save_area = reg_save_area;
  va->overflow_arg_area = NULL;
  *(int *)(reg_save_area + 8) = 42;
  *(double *)(reg_save_area + 48) = 2.0;
  *(long *)(reg_save_area + 16) = -123;
  *(double *)(reg_save_area + 64) = -3.14;
  vprintf("%d, %f, %ld, %f\n", va);
}
