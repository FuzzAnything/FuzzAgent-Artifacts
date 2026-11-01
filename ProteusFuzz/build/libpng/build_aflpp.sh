#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpng/.build-work/build_aflpp
mkdir -p /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpng/.build-work/build_aflpp
cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpng/.build-work/build_aflpp

# Append flags for fuzzing and coverage instrumentation
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Set ZLIB_ROOT to ensure CMake finds the system zlib
export ZLIB_ROOT="/usr"

# Check if we're building with sanitizers
# Disable hardware optimizations for sanitizer builds to avoid potential issues
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]] || [[ $CXXFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DPNG_HARDWARE_OPTIMIZATIONS=OFF"
fi

# Configure libpng with CMake
# Enable both static and shared libraries, disable tests and tools for faster build
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DZLIB_LIBRARY="$ZLIB_LIBRARY" \
    -DZLIB_INCLUDE_DIR="$ZLIB_INCLUDE_DIR" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DPNG_SHARED=ON \
    -DPNG_STATIC=ON \
    -DPNG_TESTS=OFF \
    -DPNG_TOOLS=OFF \
    -DPNG_EXECUTABLES=OFF \
    -DBUILD_TESTING=OFF \
    $extra_cmake_flags \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify installation
echo "Build completed. Checking installation..."
ls -la "$WORK/lib/" || echo "No lib directory found"
ls -la "$WORK/include/" || echo "No include directory found"
