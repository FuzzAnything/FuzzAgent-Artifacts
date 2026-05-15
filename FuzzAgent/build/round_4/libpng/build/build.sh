#!/bin/bash
set -e

# Check and install required dependencies if not already installed
# Skip apt-get update to avoid repository issues
check_and_install() {
    local pkg=$1
    if ! dpkg -l | grep -q "^ii.*$pkg"; then
        echo "Installing $pkg..."
        apt-get install -y --no-update "$pkg" 2>/dev/null || echo "Warning: Failed to install $pkg, continuing anyway"
    fi
}

# Check for required packages
check_and_install "make"
check_and_install "autoconf"
check_and_install "automake"
check_and_install "libtool"
check_and_install "zlib1g-dev"

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure with CMake
# Enable both static and shared libraries, disable tests and tools for faster build
# Append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DPNG_SHARED=ON \
      -DPNG_STATIC=ON \
      -DPNG_TESTS=OFF \
      -DPNG_TOOLS=OFF \
      -DPNG_EXECUTABLES=OFF \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# Ensure lib directory exists
mkdir -p "$WORK/lib"

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
