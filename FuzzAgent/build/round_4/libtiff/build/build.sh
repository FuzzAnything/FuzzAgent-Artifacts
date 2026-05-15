#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Set DEBIAN_FRONTEND to noninteractive to avoid apt-utils warnings
export DEBIAN_FRONTEND=noninteractive

# Install build dependencies
apt-get update && apt-get install -y --no-install-recommends \
    apt-utils \
    ca-certificates \
    cmake \
    autoconf \
    automake \
    libtool \
    pkg-config \
    nasm

# Install library dependencies (development packages)
apt-get install -y --no-install-recommends \
    zlib1g-dev \
    libjpeg-dev \
    libjbig-dev \
    liblzma-dev

# Append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Create install directories
mkdir -p "$WORK/lib" "$WORK/include"

# Check for ASAN and add appropriate flags
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    echo "ASAN instrumentation detected"
fi

# Build libtiff using CMake
echo "Building libtiff..."
cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=OFF \
    -Dtiff-tools=OFF \
    -Dtiff-tests=OFF \
    -Dtiff-contrib=OFF \
    -Dtiff-docs=OFF \
    -Dtiff-deprecated=OFF \
    $extra_cmake_flags

make -j$(nproc)
make install

# Verify the libraries were created
if [ -f "$WORK/lib/libtiff.a" ] || [ -f "$WORK/lib/libtiffxx.a" ]; then
    echo "Build successful: libraries found in $WORK/lib"
    echo "Libraries in $WORK/lib:"
    ls -la "$WORK/lib/" || true
else
    echo "Error: Libraries not found in $WORK/lib"
    ls -la "$WORK/lib/" 2>/dev/null || echo "$WORK/lib does not exist"
    exit 1
fi
