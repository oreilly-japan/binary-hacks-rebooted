#!/bin/bash -eux

LD_SHOW_AUXV=1 sleep 1
gcc -o use_getauxval use_getauxval.c
./use_getauxval
gcc -o stack_canary stack_canary.c
echo "AAAAAAAAAAAAAAAA" | ./stack_canary
gcc -o show_stack show_stack.c
env -i LD_SHOW_AUXV=1 ./show_stack argv0 argv1 argv2
