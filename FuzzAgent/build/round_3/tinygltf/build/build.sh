#!/bin/bash
set -e

# Clean and create temporary build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Append flags to CFLAGS and CXXFLAGS
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Configure CMake for static library build
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

# Verify library was created
if [[ -f "$WORK/lib/libtinygltf.a" ]]; then
    echo "Build successful: $WORK/lib/libtinygltf.a created"
else
    echo "ERROR: Library not created at $WORK/lib/libtinygltf.a"
    exit 1
fi
