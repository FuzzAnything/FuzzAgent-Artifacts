#!/bin/bash
set -e  # Fail immediately on error

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Add custom cmake flags for ASAN if needed
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   extra_cmake_flags="-DSANITIZE=address"
fi

# Configure with CMake
# We need to build both static and shared libraries for fuzzing
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_TESTING=OFF \
      -Dprotobuf_BUILD_TESTS=OFF \
      -Dprotobuf_BUILD_EXAMPLES=OFF \
      -Dprotobuf_BUILD_CONFORMANCE=OFF \
      -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON \
      -Dprotobuf_BUILD_LIBUPB=ON \
      -Dprotobuf_BUILD_LIBPROTOC=ON \
      -Dprotobuf_BUILD_PROTOC_BINARIES=ON \
      -Dprotobuf_BUILD_SHARED_LIBS=OFF \
      -Dprotobuf_WITH_ZLIB=ON \
      $extra_cmake_flags \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# Also build shared libraries in a separate build
mkdir -p /tmp/build_tmp_shared && cd /tmp/build_tmp_shared
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=ON \
      -DBUILD_TESTING=OFF \
      -Dprotobuf_BUILD_TESTS=OFF \
      -Dprotobuf_BUILD_EXAMPLES=OFF \
      -Dprotobuf_BUILD_CONFORMANCE=OFF \
      -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON \
      -Dprotobuf_BUILD_LIBUPB=ON \
      -Dprotobuf_BUILD_LIBPROTOC=ON \
      -Dprotobuf_BUILD_PROTOC_BINARIES=ON \
      -Dprotobuf_BUILD_SHARED_LIBS=ON \
      -Dprotobuf_WITH_ZLIB=ON \
      $extra_cmake_flags \
      "$SRC"

make -j$(nproc)
make install

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp /tmp/build_tmp_shared
