#!/bin/bash
set -e

# Install build dependencies (autotools)
DEBIAN_FRONTEND=noninteractive apt-get update && \
DEBIAN_FRONTEND=noninteractive apt-get install -y \
    autoconf \
    automake \
    libtool

# Clean any previous build artifacts in source directory
cd "$SRC"
make clean 2>/dev/null || true
make distclean 2>/dev/null || true

# Append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# Run configure to build both shared and static libraries
# Using --without-jpeg, --without-tiff, --without-zlib to prevent tool building
"$SRC/configure" \
    --enable-shared=yes \
    --enable-static=yes \
    --without-jpeg \
    --without-tiff \
    --without-zlib \
    CC="$CC" \
    CXX="$CXX"

# Build ONLY the library, not tools
# Using -C src to only build in src directory, target liblcms2.la
make -j$(nproc) -C src liblcms2.la

# Create installation directories
mkdir -p "$WORK/lib"
mkdir -p "$WORK/include"

# Copy the static library
cp src/.libs/liblcms2.a "$WORK/lib/"

# Copy the shared library (if built)
if [ -f src/.libs/liblcms2.so ]; then
    cp src/.libs/liblcms2.so "$WORK/lib/"
elif [ -f src/.libs/liblcms2.dylib ]; then
    cp src/.libs/liblcms2.dylib "$WORK/lib/"
fi

# Copy only header files (not build files)
mkdir -p "$WORK/include"
cp "$SRC/include/lcms2.h" "$WORK/include/" 2>/dev/null || true
cp "$SRC/include/lcms2_plugin.h" "$WORK/include/" 2>/dev/null || true

# Verify the installation
echo "Build completed. Checking installation..."
ls -la "$WORK/lib/"
ls -la "$WORK/include/"
