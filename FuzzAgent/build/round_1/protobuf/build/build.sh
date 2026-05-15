#!/bin/bash
set -e

# Create temporary build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Check for ASAN to handle special flags
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   # Add any special flags needed for ASAN compatibility
   extra_cmake_flags=""
fi

# Configure with CMake
cmake \
  -DCMAKE_INSTALL_PREFIX="$WORK" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_C_COMPILER="$CC" \
  -DCMAKE_CXX_COMPILER="$CXX" \
  -DCMAKE_C_FLAGS="$CFLAGS" \
  -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
  -DBUILD_SHARED_LIBS=OFF \
  -Dprotobuf_BUILD_TESTS=OFF \
  -Dprotobuf_BUILD_EXAMPLES=OFF \
  -Dprotobuf_BUILD_CONFORMANCE=OFF \
  -Dprotobuf_BUILD_SHARED_LIBS=OFF \
  -Dprotobuf_BUILD_LIBPROTOC=ON \
  -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON \
  -Dprotobuf_BUILD_PROTOC_BINARIES=ON \
  -Dprotobuf_WITH_ZLIB=ON \
  -Dprotobuf_BUILD_LIBUPB=ON \
  -Dprotobuf_INSTALL=ON \
  $extra_cmake_flags \
  "$SRC"

# Build and install
make -j$(nproc)
make install

# Note: Don't delete /tmp/build_tmp as it will be used for postprocessing
