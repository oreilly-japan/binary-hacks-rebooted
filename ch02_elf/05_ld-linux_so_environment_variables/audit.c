#define _GNU_SOURCE
#include <link.h>
#include <stdio.h>

unsigned int la_version(unsigned int version) { return LAV_CURRENT; }

unsigned int la_objopen(struct link_map *map, Lmid_t lmid,
                        uintptr_t *Cookie) {
  printf("[LD_AUDIT] %s loaded\n", map->l_name);
  return LA_FLG_BINDTO | LA_FLG_BINDFROM;
}

ElfW(Addr)
    la_x86_64_gnu_pltenter(ElfW(Sym) * sym, unsigned int ndx,
                           uintptr_t *refcook, uintptr_t *defcook,
                           La_x86_64_regs *regs, unsigned int *flags,
                           const char *symname, long *framesizep) {
  printf("[LD_AUDIT] %s entering\n", symname);
  return sym->st_value;
}
