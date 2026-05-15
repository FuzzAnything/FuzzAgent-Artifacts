#!/bin/bash
set -e

# Ensure WORK directory exists
mkdir -p "$WORK/lib"
mkdir -p "$WORK/include"

# Clean previous build if exists
rm -rf /tmp/build_zlib
mkdir -p /tmp/build_zlib
cd /tmp/build_zlib

# Append fuzzing flags to CFLAGS (preserve system flags)
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Check if we have ASAN flags to avoid issues with -Wl,-z,defs
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    echo "Detected ASAN build, adding appropriate flags..."
    extra_cmake_flags="-DSANITIZE=address"
fi

echo "Configuring zlib with CMake..."
echo "Using CC: $CC, CXX: $CXX"
echo "CFLAGS: $CFLAGS"
echo "CXXFLAGS: $CXXFLAGS"

# Use CMake for better control over build options
# Enable both static and shared libraries
# Disable testing and minizip to focus on core zlib
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

echo "Installing to $WORK..."
make install

# Verify installation
echo "Verifying installation..."
if [ -f "$WORK/lib/libz.a" ]; then
    echo "✓ Static library libz.a installed successfully"
else
    echo "✗ Static library libz.a not found!"
    exit 1
fi

# Check for shared library (could be libz.so or libz.so.1)
if ls "$WORK/lib"/libz.so* 1> /dev/null 2>&1; then
    echo "✓ Shared library installed successfully"
else
    echo "✗ Shared library not found!"
    exit 1
fi

if [ -f "$WORK/include/zlib.h" ]; then
    echo "✓ Header zlib.h installed successfully"
else
    echo "✗ Header zlib.h not found!"
    exit 1
fi

echo "Build completed successfully!"
echo "Libraries installed in: $WORK/lib"
echo "Headers installed in: $WORK/include"
