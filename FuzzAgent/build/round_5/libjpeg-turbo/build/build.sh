#!/bin/bash
set -e # Fail immediately on error

# 1. Install required dependencies (NASM for SIMD extensions)
apt-get update && apt-get install -y nasm

# 2. Clean and prep build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 3. Configure CMake for libjpeg-turbo
# Use environment variables: CC, CXX, CFLAGS, CXXFLAGS
# Append to CFLAGS/CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# Handle SANITIZER-specific flags
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DSANITIZE=address"
fi

# Configure libjpeg-turbo with static build for fuzzing
cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DENABLE_STATIC=ON \
    -DENABLE_SHARED=OFF \
    -DWITH_TURBOJPEG=ON \
    -DWITH_JPEG7=ON \
    -DWITH_JPEG8=ON \
    -DWITH_JAVA=OFF \
    -DBUILD_TESTING=OFF \
    -DBUILD_DOCS=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    $extra_cmake_flags

# 4. Build & Install
make -j$(nproc)
make install

# 5. Verify the installation
echo "Build completed. Checking installed artifacts..."
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
