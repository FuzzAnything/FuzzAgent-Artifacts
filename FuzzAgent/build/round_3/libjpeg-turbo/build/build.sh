#!/bin/bash
set -e
set -u

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Handle sanitizer-specific flags
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    # Add -DSANITIZE=address if needed for ASAN compatibility
    extra_cmake_flags="-DSANITIZE=address"
fi

# Append to CFLAGS and CXXFLAGS for fuzzing instrumentation
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# Configure libjpeg-turbo with CMake
# Based on the OSS-Fuzz build script and project requirements
cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DENABLE_STATIC=1 \
    -DENABLE_SHARED=0 \
    -DENABLE_SHARED=OFF \
    -DWITH_TURBOJPEG=1 \
    -DWITH_JPEG8=1 \
    -DWITH_12BIT=1 \
    -DWITH_16BIT=1 \
    -DWITH_SIMD=1 \
    -DWITH_JAVA=OFF \
    -DWITH_MEM_SRCDST=1 \
    -DWITH_FUZZ=0 \
    -DBUILD_TESTING=OFF \
    -DBUILD_DOC=OFF \
    $extra_cmake_flags

# Build and install
make -j$(nproc)
make install

# Verify installation
echo "Build completed successfully"
echo "Installed libraries in $WORK/lib:"
ls -la "$WORK/lib" || echo "No lib directory found"
echo "Installed headers in $WORK/include:"
ls -la "$WORK/include" 2>/dev/null || echo "No include directory found"
