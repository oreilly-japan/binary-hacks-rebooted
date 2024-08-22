#! /bin/bash

set -eux -o pipefail

seq -f "%05g" 1 10000 > seq.txt
gcc -o read_seq read_seq.c
./read_seq
