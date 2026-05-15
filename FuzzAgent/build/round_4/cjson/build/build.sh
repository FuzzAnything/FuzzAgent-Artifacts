#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_cjson
mkdir /tmp/build_cjson
cd /tmp/build_cjson

# Configure cJSON with CMake
# Enable both static and shared libraries, disable tests for faster build
# Use environment variables for compiler and flags
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_AND_STATIC_LIBS=ON \
      -DENABLE_CJSON_TEST=OFF \
      -DENABLE_CJSON_UTILS=ON \
      -DENABLE_CUSTOM_COMPILER_FLAGS=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# The build directory will be kept for potential post-processing
echo "cJSON build completed successfully"
echo "Artifacts installed to: $WORK"
