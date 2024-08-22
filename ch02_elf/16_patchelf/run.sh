#!/bin/bash -eux

SCRIPT_DIR=$(dirname "$0")
ROOT_DIR=$(git rev-parse --show-toplevel)
PWD=$(pwd)

# e.g.
# ROOT_DIR=/path/to/root
# PWD=/path/to/root/book/os/kvm
# RELATIVE_PATH=book/os/kvm
RELATIVE_PATH=$(realpath -m --relative-to="$ROOT_DIR" "$PWD")

DOCKER_IAMGE_NAME=bh2-$(basename $(realpath ${SCRIPT_DIR}))
docker build . -t ${DOCKER_IAMGE_NAME} --network=host
docker run --mount type=bind,source="$(pwd)",target=/bh2 --workdir=/bh2 ${DOCKER_IAMGE_NAME} /bh2/run_in_docker.sh
