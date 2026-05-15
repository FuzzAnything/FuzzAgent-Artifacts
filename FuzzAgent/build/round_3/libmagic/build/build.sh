#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Install dependencies needed for compression support
DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
    zlib1g-dev \
    liblzma-dev \
    libbz2-dev \
    liblz4-dev \
    libzstd-dev \
    autoconf \
    automake \
    libtool

# Run autoreconf in source directory to generate configure script
cd "$SRC"
autoreconf -i
cd /tmp/build_tmp

# Configure with static library support and enable features
# Use the provided compiler and flags
export CC="$CC"
export CXX="$CXX"
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

"$SRC"/configure \
    --prefix="$WORK" \
    --enable-static \
    --disable-shared \
    --enable-fsect-man5 \
    --with-pic

# Build and install
make -j$(nproc)
make install

# Verify the static library was created
if [ -f "$WORK/lib/libmagic.a" ]; then
    echo "Successfully built libmagic.a"
    ls -la "$WORK/lib/"
else
    echo "Error: libmagic.a not found!"
    exit 1
fi
