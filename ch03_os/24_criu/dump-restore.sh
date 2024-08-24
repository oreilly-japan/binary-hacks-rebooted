#! /bin/bash -eux

SCRIPT_DIR=$(realpath $(dirname "$0"))
PID=$(pidof read_seq)
DUMP_DIR=${SCRIPT_DIR}/criu-dump-${PID}
CRIU_PATH=${SCRIPT_DIR}/criu/criu/criu

mkdir -p ${DUMP_DIR}
sudo ${CRIU_PATH} dump -t ${PID} --shell-job -j -v4 -D ${DUMP_DIR}
read -p "Hit enter: "
sudo ${CRIU_PATH} restore --shell-job -j -v4 -D ${DUMP_DIR}
