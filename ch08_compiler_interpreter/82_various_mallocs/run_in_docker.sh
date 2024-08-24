#! /bin/bash -eux

FIND_PROF_DIR=$(realpath ./locate_prof)
rm -rf ${FIND_PROF_DIR}
mkdir -p ${FIND_PROF_DIR}
HEAPPROFILE=${FIND_PROF_DIR}/prof LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libtcmalloc.so.4.5.9 find .
google-pprof --svg $(which find) ${FIND_PROF_DIR}/prof.0001.heap > find_prof.svg

gcc -shared -fPIC -o libtoo_simple_malloc.so -Wl,-soname,libtoo_simple_malloc.so too_simple_malloc.c
LD_PRELOAD=./libtoo_simple_malloc.so find .
