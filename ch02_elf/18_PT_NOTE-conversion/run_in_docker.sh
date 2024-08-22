#! /bin/bash

set -eux -o pipefail

GCC_VERSION=$(gcc-11 --version | head -n 1)
EXPECTED_GCC_VERSION="gcc-11 (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"

if [ "${GCC_VERSION}" != "${EXPECTED_GCC_VERSION}" ]; then
  echo "GCC version is not expected: ${GCC_VERSION}"
  exit 1
fi

gcc-11 -o hello hello.c -static -no-pie
readelf -h ./hello | grep "Entry point address"
readelf --notes hello

nasm -f elf64 youkoso.asm
ld -o youkoso youkoso.o
readelf -l ./youkoso

dd skip=$((0x1000)) count=$((0x3e)) if=youkoso of=youkoso_load bs=1

HELLO_SIZE=$(ls -l hello | awk '{print $5}')
PADDED_SIZE=$(((${HELLO_SIZE} + 4095) & ~4095))
DUMMY_SIZE=$((${PADDED_SIZE} - ${HELLO_SIZE}))

dd count=${DUMMY_SIZE} if=/dev/zero of=dummy bs=1
cat ./hello ./dummy ./youkoso_load > ./hello_youkoso
chmod u+x ./hello_youkoso

gcc-11 convert.c -o convert
./convert ${PADDED_SIZE}
chmod u+x ./hello_youkoso_converted

./hello_youkoso_converted
