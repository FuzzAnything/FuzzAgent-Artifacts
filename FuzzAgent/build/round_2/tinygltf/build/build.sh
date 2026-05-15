#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure CMake build
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

# Verify the library was created
if [ -f "$WORK/lib/libtinygltf.a" ]; then
    echo "Successfully built libtinygltf.a"
elif [ -f "$WORK/lib/libtinygltf.so" ]; then
    echo "Successfully built libtinygltf.so"
else
    echo "Error: Library not found in $WORK/lib"
    ls -la "$WORK/lib" || true
    exit 1
fi
