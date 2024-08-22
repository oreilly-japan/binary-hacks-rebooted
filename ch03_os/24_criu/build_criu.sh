#! /bin/bash

set -eux -o pipefail

sudo apt-get install git build-essential asciidoc xmlto pkg-config libprotobuf-dev libprotobuf-c-dev protobuf-c-compiler protobuf-compiler python3-protobuf libbsd-dev libnl-3-dev libnet-dev libgnutls28-dev libcap-dev python3-pip
git clone https://github.com/checkpoint-restore/criu.git
cd criu
make
