#!/bin/bash
set -e

# Clean and prepare - build in source directory
cd "$SRC"

# Clean previous build artifacts
make clean 2>/dev/null || true

# Set up OpenSSL configure flags - build static libraries only
CONFIGURE_FLAGS="--prefix=$WORK --openssldir=$WORK/ssl no-shared"

# Set compiler environment variables
export CC="$CC"
export CXX="$CXX"
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# Add extra flags based on sanitizer type
if [[ $CFLAGS = *sanitize=address* ]] || [[ $CFLAGS = *sanitize=memory* ]]; then
    CONFIGURE_FLAGS="$CONFIGURE_FLAGS -fno-sanitize=alignment"
fi

if [[ $CFLAGS = *sanitize=memory* ]]; then
    CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-asm"
fi

if [[ $CFLAGS != *-m32* ]]; then
    CONFIGURE_FLAGS="$CONFIGURE_FLAGS enable-ec_nistp_64_gcc_128"
fi

if [[ $CFLAGS = *-m32* ]]; then
    # Prevent error with 32-bit builds
    CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-threads"
fi

# Run configure
echo "Configuring OpenSSL with flags: $CONFIGURE_FLAGS"
if [[ $CFLAGS = *-m32* ]]; then
    setarch i386 ./config $CONFIGURE_FLAGS
else
    ./config $CONFIGURE_FLAGS
fi

# Build and install everything (including static libraries)
echo "Building OpenSSL..."
make -j$(nproc)

echo "Installing OpenSSL to $WORK..."
make install_sw

# OpenSSL installs to lib64, but we need lib directory
if [ -d "$WORK/lib64" ]; then
    echo "Creating lib directory from lib64..."
    mkdir -p "$WORK/lib"
    cp -a "$WORK/lib64/"*.a "$WORK/lib/" 2>/dev/null || true
    cp -a "$WORK/lib64/"*.so "$WORK/lib/" 2>/dev/null || true
fi

# Ensure lib directory exists with static libraries
echo "Build completed successfully!"
echo "Installed libraries in $WORK/lib:"
ls -la $WORK/lib/ 2>/dev/null || echo "No lib directory created"
echo "Installed headers in $WORK/include:"
ls -la $WORK/include/ 2>/dev/null || echo "No include directory created"
