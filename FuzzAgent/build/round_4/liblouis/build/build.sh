#!/bin/bash
set -e

# Install dependencies
apt-get update && apt-get install -y \
    autoconf \
    automake \
    libtool \
    pkg-config \
    libyaml-dev \
    make \
    gcc \
    g++

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir -p /tmp/build_tmp
cd /tmp/build_tmp

# Copy source and prepare for build
cp -r "$SRC"/* .

# Run autogen.sh to generate configure script
./autogen.sh

# Configure with prefix to install to $WORK
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

./configure \
    --prefix="$WORK" \
    --disable-shared \
    --enable-static \
    --with-yaml \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS"

# Build and install
make -j$(nproc)
make install

# Verify installation
echo "Checking installation in $WORK"
ls -la "$WORK/lib/" || echo "No lib directory created"
ls -la "$WORK/include/" || echo "No include directory created"
