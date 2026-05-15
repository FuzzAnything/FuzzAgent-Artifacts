#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_liblouis && mkdir /tmp/build_liblouis && cd /tmp/build_liblouis

# Install dependencies if needed
apt-get update && apt-get install -y libyaml-dev autoconf automake libtool

# Export flags properly - append to existing CFLAGS/CXXFLAGS
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# First, run autogen.sh in the source directory to generate configure script
echo "Running autogen.sh in source directory..."
cd "$SRC"
./autogen.sh

# Change back to build directory
cd /tmp/build_liblouis

# Copy source to build directory or use it in place
# Actually, we can configure from source directory with build directory separate
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-static \
    --disable-shared \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS"

# Build
echo "Building..."
make -j$(nproc) V=1

# Install
echo "Installing to $WORK..."
make install

# Verify installation
echo "Verifying installation..."
ls -la "$WORK/lib" || true
ls -la "$WORK/include" || true

# Also ensure we have static library
if [ -f "$WORK/lib/liblouis.a" ]; then
    echo "Static library created successfully: $WORK/lib/liblouis.a"
elif [ -f "$WORK/lib/liblouis.la" ]; then
    echo "Found libtool library: $WORK/lib/liblouis.la"
    # Try to find the actual .a file
    find "$WORK" -name "*.a" -type f | head -5
fi
