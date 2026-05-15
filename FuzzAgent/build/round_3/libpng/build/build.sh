#!/bin/bash
set -e

# Install build dependencies
apt-get update
apt-get install -y make autoconf automake libtool zlib1g-dev

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Export flags - append to existing flags
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

# Build zlib if not already available
if [ ! -f "$WORK/lib/libz.a" ]; then
    echo "Building zlib..."
    mkdir -p /tmp/zlib_build
    cd /tmp/zlib_build
    # Note: zlib source might be in $SRC/../zlib or we need to clone it
    # For now, we'll use system zlib since we installed zlib1g-dev
    # The headers and libraries should be in standard system paths
    cd /tmp/build_tmp
fi

# Configure libpng using CMake
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCMAKE_EXE_LINKER_FLAGS="$LDFLAGS" \
    -DCMAKE_SHARED_LINKER_FLAGS="$LDFLAGS" \
    -DPNG_SHARED=ON \
    -DPNG_STATIC=ON \
    -DPNG_TESTS=OFF \
    -DPNG_TOOLS=OFF \
    -DBUILD_TESTING=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the installation
echo "Checking installed libraries:"
ls -la "$WORK/lib/"
echo "Checking installed headers:"
ls -la "$WORK/include/"
