#!/bin/bash
set -e

# Clean and create temporary build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Use system zlib (already installed via apt-get)
export ZLIB_ROOT="/usr"

# Append flags for fuzzing and coverage instrumentation
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

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
    $extra_cmake_flags \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the build artifacts
if [ -f "$WORK/lib/libpng.a" ] || [ -f "$WORK/lib/libpng16.a" ]; then
    echo "libpng build successful!"
else
    echo "Error: libpng static library not found in $WORK/lib"
    exit 1
fi
