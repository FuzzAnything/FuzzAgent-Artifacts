#!/bin/bash
set -e  # Fail immediately on error

# Clean and prep - build in source directory as OpenSSL expects
cd "$SRC"

# Create target directories
mkdir -p "$WORK/lib" "$WORK/include" "$WORK/ssl"

# Set compiler environment variables
export CC="$CC"
export CXX="$CXX"
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# Configure OpenSSL for library building (not fuzzer executables)
# Based on oss-fuzz configuration but adapted for library building
CONFIGURE_FLAGS="linux-x86_64 --prefix=$WORK --openssldir=$WORK/ssl -DPEDANTIC no-shared enable-tls1_3 enable-rc5 enable-md2 enable-nextprotoneg enable-weak-ssl-ciphers"

# Add fuzzing flags but without building fuzzer executables
if [[ "$CFLAGS" == *-fsanitize* ]] || [[ "$CFLAGS" == *-fprofile-instr-generate* ]]; then
    CONFIGURE_FLAGS="$CONFIGURE_FLAGS -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION"
fi

# Add no-asm for memory sanitizer builds
if [[ $CFLAGS = *sanitize=memory* ]]; then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-asm"
fi

# Add ec_nistp_64_gcc_128 for 64-bit builds
if [[ $CFLAGS != *-m32* ]]; then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS enable-ec_nistp_64_gcc_128"
fi

# Disable threads for 32-bit builds to avoid libatomic issues
if [[ $CFLAGS = *-m32* ]]; then
  CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-threads"
fi

# Additional flags to disable unnecessary components
CONFIGURE_FLAGS="$CONFIGURE_FLAGS no-apps no-docs no-tests"

echo "Configuring OpenSSL with flags: $CONFIGURE_FLAGS"

# Run configuration - use ./config since we're in $SRC directory
if [[ $CFLAGS = *-m32* ]]; then
    setarch i386 ./config $CONFIGURE_FLAGS
else
    ./config $CONFIGURE_FLAGS
fi

# Build with parallel jobs
make -j$(nproc)

# Install libraries and headers to $WORK
make install

# OpenSSL installs to lib64 on 64-bit systems, ensure libraries are in $WORK/lib
if [ -d "$WORK/lib64" ]; then
    echo "Copying libraries from lib64 to lib..."
    mkdir -p "$WORK/lib"
    find "$WORK/lib64" -name "*.a" -exec cp {} "$WORK/lib/" \;
    find "$WORK/lib64" -name "*.so" -exec cp {} "$WORK/lib/" 2>/dev/null \;
fi

# Fallback: manually copy libraries if still not found
if [ ! -f "$WORK/lib/libcrypto.a" ] && [ -f libcrypto.a ]; then
    echo "Copying libcrypto.a to $WORK/lib/"
    cp libcrypto.a "$WORK/lib/"
fi
if [ ! -f "$WORK/lib/libssl.a" ] && [ -f libssl.a ]; then
    echo "Copying libssl.a to $WORK/lib/"
    cp libssl.a "$WORK/lib/"
fi

# Verify installation
echo "Build completed successfully. Artifacts installed to $WORK"
echo "Library directory contents:"
find $WORK/lib -type f -name "*.a" -o -name "*.so" 2>/dev/null || true
ls -la $WORK/lib/ 2>/dev/null || true
echo "Include directory contents:"
ls -la $WORK/include/ 2>/dev/null || true
