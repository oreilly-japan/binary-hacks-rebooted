#!/bin/bash -ux

SCRIPT_DIR=$(realpath $(dirname "$0"))

cp /bin/ls .
./ls -l ./ls
readelf -l ./ls | grep INTERP -A 2

nasm -f elf64 hello.asm
ld -s -o hello hello.o
./hello

patchelf --set-interpreter ./hello ./ls
./ls -l ./ls
readelf -l ./ls | grep INTERP -A 2

gcc -o libfuga.so -shared -fPIC fuga.c
gcc -o use_fuga use_fuga.c libfuga.so

rm -rf tmp
mkdir -p tmp
cp use_fuga tmp
cd tmp
./use_fuga
patchelf --set-rpath '$ORIGIN/..' use_fuga
./use_fuga
