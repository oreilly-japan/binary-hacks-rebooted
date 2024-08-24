#! /bin/bash

set -eux -o pipefail

gcc -o crash crash.c

sudo bash -c 'echo core > /proc/sys/kernel/core_pattern'
sudo bash -c 'echo 0 > /proc/sys/kernel/core_uses_pid'
ulimit -c unlimited
sudo echo 511 > /proc/self/coredump_filter
set +e
setarch $(uname -m) -R ./crash
set -e

file core
readelf -l core

readelf --note core

gcc -o show_pic show_pic.c
./show_pic
