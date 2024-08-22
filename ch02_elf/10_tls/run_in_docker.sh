#! /bin/bash

set -eux -o pipefail

gcc -std=c11 -o libhoge.so -shared -fPIC hoge.c
gcc -std=c11 -o main main.c ./libhoge.so

objdump -d libhoge.so --disassemble=general_dynamic_access -M intel --no-show-raw-insn

readelf -r libhoge.so

objdump -d libhoge.so --disassemble=local_dynamic_access -M intel --no-show-raw-insn

objdump -d main --disassemble=initial_executable_access -M intel --no-show-raw-insn

readelf -r main

objdump -d main --disassemble=local_executable_access -M intel --no-show-raw-insn
