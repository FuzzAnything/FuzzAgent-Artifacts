#!/bin/bash
set -e # Fail immediately on error

# 1. Clean and Prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Configure with CMake
# Build as static library (not header-only)
# Disable examples and tests to speed up compilation
# Enable installation
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
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
      -DBUILD_SHARED_LIBS=OFF \
      "$SRC"

# 3. Build & Install
make -j$(nproc)
make install

# 4. Verify the library was created
if [ -f "$WORK/lib/libtinygltf.a" ]; then
    echo "Success: libtinygltf.a created at $WORK/lib/libtinygltf.a"
else
    echo "Error: libtinygltf.a not found in $WORK/lib"
    exit 1
fi
