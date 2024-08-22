#! /bin/bash

set -eux -o pipefail

gcc -o libhoge.so -shared -fPIC hoge.c
gcc -o main main.c libhoge.so
LD_LIBRARY_PATH=. ./main

gcc -o libfuga.so -shared -fPIC fuga.c
LD_PRELOAD=./libfuga.so LD_LIBRARY_PATH=. ./main

LD_DEBUG=symbols LD_LIBRARY_PATH=. ./main
LD_DEBUG=symbols LD_PRELOAD=./libfuga.so LD_LIBRARY_PATH=. ./main

gcc -o libaudit.so -shared -fPIC audit.c
# LD_AUDIT doesn't work in container environment.
LD_AUDIT=libaudit.so LD_LIBRARY_PATH=. ./main
