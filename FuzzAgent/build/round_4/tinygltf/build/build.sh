#!/bin/bash
set -e

# Clean and prep build directory
rm -rf /tmp/build_tmp && mkdir -p /tmp/build_tmp && cd /tmp/build_tmp

# Configure with CMake
# Important: We must use the provided compilers and append to flags
# We need to disable header-only mode to build a library
# We also disable examples and tests to speed up build
# Enable install and vendor header installation
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DTINYGLTF_HEADER_ONLY=OFF \
    -DTINYGLTF_BUILD_LOADER_EXAMPLE=OFF \
    -DTINYGLTF_BUILD_GL_EXAMPLES=OFF \
    -DTINYGLTF_BUILD_VALIDATOR_EXAMPLE=OFF \
    -DTINYGLTF_BUILD_BUILDER_EXAMPLE=OFF \
    -DTINYGLTF_BUILD_TESTS=OFF \
    -DTINYGLTF_INSTALL=ON \
    -DTINYGLTF_INSTALL_VENDOR=ON \
    -DTINYGLTF_USE_CUSTOM_JSON=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Note: Do not delete the temporary build directory after building
# It will be used for postprocessing.
