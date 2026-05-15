#!/bin/bash
set -e

# Create output directories
mkdir -p "$WORK/lib"
mkdir -p "$WORK/include"

# Clean and prepare build in source directory
cd "$SRC"
make clean 2>/dev/null || true

# Set up environment variables
export CC="$CC"
export CXX="$CXX"
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# Based on oss-fuzz openssl build.sh configuration flags
# Modified for static library build only (no fuzzers)
CONFIGURE_FLAGS="--debug -DPEDANTIC -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION no-shared"
CONFIGURE_FLAGS="$CONFIGURE_FLAGS enable-tls1_3 enable-rc5 enable-md2 enable-nextprotoneg enable-weak-ssl-ciphers"
CONFIGURE_FLAGS="$CONFIGURE_FLAGS enable-unit-test no-tests"

# Handle memory sanitizer (needs no-asm)
if [[ $CFLAGS = *sanitize=memory* ]]; then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-asm"
fi

# Handle 32-bit builds (needs no-threads)
if [[ $CFLAGS = *-m32* ]]; then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-threads"
else
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS enable-ec_nistp_64_gcc_128"
fi

echo "Configuring OpenSSL with flags: $CONFIGURE_FLAGS"

# Configure - do NOT use --prefix, build in-place
if [[ $CFLAGS = *-m32* ]]; then
    setarch i386 ./config $CONFIGURE_FLAGS
else
    ./config $CONFIGURE_FLAGS
fi

# Build only the libraries (skip building fuzzers and apps)
echo "Building libraries..."
make -j$(nproc) build_libs

# Search for and copy static libraries
echo "Searching for built static libraries..."

# Look for libraries in common locations
LIBRARIES_FOUND=0

# Check root directory
if [ -f "libcrypto.a" ]; then
    echo "Found libcrypto.a in build dir, copying to $WORK/lib/"
    cp libcrypto.a "$WORK/lib/"
    LIBRARIES_FOUND=1
fi

if [ -f "libssl.a" ]; then
    echo "Found libssl.a in build dir, copying to $WORK/lib/"
    cp libssl.a "$WORK/lib/"
    LIBRARIES_FOUND=1
fi

# Check .libs directory
if [ -f ".libs/libcrypto.a" ]; then
    echo "Found .libs/libcrypto.a, copying to $WORK/lib/"
    cp .libs/libcrypto.a "$WORK/lib/"
    LIBRARIES_FOUND=1
fi

if [ -f ".libs/libssl.a" ]; then
    echo "Found .libs/libssl.a, copying to $WORK/lib/"
    cp .libs/libssl.a "$WORK/lib/"
    LIBRARIES_FOUND=1
fi

# If not found in common locations, search more broadly
if [ $LIBRARIES_FOUND -eq 0 ]; then
    echo "Searching more broadly for .a files..."
    find . -name "*.a" -type f | while read lib; do
        echo "Found: $lib"
        cp "$lib" "$WORK/lib/"
        LIBRARIES_FOUND=1
    done
fi

# Copy headers
echo "Copying headers to $WORK/include/"
if [ -d "include/openssl" ]; then
    cp -r include/openssl "$WORK/include/"
fi

# Verify installation
echo "Checking installation in $WORK:"
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true

# Check for static libraries
if ls "$WORK/lib/"*.a 1> /dev/null 2>&1; then
    echo "Static libraries built successfully:"
    ls -la "$WORK/lib/"*.a
    echo "Build completed successfully!"
else
    echo "ERROR: Failed to build or find static libraries"
    echo "Searching for any .a files in build directory:"
    find . -name "*.a" -type f
    exit 1
fi
