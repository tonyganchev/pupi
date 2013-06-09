#!/usr/bin/bash
source config.sh

echo == Creating build directory ==================================================

if [ ! -d build ]; then
	mkdir build
fi

cd $HOME_DIR/build

echo == Downloading binutils source ===============================================

if [ ! -f binutils-$BINUTILS_VER.tar.bz2 ]; then
	wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.bz2
fi
if [ ! -d binutils-$BINUTILS_VER ]; then
	tar xfv binutils-$BINUTILS_VER.tar.bz2
fi

echo == Configuring binutils ======================================================

if [ ! -d build-binutils ]; then
	mkdir build-binutils
fi
cd build-binutils
../binutils-${BINUTILS_VER}/configure --target=$TARGET --prefix=$PREFIX --disable-nls

echo == binutils make all =========================================================

make all

echo == binutils make install =====================================================

make install

cd $HOME_DIR/build