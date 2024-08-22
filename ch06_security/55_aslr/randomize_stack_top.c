#ifndef STACK_RND_MASK
#define STACK_RND_MASK (0x7ff >> (PAGE_SHIFT - 12)) /* 8MB of VA */
#endif

unsigned long randomize_stack_top(unsigned long stack_top) {
  unsigned long random_variable = 0;

  if (current->flags & PF_RANDOMIZE) {
    random_variable = get_random_long();
    random_variable &= STACK_RND_MASK;
    random_variable <<= PAGE_SHIFT;
  }
#ifdef CONFIG_STACK_GROWSUP
  return PAGE_ALIGN(stack_top) + random_variable;
#else
  return PAGE_ALIGN(stack_top) - random_variable;
#endif
}
