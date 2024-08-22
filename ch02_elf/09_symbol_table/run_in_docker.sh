#! /bin/bash

set -eux -o pipefail

echo "========= sysv hash ========="
rm -fr sysv_hash
mkdir sysv_hash
pushd sysv_hash
cp ../foo.c .
cp ../main.c .
gcc -o libfoo.so -shared -fPIC -Wl,--hash-style=sysv foo.c
gcc -o main -Wl,--hash-style=sysv main.c libfoo.so
LD_BIND_NOW=1 LD_DEBUG=statistics LD_LIBRARY_PATH=. ./main
popd

echo "========= gnu hash ========="
rm -fr gnu_hash
mkdir gnu_hash
pushd gnu_hash
cp ../foo.c .
cp ../main.c .
gcc -o libfoo.so -shared -fPIC -Wl,--hash-style=gnu foo.c
gcc -o main -Wl,--hash-style=gnu main.c libfoo.so
LD_BIND_NOW=1 LD_DEBUG=statistics LD_LIBRARY_PATH=. ./main
popd
