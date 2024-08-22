#! /bin/bash

set -eux -o pipefail

gcc -o ifunc_basic ifunc_basic.c
./ifunc_basic

gcc -shared -fpic -fPIC bar.c -o libbar.so
gcc -z now -o main-now main.c libbar.so

LD_LIBRARY_PATH=. ./main-now
LD_BIND_NOT=1 LD_LIBRARY_PATH=. ./main-now

gcc -shared -fpic -fPIC bar.c -o libbar.so
gcc -z lazy -o main main.c libbar.so


LD_LIBRARY_PATH=. ./main
LD_BIND_NOW=1 LD_LIBRARY_PATH=. ./main
