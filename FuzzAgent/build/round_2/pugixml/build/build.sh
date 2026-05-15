#!/bin/bash
set -e  # Fail immediately on error

# 1. Clean and prep build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Append -fno-sanitize=float-divide-by-zero to CXXFLAGS
# This is disabled by default in clang but oss-fuzz reenables it
# See: https://github.com/google/oss-fuzz/issues/10564
export CXXFLAGS="$CXXFLAGS -fno-sanitize=float-divide-by-zero"

# 3. Configure with CMake
# Use the system-provided CC, CXX, CFLAGS, CXXFLAGS
# Disable tests and examples to speed up compilation
cmake "$SRC" \
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
  -DPUGIXML_USE_POSTFIX=OFF \
  -DPUGIXML_USE_VERSIONED_LIBDIR=OFF

# 4. Build and install
make -j$(nproc)
make install

# 5. Verify the installation
echo "Verifying installation in $WORK:"
ls -la "$WORK/lib/" || echo "lib directory not found"
ls -la "$WORK/include/" || echo "include directory not found"

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
