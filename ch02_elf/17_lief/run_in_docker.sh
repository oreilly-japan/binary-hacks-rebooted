#! /bin/bash

set -eux -o pipefail

pip3 install lief==0.14.1

# Build libadd.so
gcc-11 -fPIC -shared -o libadd.so add.c

# Rename symbol
python3 rename.py
readelf --dyn-syms libadd.so
readelf --dyn-syms libadd_renamed.so

gcc-11 -o use_libadd_renamed use_libadd_renamed.c libadd_renamed.so
LD_LIBRARY_PATH=. ./use_libadd_renamed

# Inject mul
cat mul.c
gcc-11 -Os -nostdlib -nodefaultlibs -fPIC -Wl,-shared mul.c -o mul
cat inject.py
python3 inject.py
cat use_libadd_injected.c
gcc-11 -o use_libadd_injected use_libadd_injected.c libadd_injected.so
LD_LIBRARY_PATH=. ./use_libadd_injected

# Extract mul
dd if=./libadd_injected.so of=./extracted_mul bs=1 skip=$((0x5000)) count=$((0x10))
objdump --target=binary --disassembler-options=x86-64,intel --architecture=i386 --disassemble-all --no-show-raw-insn extracted_mul
