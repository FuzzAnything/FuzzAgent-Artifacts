#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Generate configure script and build system
echo "Running autoreconf..."
autoreconf -ivf "$SRC"

# Configure with static library and installation to $WORK
echo "Configuring..."
CFLAGS="$CFLAGS" CXXFLAGS="$CXXFLAGS" \
"$SRC"/configure \
    --prefix="$WORK" \
    --enable-static \
    --disable-shared \
    --enable-fsect-man5 \
    --with-pic \
    CC="$CC" \
    CXX="$CXX"

# Build and install
echo "Building..."
make -j$(nproc) V=1

echo "Installing to $WORK..."
make install

# Verify installation
echo "Verifying installation..."
if [ -f "$WORK/lib/libmagic.a" ]; then
    echo "Success: libmagic.a installed to $WORK/lib"
else
    echo "Error: libmagic.a not found in $WORK/lib"
    ls -la "$WORK/lib/" || true
    exit 1
fi

# Also copy headers if not already installed
if [ ! -f "$WORK/include/magic.h" ]; then
    echo "Copying magic.h to $WORK/include..."
    mkdir -p "$WORK/include"
    cp -f "$SRC"/src/magic.h.in "$WORK/include/magic.h" 2>/dev/null || true
    cp -f /tmp/build_tmp/src/magic.h "$WORK/include/magic.h" 2>/dev/null || true
fi

echo "Build completed successfully!"
