#! /bin/bash -eux

SCRIPT_DIR=$(realpath $(dirname "$0"))
PID=$(pidof read_seq)
DUMP_DIR=${SCRIPT_DIR}/criu-ns-dump-${PID}

mkdir -p ${DUMP_DIR}
sudo ~/.local/sbin/criu-ns dump --criu-binary ~/.local/sbin/criu -t ${PID} --shell-job -j -v4 -D ${DUMP_DIR}
read -p "Hit enter to run sudo ~/.local/sbin/criu-ns restore --criu-binary ~/.local/sbin/criu --shell-job -j -v4 -D ${DUMP_DIR}: "
sudo ~/.local/sbin/criu-ns restore --criu-binary ~/.local/sbin/criu --shell-job -j -v4 -D ${DUMP_DIR}
