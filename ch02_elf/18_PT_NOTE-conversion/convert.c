/* convert.c */
#include <elf.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) {
  int fd = open("./hello_youkoso", O_RDONLY);
  size_t size = lseek(fd, 0, SEEK_END);
  size_t mapped_size = (size + 0xfff) & ~0xfff;
  char *p = (char *)mmap(NULL, mapped_size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE, fd, 0);

  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)p;
  const int youkoso_size = 0x2f;
  const int youkoso_file_offset = 901120;
  const int youkoso_addr = 0x4d0000;

  /* ELF ヘッダのエントリポイントを書き換える */
  ehdr->e_entry = youkoso_addr;

  /* プログラムヘッダをyoukoso_loadの部分をロードするように書き換える */
  for (size_t pi = 0; pi < ehdr->e_phnum; pi++) {
    Elf64_Phdr *phdr = ((Elf64_Phdr *)(p + ehdr->e_phoff)) + pi;
    if (phdr->p_type == PT_NOTE) {
      phdr->p_align = 0x1000;
      phdr->p_filesz = youkoso_size;
      phdr->p_memsz = youkoso_size;
      phdr->p_offset = youkoso_file_offset;
      phdr->p_type = PT_LOAD;
      phdr->p_flags = PF_X | PF_R;
      phdr->p_vaddr = youkoso_addr;
      phdr->p_paddr = youkoso_addr;
      break;
    }
  }

  FILE *fp = fopen("./hello_youkoso_converted", "wb");
  fwrite(p, size, 1, fp);
  return 0;
}
