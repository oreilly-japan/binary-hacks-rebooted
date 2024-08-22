#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/procfs.h>
#include <unistd.h>

int main(void) {
  int fd = open("./core", O_RDONLY);
  size_t size = lseek(fd, 0, SEEK_END);
  size_t mapped_size = (size + 0xfff) & ~0xfff;
  char *head = (char *)(mmap(
      NULL, mapped_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0));
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)(head);
  for (size_t pi = 0; pi < ehdr->e_phnum; pi++) {
    Elf64_Phdr *phdr = ((Elf64_Phdr *)(head + ehdr->e_phoff)) + pi;
    if (phdr->p_type == PT_NOTE) {
      size_t offset_in_note = 0;
      while (offset_in_note < phdr->p_filesz) {
        Elf64_Nhdr *note =
            (Elf64_Nhdr *)(head + phdr->p_offset + offset_in_note);
        uint32_t type = note->n_type;
        if (type == NT_PRSTATUS) {
          prstatus_t *prstatus =
              (prstatus_t *)(head + phdr->p_offset + offset_in_note +
                             4 * 3 + (note->n_namesz + 3) / 4 * 4);
          struct user_regs_struct *regs =
              (struct user_regs_struct *)(prstatus->pr_reg);
          printf("RIP: 0x%llx\n", regs->rip);
        }

        /* 実際のサイズは4の倍数になるようにパディングされている */
        offset_in_note += sizeof(Elf64_Nhdr) +
                          (note->n_descsz + 3) / 4 * 4 +
                          (note->n_namesz + 3) / 4 * 4;
      }
    }
  }
}
