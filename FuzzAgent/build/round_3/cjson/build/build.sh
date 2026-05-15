#!/bin/bash
set -e

# Create build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Check for ASAN in CFLAGS to handle -Wl,-z,defs issue
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DENABLE_SANITIZERS=ON"
fi

# Configure with CMake
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DENABLE_CJSON_TEST=OFF \
      -DENABLE_CJSON_UTILS=ON \
      -DENABLE_CUSTOM_COMPILER_FLAGS=OFF \
      -DENABLE_CJSON_VERSION_SO=ON \
      -DENABLE_PUBLIC_SYMBOLS=ON \
      -DENABLE_CJSON_UNINSTALL=OFF \
      -DENABLE_LOCALES=ON \
      $extra_cmake_flags \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# Also build shared libraries
cd /tmp
rm -rf /tmp/build_tmp_shared && mkdir /tmp/build_tmp_shared && cd /tmp/build_tmp_shared

cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DBUILD_SHARED_LIBS=ON \
      -DBUILD_SHARED_AND_STATIC_LIBS=ON \
      -DENABLE_CJSON_TEST=OFF \
      -DENABLE_CJSON_UTILS=ON \
      -DENABLE_CUSTOM_COMPILER_FLAGS=OFF \
      -DENABLE_CJSON_VERSION_SO=ON \
      -DENABLE_PUBLIC_SYMBOLS=ON \
      -DENABLE_CJSON_UNINSTALL=OFF \
      -DENABLE_LOCALES=ON \
      $extra_cmake_flags \
      "$SRC"

make -j$(nproc)
make install

# Verify the libraries are installed
echo "Build completed. Checking installed artifacts..."
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true
