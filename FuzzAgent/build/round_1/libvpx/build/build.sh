#!/bin/bash
set -e # Fail immediately on error

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir -p /tmp/build_tmp && cd /tmp/build_tmp

# Add extra flags for sanitizers
extra_c_flags=""
extra_configure_flags=()

# oss-fuzz has 2 GB total memory allocation limit. So, we limit per-allocation
# limit in libvpx to 1 GB to avoid OOM errors. A smaller per-allocation is
# needed for MemorySanitizer (see bug oss-fuzz:9497 and bug oss-fuzz:9499).
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

# Append our CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS $extra_c_flags"
export CXXFLAGS="$CXXFLAGS"

# Configure libvpx
echo "Configuring libvpx..."
LDFLAGS="$CXXFLAGS" LD="$CXX" "$SRC/configure" \
    --prefix="$WORK" \
    --enable-vp9-highbitdepth \
    --disable-unit-tests \
    --disable-examples \
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
echo "Building libvpx..."
make -j$(nproc) all

echo "Installing libvpx to $WORK..."
make install

# Verify installation
echo "Verifying installation..."
if [ -f "$WORK/lib/libvpx.a" ]; then
    echo "Success: $WORK/lib/libvpx.a exists"
else
    echo "Error: $WORK/lib/libvpx.a not found!"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
