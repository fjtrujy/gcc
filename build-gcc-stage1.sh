#!/bin/bash

TARGET_ALIAS=$1
if [ -z "$2" ]
then
  TARGET=$TARGET_ALIAS
else
  TARGET=$2
fi

OSVER=$(uname)
## Apple needs to pretend to be linux
if [ ${OSVER:0:6} == Darwin ]; then
	TARG_XTRA_OPTS="--build=i386-linux-gnu --host=i386-linux-gnu"
else
	TARG_XTRA_OPTS=""
fi

## Determine the maximum number of processes that Make can work with.
PROC_NR=$(getconf _NPROCESSORS_ONLN)

## Prevent regeneration of generated c files since it causes compile errors
## gcc 3.2.3 only ?
# touch -c toolchain/$TARGET_ALIAS/gcc/gcc/c-parse.c
# touch -c toolchain/$TARGET_ALIAS/gcc/gcc/cp/parse.c
# touch -c toolchain/$TARGET_ALIAS/gcc/gcc/tradcif.c

## Create and enter the toolchain/build directory
mkdir -p build && cd build || { exit 1; }

## Create and enter the build directory.
# rm -rf gcc-$TARGET_ALIAS-stage1 && mkdir gcc-$TARGET_ALIAS-stage1 && cd gcc-$TARGET_ALIAS-stage1 || { exit 1; }

## Configure the build.
../configure \
  --quiet \
  --prefix="$PS2DEV/$TARGET_ALIAS" \
  --target="$TARGET" \
  --enable-languages="c" \
  --with-float=hard \
  --with-newlib \
  --disable-nls \
  --disable-shared \
  --disable-libssp \
  --disable-libmudflap \
  --disable-threads \
  --disable-libgomp \
  --disable-libquadmath \
  --disable-target-libiberty \
  --disable-target-zlib \
  --without-ppl \
  --without-cloog \
  --with-headers=no \
  --disable-libada \
  --disable-libatomic \
  --disable-multilib \
  $TARG_XTRA_OPTS || { exit 1; }

## Compile and install.
make --quiet -j $PROC_NR clean   || { exit 1; }
make --quiet -j $PROC_NR all     || { exit 1; }
make --quiet -j $PROC_NR install || { exit 1; }
make --quiet -j $PROC_NR clean   || { exit 1; }

## Exit the build directory
cd .. || { exit 1; }

## Exit the toolchain/build directory
cd ../.. || { exit 1; }
