#!/bin/bash
set -e # Fail immediately on error

# Clean and create build directory
build_dir=/tmp/build_libvpx
rm -rf ${build_dir}
mkdir -p ${build_dir}
pushd ${build_dir}

# Handle MemorySanitizer special flags
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

# Configure libvpx
# Note: We append to CFLAGS/CXXFLAGS, don't overwrite
export CFLAGS="$CFLAGS ${extra_c_flags}"
export CXXFLAGS="$CXXFLAGS"

# Run configure with appropriate options
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
    "${extra_configure_flags[@]}"

# Build and install
make -j$(nproc)
make install

popd

# Verify installation
echo "Checking installation in $WORK..."
ls -la $WORK/lib/ || echo "lib directory not found"
ls -la $WORK/include/ || echo "include directory not found"
