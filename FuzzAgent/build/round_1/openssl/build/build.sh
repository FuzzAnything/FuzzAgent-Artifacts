#!/bin/bash
set -e

# Install required dependencies
export DEBIAN_FRONTEND=noninteractive
apt-get update && apt-get install -y --no-install-recommends perl libtext-template-perl

# Change to source directory
cd "$SRC"

# Clean previous builds
make clean 2>/dev/null || true

# Set up configuration flags for building libraries with fuzzing instrumentation
# Based on OSS-Fuzz configuration but adapted for library building
CONFIGURE_FLAGS="--debug -DPEDANTIC -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION no-shared enable-tls1_3 enable-rc5 enable-md2 enable-nextprotoneg enable-weak-ssl-ciphers --prefix=$WORK --libdir=lib $CFLAGS -fno-sanitize=alignment"
if [[ $CFLAGS = *sanitize=memory* ]]
then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-asm"
fi
if [[ $CFLAGS != *-m32* ]]
then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS enable-ec_nistp_64_gcc_128"
fi
if [[ $CFLAGS = *-m32* ]]
then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-threads"
fi

# Add additional flags for building libraries (not fuzzers)
CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-apps no-docs"

# Configure OpenSSL
echo "Configuring OpenSSL with flags: $CONFIGURE_FLAGS"
if [[ $CFLAGS = *-m32* ]]
then
    setarch i386 ./config $CONFIGURE_FLAGS
else
    ./config $CONFIGURE_FLAGS
fi

# Build
echo "Building OpenSSL..."
make -j$(nproc)

# Check what was built
echo "Checking built libraries in source directory..."
ls -la *.a 2>/dev/null || echo "No .a files in root directory"
find . -name "*.a" -type f | head -20

# Install to $WORK
echo "Installing OpenSSL to $WORK..."
make install

# Check installation directory
echo "Checking installation in $WORK..."
find "$WORK" -type f 2>/dev/null | head -30

# Verify installation
echo "Checking installed libraries in $WORK/lib..."
ls -la "$WORK/lib/" 2>/dev/null || echo "lib directory not found"
ls -la "$WORK/include/" 2>/dev/null || echo "include directory not found"
