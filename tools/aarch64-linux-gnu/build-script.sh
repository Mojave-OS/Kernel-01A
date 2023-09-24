#!/bin/sh
echo "Making our compiler for: $TARGET"
cd /osdev/src/
mkdir binutils
mkdir gcc

cd binutils
../build-binutils/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd /osdev/src/gcc
../build-gcc/configure --target=$TARGET --prefix="$PREFIX" -enable-languages=c --disable-nls --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
