#!/bin/bash
set -e # Fail immediately on error

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Enable building both shared and static libraries
# PUGIXML_BUILD_SHARED_AND_STATIC_LIBS builds both when BUILD_SHARED_LIBS is ON
export BUILD_SHARED_LIBS=ON

# Configure CMake with appropriate flags
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=ON \
      -DPUGIXML_BUILD_SHARED_AND_STATIC_LIBS=ON \
      -DPUGIXML_BUILD_TESTS=OFF \
      -DPUGIXML_INSTALL=ON \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify that libraries were created
if [ -f "$WORK/lib/libpugixml.a" ] && [ -f "$WORK/lib/libpugixml.so" ]; then
    echo "Successfully built both static and shared libraries"
    echo "Static library: $WORK/lib/libpugixml.a"
    echo "Shared library: $WORK/lib/libpugixml.so"
else
    echo "Warning: Not all libraries were created"
    ls -la "$WORK/lib/" || true
fi
