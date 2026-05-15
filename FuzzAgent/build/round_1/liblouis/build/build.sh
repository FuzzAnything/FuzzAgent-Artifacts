#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_liblouis
mkdir /tmp/build_liblouis
cd /tmp/build_liblouis

# First, ensure configure script exists in source directory
if [ ! -f "$SRC/configure" ]; then
    echo "Generating configure script..."
    cd "$SRC"
    ./autogen.sh
    cd /tmp/build_liblouis
fi

# Set up environment variables for autotools
export CC="$CC"
export CXX="$CXX"
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"
export LDFLAGS="$LDFLAGS"

# Configure with prefix set to $WORK
"$SRC/configure" \
    --prefix="$WORK" \
    --disable-shared \
    --enable-static \
    --with-yaml \
    --without-coverage \
    --without-fuzzer \
    --disable-dependency-tracking

# Build and install
make -j$(nproc)
make install

# Verify the installation
if [ -f "$WORK/lib/liblouis.a" ]; then
    echo "Successfully built liblouis static library"
else
    echo "ERROR: liblouis.a not found in $WORK/lib"
    exit 1
fi
