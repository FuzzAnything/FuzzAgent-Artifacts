#!/bin/bash
set -e  # Fail immediately on error

# Ensure we have necessary build tools
apt-get update && apt-get install -y cmake ninja-build autoconf automake libtool pkg-config 2>/dev/null || true

# Clean and prepare temporary build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Configure using CMake (preferred)
# Build static library only, disable tests and tools for faster compilation
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCARES_STATIC=ON \
    -DCARES_SHARED=OFF \
    -DCARES_BUILD_TESTS=OFF \
    -DCARES_BUILD_TOOLS=OFF \
    -DCARES_BUILD_CONTAINER_TESTS=OFF \
    -DCARES_SYMBOL_HIDING=OFF \
    -DCARES_THREADS=ON \
    -DBUILD_TESTING=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify installation
echo "Checking installed artifacts in $WORK/lib:"
ls -la "$WORK/lib/" 2>/dev/null || echo "No lib directory found"
echo "Checking installed artifacts in $WORK/include:"
ls -la "$WORK/include/" 2>/dev/null || echo "No include directory found"

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
