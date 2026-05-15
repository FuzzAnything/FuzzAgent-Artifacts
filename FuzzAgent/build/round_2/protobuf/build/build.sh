#!/bin/bash
set -e  # Fail immediately on error

# 1. Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Configure with CMake
# We need to handle ASAN flags to avoid undefined symbol errors
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    # Add any ASAN-specific flags needed for protobuf
    extra_cmake_flags="-Dprotobuf_ASAN=ON"
fi

# 3. Set up environment variables for compilation
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# 4. Run CMake configuration
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -Dprotobuf_BUILD_TESTS=OFF \
      -Dprotobuf_BUILD_EXAMPLES=OFF \
      -Dprotobuf_BUILD_CONFORMANCE=OFF \
      -Dprotobuf_BUILD_SHARED_LIBS=OFF \
      -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON \
      -Dprotobuf_BUILD_PROTOC_BINARIES=ON \
      -Dprotobuf_BUILD_LIBPROTOC=ON \
      -Dprotobuf_BUILD_LIBUPB=ON \
      -Dprotobuf_WITH_ZLIB=ON \
      -Dprotobuf_VERBOSE=OFF \
      $extra_cmake_flags \
      "$SRC"

# 5. Build and install
make -j$(nproc)
make install

# 6. Also build shared libraries separately
cd /tmp/build_tmp
rm -rf CMakeCache.txt CMakeFiles
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -Dprotobuf_BUILD_TESTS=OFF \
      -Dprotobuf_BUILD_EXAMPLES=OFF \
      -Dprotobuf_BUILD_CONFORMANCE=OFF \
      -Dprotobuf_BUILD_SHARED_LIBS=ON \
      -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON \
      -Dprotobuf_BUILD_PROTOC_BINARIES=ON \
      -Dprotobuf_BUILD_LIBPROTOC=ON \
      -Dprotobuf_BUILD_LIBUPB=ON \
      -Dprotobuf_WITH_ZLIB=ON \
      -Dprotobuf_VERBOSE=OFF \
      $extra_cmake_flags \
      "$SRC"

make -j$(nproc)
make install

# 7. Verify the installation
echo "Checking installed libraries in $WORK/lib:"
ls -la "$WORK/lib/" || true
echo "Checking installed headers in $WORK/include:"
ls -la "$WORK/include/" || true
