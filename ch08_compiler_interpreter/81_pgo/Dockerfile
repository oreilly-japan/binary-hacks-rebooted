FROM ubuntu:22.04
RUN apt-get update
RUN apt-get install -y build-essential clang-15 wget
RUN wget https://github.com/sharkdp/hyperfine/releases/download/v1.16.1/hyperfine_1.16.1_amd64.deb
RUN dpkg -i hyperfine_1.16.1_amd64.deb
WORKDIR /bh
