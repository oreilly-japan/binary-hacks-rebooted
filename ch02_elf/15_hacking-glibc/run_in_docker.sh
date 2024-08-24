#!/bin/bash -eux

SCRIPT_DIR=$(realpath $(dirname "$0"))

rm -rf "glibc-source"

ldd --version
git clone https://sourceware.org/git/glibc.git glibc-source --branch glibc-2.35 --depth 1

rm -rf "glibc-build" "glibc-install"

mkdir glibc-build glibc-install
pushd glibc-build
LD_LIBRARY_PATH="" ../glibc-source/configure --prefix=$(realpath ../glibc-install)
make all -j
make install -j
popd

./glibc-install/lib/ld-linux-x86-64.so.2 /bin/date -d @1234567890 +"%Y/%m/%d %T"

cat rewrite-puts.patch
patch glibc-source/libio/ioputs.c rewrite-puts.patch
pushd glibc-build
make all -j
make install -j
popd

cat hello.c
gcc -o hello hello.c
LD_PRELOAD=./glibc-install/lib/libc.so.6 ./hello

cat use_tls.c
gcc -o use_tls use_tls.c
./use_tls
./glibc-install/lib/ld-linux-x86-64.so.2 ./use_tls

patch -R glibc-source/libio/ioputs.c rewrite-puts.patch
cat rewrite-tls.patch
patch glibc-source/elf/dl-tls.c rewrite-tls.patch
pushd glibc-build
make all -j
make install -j
popd

./glibc-install/lib/ld-linux-x86-64.so.2 ./use_tls
