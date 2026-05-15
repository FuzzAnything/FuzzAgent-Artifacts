#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Handle float-divide-by-zero false positives during XPath evaluation
# This is disabled by default in clang but oss-fuzz reenables it, so we need to disable it again
# See https://github.com/google/oss-fuzz/issues/10564
export CXXFLAGS="$CXXFLAGS -fno-sanitize=float-divide-by-zero"

# Configure with CMake
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_TESTING=OFF \
    -DPUGIXML_BUILD_TESTS=OFF \
    -DPUGIXML_INSTALL=ON \
    -DPUGIXML_WCHAR_MODE=OFF \
    -DPUGIXML_COMPACT=OFF \
    -DPUGIXML_NO_XPATH=OFF \
    -DPUGIXML_NO_STL=OFF \
    -DPUGIXML_NO_EXCEPTIONS=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Also build shared libraries for completeness
cd /tmp
rm -rf /tmp/build_tmp_shared && mkdir /tmp/build_tmp_shared && cd /tmp/build_tmp_shared

cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_TESTING=OFF \
    -DPUGIXML_BUILD_TESTS=OFF \
    -DPUGIXML_INSTALL=ON \
    -DPUGIXML_WCHAR_MODE=OFF \
    -DPUGIXML_COMPACT=OFF \
    -DPUGIXML_NO_XPATH=OFF \
    -DPUGIXML_NO_STL=OFF \
    -DPUGIXML_NO_EXCEPTIONS=OFF \
    "$SRC"

make -j$(nproc)
make install

# Don't delete the temporal build directories after building! They will be used for postprocessing.
# rm -rf /tmp/build_tmp /tmp/build_tmp_shared
