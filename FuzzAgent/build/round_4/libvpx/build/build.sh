#!/bin/bash
set -e

# Clean and prep build directory
rm -rf /tmp/build_libvpx
mkdir -p /tmp/build_libvpx
cd /tmp/build_libvpx

# Handle different sanitizers - MemorySanitizer requires special handling
if [[ $CFLAGS = *sanitize=memory* ]]; then
  extra_c_flags='-DVPX_MAX_ALLOCABLE_MEMORY=536870912'
  # MemorySanitizer requires that all program code is instrumented.
  # Disable all assembly code, leaving the optimizations implemented with
  # compiler intrinsics, to improve sanitizer performance and coverage.
  extra_configure_flags=(--disable-x86-asm)
else
  extra_c_flags='-DVPX_MAX_ALLOCABLE_MEMORY=1073741824'
  extra_configure_flags=()
fi

# Set up installation prefix
# Note: libvpx configure uses --prefix, not DESTDIR for make install
PREFIX="$WORK"

# Configure libvpx
# We need to pass our compiler and flags
# Note: libvpx configure script handles CC and CXX environment variables
export CC="$CC"
export CXX="$CXX"

# Append our CFLAGS and CXXFLAGS
export CFLAGS="$CFLAGS $extra_c_flags"
export CXXFLAGS="$CXXFLAGS"

# Run configure with installation prefix
LDFLAGS="$CXXFLAGS" LD="$CXX" "$SRC/configure" \
    --prefix="$PREFIX" \
    --enable-vp9-highbitdepth \
    --disable-unit-tests \
    --disable-examples \
    --disable-tools \
    --disable-docs \
    --size-limit=12288x12288 \
    --extra-cflags="$extra_c_flags" \
    --disable-webm-io \
    --enable-debug \
    --enable-vp8-encoder \
    --enable-vp9-encoder \
    --enable-static \
    --disable-shared \
    "${extra_configure_flags[@]}"

# Build and install
make -j$(nproc)
make install

# Verify installation
echo "Checking installation in $WORK"
if [ -f "$WORK/lib/libvpx.a" ]; then
    echo "Success: libvpx.a found in $WORK/lib"
else
    echo "Error: libvpx.a not found in $WORK/lib"
    exit 1
fi

# The build directory is kept for potential post-processing
# rm -rf /tmp/build_libvpx
