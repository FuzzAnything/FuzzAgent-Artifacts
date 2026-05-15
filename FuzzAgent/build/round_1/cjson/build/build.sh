#!/bin/bash
set -e # Fail immediately on error

# Clean and prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure with CMake
# Enable both shared and static libraries for fuzzing
# Disable tests and utils to speed up compilation
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_AND_STATIC_LIBS=ON \
      -DBUILD_SHARED_LIBS=OFF \
      -DENABLE_CJSON_TEST=OFF \
      -DENABLE_CJSON_UTILS=OFF \
      -DENABLE_CUSTOM_COMPILER_FLAGS=OFF \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
