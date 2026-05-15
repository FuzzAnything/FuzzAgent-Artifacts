#!/bin/bash
set -e

# Install build dependencies
apt-get update && apt-get install -y \
    apt-utils \
    autoconf \
    automake \
    libtool \
    pkg-config \
    zlib1g-dev \
    libbz2-dev \
    liblzma-dev \
    libzstd-dev \
    liblz4-dev

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Copy source to build directory
cp -r "$SRC"/* .

# Generate configure script if it doesn't exist
if [ ! -f configure ]; then
    autoreconf -fiv
fi

# Configure with appropriate flags
# Use the system-provided CC/CXX and append to CFLAGS/CXXFLAGS
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

./configure \
    --prefix="$WORK" \
    --disable-shared \
    --enable-static \
    --enable-elf \
    --enable-elf-core \
    --enable-zlib \
    --enable-bzlib \
    --enable-xzlib \
    --enable-zstdlib \
    --disable-libseccomp \
    --disable-fsect-man5 \
    --disable-warnings \
    CC="$CC" \
    CXX="$CXX"

# Build and install - skip the magic.mgc generation by touching it first
# The magic.mgc generation might fail in fuzzing environment due to seccomp
touch magic/magic.mgc 2>/dev/null || true

make -j$(nproc) V=1 || {
    echo "First make attempt failed, trying to skip magic.mgc..."
    # Try to build without magic.mgc if the first attempt fails
    cd /tmp/build_tmp
    make -j$(nproc) -k || true
}

make install

# Verify installation
if [ -f "$WORK/lib/libmagic.a" ]; then
    echo "Successfully built libmagic.a"
    ls -la "$WORK/lib/"
else
    echo "ERROR: libmagic.a not found in $WORK/lib"
    ls -la "$WORK/" || true
    ls -la "$WORK/lib/" || true
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
