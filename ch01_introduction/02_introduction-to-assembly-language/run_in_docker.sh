#! /bin/bash

set -eux -o pipefail

gcc -o add add.c
objdump --disassemble=add -M intel --no-addresses add
