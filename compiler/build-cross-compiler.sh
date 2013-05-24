export GCC_VER=4.4.7
export BINUTILS_VER=2.20.1
export PREFIX=`pwd`/cross
export TARGET=i586-elf
export HOME_DIR=`pwd`

echo == Creating build directory ==================================================

if [ ! -f build ]; then
	mkdir build
fi

cd $HOME_DIR/build

echo == Downloading binutils source ===============================================

if [ ! -f binutils-$BINUTILS_VER.tar.bz2 ]; then
	wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.bz2
	tar xfv binutils-$BINUTILS_VER.tar.bz2
fi

echo == Configuring binutils ======================================================

if [ ! -f build-binutils ]; then
	mkdir build-binutils
fi
cd build-binutils
../binutils-${BINUTILS_VER}/configure --target=$TARGET --prefix=$PREFIX --disable-nls

echo == binutils make all =========================================================

make all

echo == binutils make install =====================================================

make install

cd $HOME_DIR/build

echo == Downloading gcc source ====================================================

if [ ! -f gcc-$GCC_VER.tar.bz2 ]; then
	wget http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.bz2
	tar xfv gcc-${GCC_VER}.tar.bz2
fi

echo == Configuring GCC ===========================================================

if [ ! -f build-gcc ]; then
	mkdir build-gcc
fi
cd build-gcc
export PATH=$PATH:$PREFIX/bin
../gcc-$GCC_VER/configure --target=$TARGET --prefix=$PREFIX --disable-nls \
    --enable-languages=c,c++ --without-headers

echo == GCC make all-gcc ==========================================================

make all-gcc

echo == GCC make all-target-libgcc ================================================

make all-target-libgcc

echo == GCC make install-gcc ======================================================

make install-gcc

echo == GCC make install-target-libgcc ============================================

make install-target-libgcc
