#include <stdio.h>
#include <stdlib.h>

#define DECLARE_NORMAL_FUNC(name)            \
  void name(char *p) {                       \
    (void)p; /* suppress Wunused-variable */ \
    puts(#name " called");                   \
  }

DECLARE_NORMAL_FUNC(f0)
DECLARE_NORMAL_FUNC(f1)
DECLARE_NORMAL_FUNC(f2)
DECLARE_NORMAL_FUNC(f3)
DECLARE_NORMAL_FUNC(f4)
DECLARE_NORMAL_FUNC(f5)
DECLARE_NORMAL_FUNC(f6)

void f7(char *buf) {
  puts("f7 called");
  // format string bug
  printf(buf);
}

typedef void (*func_ptr)(char *);
func_ptr funcs[] = {f0, f1, f2, f3, f4, f5, f6, f7};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s path/to/file\n", argv[0]);
    exit(0);
  }

  FILE *fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    exit(1);
  }

  char *buf = NULL;
  int cur_size = 0;
  while (1) {
    int c = fgetc(fp);
    if (c == EOF) break;

    buf = realloc(buf, cur_size + 1);
    if (!buf) {
      perror("realloc");
      exit(1);
    }

    buf[cur_size++] = (char)c;
  }

  fclose(fp);

  if (cur_size < 1000) {
    puts("invalid length");
    exit(0);
  }

  srand(0xdeadbeef);

  int func_idx = 0;
  while (1) {
    int rnd_pos = rand() % 1000;
    char rnd_char = rand() % 256;
    if (buf[rnd_pos] != rnd_char) {
      puts("nope");
      exit(0);
    }

    funcs[func_idx++](buf);
    if (func_idx == sizeof(funcs) / sizeof(func_ptr)) {
      puts("nice");
      exit(0);
    }
  }
}
