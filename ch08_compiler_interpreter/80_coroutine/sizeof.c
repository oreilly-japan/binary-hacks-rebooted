#define _GNU_SOURCE
#include <assert.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#ifndef _NSIG
#define _NSIG 64
#endif

#ifdef __i386__
#define _NSIG_BPW 32
#else
#define _NSIG_BPW 64
#endif

#define _NSIG_WORDS (_NSIG / _NSIG_BPW)

typedef unsigned long old_sigset_t; /* at least 32 bits */

typedef struct {
  unsigned long sig[_NSIG_WORDS];
} linux_sigset_t;

int main(void) {
  struct sigaction sa;
  printf("ucontext_t %zu\n", sizeof(ucontext_t));
  printf("jmp_buf %zu\n", sizeof(jmp_buf));
  printf("sigjmp_buf %zu\n", sizeof(sigjmp_buf));
  printf("sigset_t %zu\n", sizeof(sigset_t));
  printf("linux_sigset_t %zu\n", sizeof(linux_sigset_t));
  printf("sa_mask %zu\n", sizeof(sa.sa_mask));
}
