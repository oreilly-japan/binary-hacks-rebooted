#!/bin/bash -eux

gcc -fno-omit-frame-pointer -fno-stack-protector -mno-red-zone -o stack-vanilla stack.c
gcc -fno-stack-protector -fomit-frame-pointer -mno-red-zone -o stack-without-fp stack.c
gcc -fno-stack-protector -fomit-frame-pointer -mno-red-zone -g -o stack-alloca stack-alloca.c
gcc -fno-omit-frame-pointer -fstack-protector-all -mno-red-zone -o stack-with-ssp stack.c
gcc -fno-stack-protector -fomit-frame-pointer -o stack-without-fp-with-red-zone stack.c
gcc stack.c -o stack-default-gcc
gcc stack.c -o stack-default-gcc-no-stacl-protector -fno-stack-protector
clang-15 stack.c -o stack-default-clang

objdump --disassemble=add --no-show-raw-insn -M intel stack-vanilla
objdump --disassemble=add --no-show-raw-insn -M intel stack-without-fp
objdump --disassemble=add_alloca --no-show-raw-insn -M intel stack-alloca
objdump --disassemble=add --no-show-raw-insn -M intel stack-with-ssp
