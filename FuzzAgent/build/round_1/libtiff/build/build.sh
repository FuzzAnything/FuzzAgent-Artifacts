#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Ensure WORK directories exist
mkdir -p "$WORK/lib" "$WORK/include" "$WORK/3rdparty"

# Install build dependencies without interactive prompts
export DEBIAN_FRONTEND=noninteractive
apt-get update && apt-get install -y \
    nasm \
    liblzma-dev \
    pkg-config \
    libjpeg-turbo8-dev \
    libjbig-dev \
    zlib1g-dev

# Handle ASAN compatibility flags for libtiff
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DSANITIZE=address"
fi

# Build libtiff with CMake
echo "Building libtiff..."
cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=ON \
    -Dtiff-tools=OFF \
    -Dtiff-tests=OFF \
    -Dtiff-contrib=OFF \
    -Dtiff-docs=OFF \
    -Dtiff-deprecated=OFF \
    $extra_cmake_flags

make -j$(nproc)
make install

# Also build static library by setting BUILD_SHARED_LIBS=OFF in a separate build
echo "Building static library..."
cd /tmp/build_tmp && mkdir -p static_build && cd static_build
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
# Copy static library to WORK/lib
find . -name "*.a" -exec cp {} "$WORK/lib/" \;

# Verify the build produced required artifacts
echo "Checking build artifacts..."
if [ -f "$WORK/lib/libtiff.a" ]; then
    echo "✓ Static library built: $WORK/lib/libtiff.a"
fi
if [ -f "$WORK/lib/libtiff.so" ] || [ -f "$WORK/lib/libtiff.dylib" ]; then
    echo "✓ Shared library built"
fi
if [ -f "$WORK/include/tiff.h" ]; then
    echo "✓ Headers installed"
fi

echo "Build completed successfully!"
