#!/bin/bash
set -e

# Clean and prepare temporary build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Ensure we have configure script
if [ ! -f "$SRC/configure" ]; then
    echo "Generating configure script..."
    cd "$SRC"
    autoreconf -i
    cd /tmp/build_tmp
fi

# Configure with static library support and installation prefix
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-static \
    --enable-fsect-man5 \
    --disable-shared \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS"

# Build and install
make -j$(nproc) V=1
make install

# Verify the installation
echo "Checking installed libraries..."
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true

# The temporal build directory should NOT be deleted after building
# as mentioned in the instructions.
