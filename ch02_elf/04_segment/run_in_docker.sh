#!/bin/bash
set -eux -o pipefail

gcc -o hello hello.c
readelf -l hello
