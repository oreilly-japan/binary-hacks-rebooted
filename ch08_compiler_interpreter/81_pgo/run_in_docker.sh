#! /bin/bash

clang-15 -o print print.c
./print 0 0 | head -n 4

clang-15 -O2 -fprofile-instr-generate -o print print.c
clang-15 -O2 -o print_nonpgo print.c
objdump --disassemble=main print_nonpgo

rm -f *profraw
rm -rf *profdata

LLVM_PROFILE_FILE="print00.profraw" ./print 0 0 > /dev/null
llvm-profdata-15 merge -output=print00.profdata print00.profraw
clang-15 -O2 -fprofile-instr-use=print00.profdata -o print00_pgo print.c
objdump --no-addresses --no-show-raw-insn -M intel --disassemble=main print00_pgo | tail -n -34 | head -n 32

LLVM_PROFILE_FILE="print01.profraw" ./print 0 1 > /dev/null
llvm-profdata-15 merge -output=print01.profdata print01.profraw
clang-15 -O2 -fprofile-instr-use=print01.profdata -o print01_pgo print.c
objdump  --no-show-raw-insn --disassemble=main print01_pgo | tail -n -34 | head -n 32

LLVM_PROFILE_FILE="print10.profraw" ./print 1 0 > /dev/null
llvm-profdata-15 merge -output=print10.profdata print10.profraw
clang-15 -O2 -fprofile-instr-use=print10.profdata -o print10_pgo print.c
objdump --no-addresses -M intel --no-show-raw-insn --disassemble=main print10_pgo | tail -n -34 | head -n 32

LLVM_PROFILE_FILE="print11.profraw" ./print 1 1 > /dev/null
llvm-profdata-15 merge -output=print11.profdata print11.profraw
clang-15 -O2 -fprofile-instr-use=print11.profdata -o print11_pgo print.c
objdump  --no-show-raw-insn --disassemble=main print11_pgo | tail -n -34 | head -n 32

hyperfine --style basic --warmup 3 --min-runs 20 'taskset -c 0 ./print00_pgo 0 0'
hyperfine --style basic --warmup 3 --min-runs 20 'taskset -c 0 ./print00_pgo 0 1'
hyperfine --style basic --warmup 3 --min-runs 20 'taskset -c 0 ./print00_pgo 1 0'
hyperfine --style basic --warmup 3 --min-runs 20 'taskset -c 0 ./print00_pgo 1 1'
