#!/bin/bash
set -e  # Fail immediately on error

# Ensure we have required dependencies
if ! command -v nasm &> /dev/null; then
    echo "Installing NASM..."
    apt-get update && apt-get install -y nasm
fi

# Create build directory
BUILD_DIR=/tmp/build_libjpeg_turbo
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Handle sanitizer-specific flags
extra_cmake_flags=""
if [[ "$CFLAGS" =~ sanitize=address ]]; then
    # libjpeg-turbo might have specific sanitizer handling
    extra_cmake_flags="$extra_cmake_flags"
fi

# MemorySanitizer needs ZERO_BUFFERS
if [[ "$SANITIZER" == "memory" ]]; then
    export CFLAGS="$CFLAGS -DZERO_BUFFERS=1"
fi

# Configure with CMake
cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_STATIC=ON \
    -DENABLE_SHARED=OFF \
    -DWITH_TURBOJPEG=ON \
    -DWITH_SIMD=ON \
    -DWITH_ARITH_DEC=ON \
    -DWITH_ARITH_ENC=ON \
    -DWITH_JPEG7=ON \
    -DWITH_JPEG8=ON \
    -DWITH_TOOLS=OFF \
    -DWITH_TESTS=OFF \
    -DWITH_JNA=OFF \
    -DWITH_FUZZ=OFF \
    -DWITH_PROFILE=OFF \
    $extra_cmake_flags

# Build and install
make -j$(nproc)
make install

# Verify the installation
if [ -f "$WORK/lib/libturbojpeg.a" ] || [ -f "$WORK/lib/libjpeg.a" ]; then
    echo "Build successful! Libraries installed to $WORK/lib"
    ls -la "$WORK/lib/" || true
else
    echo "ERROR: Libraries not found in $WORK/lib"
    ls -la "$WORK/" || true
    exit 1
fi

# Don't delete build directory - may be used for postprocessing
# rm -rf "$BUILD_DIR"
