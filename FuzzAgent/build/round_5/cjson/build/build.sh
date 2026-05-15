#!/bin/bash
set -e

# Clean and prepare temporary build directory
rm -rf /tmp/build_cjson && mkdir /tmp/build_cjson && cd /tmp/build_cjson

# Disable custom compiler flags to avoid conflicts with fuzzer instrumentation
# The custom flags include -Werror which would break with fuzzer instrumentation
# Also enable both static and shared libraries for comprehensive testing
# Enable cJSON_Utils for more comprehensive testing
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DENABLE_CUSTOM_COMPILER_FLAGS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_SHARED_AND_STATIC_LIBS=ON \
    -DENABLE_CJSON_UTILS=ON \
    -DENABLE_CJSON_TEST=OFF \
    -DENABLE_TARGET_EXPORT=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the installation
echo "Build completed. Checking installation in $WORK..."
ls -la "$WORK/lib/" || echo "No lib directory found"
ls -la "$WORK/include/" || echo "No include directory found"
