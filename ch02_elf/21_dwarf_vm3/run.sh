#!/bin/bash

docker build -t binary-hacks-dwarf .
docker run --rm --cap-add=SYS_PTRACE --security-opt="seccomp=unconfined" -it binary-hacks-dwarf /bin/bash
