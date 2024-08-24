#! /bin/bash -eux

SCRIPT_DIR=$(realpath $(dirname "$0"))

cd ${SCRIPT_DIR}

# Clone
if [[ ! -d glibc-source ]]
then
    git clone https://github.com/akawashiro/glibc.git glibc-source
    pushd glibc-source
    git checkout origin/hacking-glibc
    popd
fi

# Configure
rm -rf glibc-build glibc-install
mkdir -p glibc-build
mkdir -p glibc-install
cd glibc-build
PREFIX_PATH=$(realpath ../glibc-install)
LD_LIBRARY_PATH="" ${SCRIPT_DIR}/glibc-source/configure CC="ccache gcc" --prefix=${PREFIX_PATH}

# Build
cd ${SCRIPT_DIR}/glibc-build
LD_LIBRARY_PATH="" bear -- make all -j 20
LD_LIBRARY_PATH="" bear -- make install -j 20
