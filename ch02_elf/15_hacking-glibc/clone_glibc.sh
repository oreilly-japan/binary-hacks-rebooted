#! /bin/bash -eux

SCRIPT_DIR=$(realpath $(dirname "$0"))
cd ${SCRIPT_DIR}

if [[ ! -d glibc-source ]]
then
    git clone https://github.com/akawashiro/glibc.git glibc-source
    git checkout origin/hacking-glibc
fi

mkdir -p glibc-build
mkdir -p glibc-install
cd glibc-build
PREFIX_PATH=$(realpath ../glibc-install)
LD_LIBRARY_PATH="" ${SCRIPT_DIR}/glibc-source/configure CC="ccache gcc" --prefix=${PREFIX_PATH}
