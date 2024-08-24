#!/bin/bash
set -eux -o pipefail

gcc-11 -o hello_c hello.c
./hello_c

gcc-11 -o hello_c_syscall hello_syscall.c
./hello_c_syscall

gcc-11 -o hello_c_syscall_2 hello_syscall_2.c
./hello_c_syscall_2

nasm -f elf64 ./hello_syscall.asm
ld -o hello_syscall ./hello_syscall.o
./hello_syscall

readelf -l hello_syscall

objdump -d hello_syscall -M intel

nasm -f elf64 ./hello_for_elfhack.asm
ld -o hello_for_elfhack ./hello_for_elfhack.o
./hello_for_elfhack

readelf -l hello_for_elfhack
objdump -d hello_for_elfhack -M intel
xxd -seek 0x1000 -len 0x31 -include hello_for_elfhack

gcc-11 -o hello_exec_byte hello_exec_byte.c
./hello_exec_byte
