#!/bin/bash
set -e

# Create temporary build directory
rm -rf /tmp/build_cares && mkdir /tmp/build_cares && cd /tmp/build_cares

# Append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Configure with CMake
# Enable both static and shared libraries, disable tests and tools for faster build
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCARES_STATIC=ON \
    -DCARES_SHARED=ON \
    -DCARES_BUILD_TESTS=OFF \
    -DCARES_BUILD_TOOLS=OFF \
    -DCARES_INSTALL=ON \
    -DBUILD_TESTING=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the libraries are installed
echo "Checking installed artifacts in $WORK:"
ls -la $WORK/lib/ || true
ls -la $WORK/include/ || true

# Don't delete the temporary build directory after building
# It will be used for postprocessing.
# rm -rf /tmp/build_cares
