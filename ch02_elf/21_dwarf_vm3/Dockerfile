FROM ubuntu:20.04@sha256:0b897358ff6624825fb50d20ffb605ab0eaea77ced0adb8c6a4b756513dec6fc

WORKDIR /dwarf

RUN apt update -y
RUN apt install -y g++ python3 gdb

COPY test_rop.s .
COPY final_throw_exception.s .

RUN g++ test_rop.s -o test_rop
RUN g++ final_throw_exception.s -o throw_exception
