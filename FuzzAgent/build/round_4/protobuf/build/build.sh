#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir -p /tmp/build_tmp
cd /tmp/build_tmp

# Install essential build tools (non-interactive)
DEBIAN_FRONTEND=noninteractive apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    cmake \
    git \
    ninja-build \
    pkg-config \
    zlib1g-dev
# Configure CMake build
# Note: We append to CFLAGS/CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Handle ASAN compatibility flags if needed
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DSANITIZE=address"
fi

# Configure the build
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DBUILD_SHARED_LIBS=OFF \
    -Dprotobuf_BUILD_TESTS=OFF \
    -Dprotobuf_BUILD_EXAMPLES=OFF \
    -Dprotobuf_BUILD_CONFORMANCE=OFF \
    -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON \
    -Dprotobuf_BUILD_PROTOC_BINARIES=ON \
    -Dprotobuf_BUILD_LIBPROTOC=ON \
    -Dprotobuf_BUILD_LIBUPB=ON \
    -Dprotobuf_WITH_ZLIB=ON \
    -Dprotobuf_INSTALL=ON \
    -Dprotobuf_MSVC_STATIC_RUNTIME=OFF \
    -Dprotobuf_MODULE_COMPATIBLE=OFF \
    -Dprotobuf_VERBOSE=OFF \
    -Dprotobuf_FORCE_FETCH_DEPENDENCIES=ON \
    -Dprotobuf_LOCAL_DEPENDENCIES_ONLY=OFF \
    -DABSL_ENABLE_INSTALL=ON \
    $extra_cmake_flags \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Ensure lib directory exists
mkdir -p "$WORK/lib"

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
