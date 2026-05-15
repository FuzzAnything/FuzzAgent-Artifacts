#!/bin/bash
set -e

# Clean and create temporary build directory
rm -rf /tmp/build_tmp && mkdir -p /tmp/build_tmp && cd /tmp/build_tmp

# Handle sanitizer-specific flags
extra_c_flags=""
extra_configure_flags=()

# Set memory limit based on sanitizer type
if [[ $CFLAGS = *sanitize=memory* ]]; then
  # MemorySanitizer requires that all program code is instrumented.
  # Disable all assembly code, leaving the optimizations implemented with
  # compiler intrinsics, to improve sanitizer performance and coverage.
  extra_c_flags='-DVPX_MAX_ALLOCABLE_MEMORY=536870912'
  extra_configure_flags=(--disable-x86-asm)
else
  extra_c_flags='-DVPX_MAX_ALLOCABLE_MEMORY=1073741824'
  extra_configure_flags=()
fi

# Append our CFLAGS to the existing ones
export CFLAGS="$CFLAGS $extra_c_flags"
export CXXFLAGS="$CXXFLAGS $extra_c_flags"

# Configure libvpx
LDFLAGS="$CXXFLAGS" LD=$CXX $SRC/configure \
    --prefix="$WORK" \
    --enable-vp9-highbitdepth \
    --disable-unit-tests \
    --disable-examples \
    --size-limit=12288x12288 \
    --disable-webm-io \
    --enable-debug \
    --enable-vp8-encoder \
    --enable-vp9-encoder \
    --enable-shared \
    --enable-static \
    --enable-pic \
    --enable-runtime-cpu-detect \
    "${extra_configure_flags[@]}"

# Build and install
make -j$(nproc) all
make install

# Verify installation
echo "Build completed. Checking installation..."
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
