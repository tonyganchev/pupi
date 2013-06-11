#!/bin/bash

NEWLIBC_VER=2.0.0

NEWLIBC_NAME=newlib-${NEWLIBC_VER}
NEWLIBC_TAR=${NEWLIBC_NAME}.tar.gz
SRC_URL=ftp://sourceware.org/pub/newlib/${NEWLIBC_TAR}

ROOT_DIR=`pwd`
WORK_DIR=${ROOT_DIR}/work
BUILD_DIR=${WORK_DIR}/build
SRC_DIR=${WORK_DIR}/${NEWLIBC_NAME}
INSTALL_DIR=$ROOT_DIR

if [ ! -d $WORK_DIR ]; then
	mkdir $WORK_DIR
fi

if [ ! -d $BUILD_DIR ]; then
	mkdir $BUILD_DIR
fi

cd $WORK_DIR

if [ ! -f ${NEWLIBC_TAR} ]; then
	wget $SRC_URL
fi

if [ ! -d $SRC_DIR ]; then
	tar xfvz $NEWLIBC_TAR
fi

cd $BUILD_DIR

${SRC_DIR}/configure --target=i586-elf --prefix=$INSTALL_DIR

make

make install

