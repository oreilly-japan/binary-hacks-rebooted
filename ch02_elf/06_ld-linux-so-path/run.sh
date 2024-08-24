#!/bin/bash -eux

set -eux -o pipefail

gcc -o libfoo.so -shared -fPIC foo.c
gcc -o main main.c libfoo.so
LD_LIBRARY_PATH=. ./main

readelf --dynamic main | grep NEEDED

set +e

cp ./main ./main_suid
sudo chown root:root ./main_suid
sudo chmod u+s ./main_suid
LD_LIBRARY_PATH=. ./main_suid

set -e

mkdir main_runpath_app
mkdir -p main_runpath_app/bin
mkdir -p main_runpath_app/lib
gcc -o main_runpath_app/lib/libfoo.so -shared -fPIC foo.c
gcc -o main_runpath_app/bin/main_runpath main.c main_runpath_app/lib/libfoo.so -Wl,-rpath='$ORIGIN/../lib'
LD_DEBUG=libs ./main_runpath_app/bin/main_runpath

tree ./main_runpath_app

readelf --dynamic ./main_runpath_app/bin/main_runpath | grep RUNPATH
