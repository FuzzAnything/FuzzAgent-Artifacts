#!/bin/bash
set -e # Fail immediately on error

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure CMake with fuzzing instrumentation
# Use the environment variables for compiler and flags
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DENABLE_CJSON_TEST=OFF \
      -DENABLE_CJSON_UTILS=ON \
      -DENABLE_CUSTOM_COMPILER_FLAGS=OFF \
      -DENABLE_TARGET_EXPORT=OFF \
      -DBUILD_SHARED_AND_STATIC_LIBS=OFF \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# The build directory will be kept for potential postprocessing
# rm -rf /tmp/build_tmp
