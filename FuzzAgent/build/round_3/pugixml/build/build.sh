#!/bin/bash
set -e # Fail immediately on error

# 1. Clean and Prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Configure with CMake
# Use CMake to build static library
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_TESTING=OFF \
      -DPUGIXML_BUILD_TESTS=OFF \
      -DPUGIXML_INSTALL=ON \
      "$SRC"

# 3. Build & Install
make -j$(nproc)
make install

# 4. Verify the library was created
if [ ! -f "$WORK/lib/libpugixml.a" ]; then
    echo "ERROR: Static library not found at $WORK/lib/libpugixml.a"
    exit 1
fi

echo "Build successful: libpugixml.a installed to $WORK/lib"
