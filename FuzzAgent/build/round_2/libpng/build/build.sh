#!/bin/bash
set -e

# Create temporary build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Install required build tools
apt-get update
apt-get install -y make autoconf automake libtool

# Build and install zlib first
echo "Building zlib..."
ZLIB_SRC="$SRC/../zlib"
if [ ! -d "$ZLIB_SRC" ]; then
    echo "Zlib source not found, downloading..."
    git clone --depth 1 https://github.com/madler/zlib.git "$ZLIB_SRC"
fi

cd "$ZLIB_SRC"
./configure --prefix="$WORK/3rdparty" --static
make -j$(nproc)
make install

# Set up environment variables for libpng build
export CFLAGS="$CFLAGS -I$WORK/3rdparty/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/3rdparty/include"
export LDFLAGS="$LDFLAGS -L$WORK/3rdparty/lib"

# Return to build directory
cd /tmp/build_tmp

# Check if we should use CMake or Autotools
# Prefer CMake for better cross-platform support
if command -v cmake >/dev/null 2>&1; then
    echo "Building libpng with CMake..."
    
    # Create build directory
    mkdir -p build && cd build
    
    # Configure with CMake
    cmake \
        -DCMAKE_INSTALL_PREFIX="$WORK" \
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
        -DCMAKE_C_COMPILER="$CC" \
        -DCMAKE_CXX_COMPILER="$CXX" \
        -DCMAKE_C_FLAGS="$CFLAGS" \
        -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
        -DZLIB_LIBRARY="$WORK/3rdparty/lib/libz.a" \
        -DZLIB_INCLUDE_DIR="$WORK/3rdparty/include" \
        -DPNG_SHARED=OFF \
        -DPNG_STATIC=ON \
        -DPNG_TESTS=OFF \
        -DPNG_TOOLS=OFF \
        "$SRC"
    
    # Build and install
    make -j$(nproc)
    make install
else
    echo "CMake not found, using Autotools..."
    
    # Check if we need to run autogen.sh
    if [ ! -f "$SRC/configure" ]; then
        cd "$SRC"
        ./autogen.sh
        cd /tmp/build_tmp
    fi
    
    # Configure with Autotools
    "$SRC/configure" \
        --prefix="$WORK" \
        --disable-shared \
        --enable-static \
        --disable-dependency-tracking \
        --with-zlib-prefix="$WORK/3rdparty" \
        CC="$CC" \
        CXX="$CXX" \
        CFLAGS="$CFLAGS" \
        CXXFLAGS="$CXXFLAGS"
    
    # Build and install
    make -j$(nproc)
    make install
fi

# Verify the build
if [ -f "$WORK/lib/libpng.a" ] || [ -f "$WORK/lib/libpng16.a" ]; then
    echo "Build successful!"
    ls -la "$WORK/lib/"
else
    echo "Build failed: libpng static library not found"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
