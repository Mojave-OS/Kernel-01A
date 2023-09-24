FROM ubuntu:latest
WORKDIR /usr/share/osdev

RUN apt-get update
RUN apt-get upgrade -y 

# install compilation related items
RUN apt-get install -y build-essential bison flex texinfo libgmp3-dev libmpc-dev

# development related items
RUN apt-get install -y git curl zsh vim

# prepare environment for cross-compiler
RUN mkdir /osdev
RUN mkdir /osdev/src

# install binaries for our cross-compiler
RUN git clone git://sourceware.org/git/binutils-gdb.git /osdev/src/build-binutils
RUN git clone git://gcc.gnu.org/git/gcc.git /osdev/src/build-gcc

# set some environment variables
# this is dependent on where we ware wanting
# to compile to.
ENV TARGET=aarch64-arm-none-eabi
ENV PREFIX="/osdev/src/cross"
ENV PATH="$PREFIX/bin:$PATH"

COPY build-script.sh ./
CMD /bin/bash
