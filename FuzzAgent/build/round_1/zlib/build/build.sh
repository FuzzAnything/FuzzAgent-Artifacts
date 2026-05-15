#!/bin/bash
set -e

# Clean and prep
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Append fuzzing flags to CFLAGS
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Use CMake for better control over build options
# Enable both static and shared libraries
# Disable testing and minizip to focus on core zlib
echo "Configuring zlib with CMake..."
echo "Using CC: $CC, CXX: $CXX"
echo "CFLAGS: $CFLAGS"
echo "CXXFLAGS: $CXXFLAGS"

# Check if we have ASAN flags to avoid issues with -Wl,-z,defs
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    echo "Detected ASAN build, adding appropriate flags..."
    extra_cmake_flags="-DSANITIZE=address"
fi

cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DZLIB_BUILD_SHARED=ON \
    -DZLIB_BUILD_STATIC=ON \
    -DZLIB_BUILD_TESTING=OFF \
    -DZLIB_BUILD_MINIZIP=OFF \
    -DZLIB_INSTALL=ON \
    $extra_cmake_flags

# Build and install
echo "Building zlib..."
make -j$(nproc)
echo "Installing zlib to $WORK..."
make install

# Verify the libraries are present
echo "Build completed. Checking installed artifacts..."
echo "Contents of $WORK/lib/:"
ls -la "$WORK/lib/" || true
echo "Contents of $WORK/include/:"
ls -la "$WORK/include/" || true

# Verify static library exists (required)
if [ ! -f "$WORK/lib/libz.a" ]; then
    echo "ERROR: Static library libz.a not found!"
    exit 1
fi

echo "zlib build successful!"
