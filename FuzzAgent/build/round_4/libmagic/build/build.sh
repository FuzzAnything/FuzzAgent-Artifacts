#!/bin/bash
set -e

# Install build dependencies
apt-get update && apt-get install -y \
    zlib1g-dev \
    liblz4-dev \
    libbz2-dev \
    liblzma-dev \
    libzstd-dev \
    autoconf \
    automake \
    libtool \
    pkg-config

# Create build directory
rm -rf /tmp/build_tmp && mkdir -p /tmp/build_tmp && cd /tmp/build_tmp

# Copy source
cp -r "$SRC"/* .

# Generate configure script if needed
if [ ! -f configure ]; then
    autoreconf -i
fi

# Configure with proper flags for static library
# Append to CFLAGS/CXXFLAGS, don't overwrite
export CFLAGS="$CFLAGS -DHAVE_CONFIG_H"
export CXXFLAGS="$CXXFLAGS -DHAVE_CONFIG_H"

./configure \
    --prefix="$WORK" \
    --enable-static \
    --enable-fsect-man5 \
    --disable-shared \
    --host=x86_64-linux-gnu

# Build
make -j$(nproc) V=1

# Install
make install

# Verify installation
echo "Checking installed files:"
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true
