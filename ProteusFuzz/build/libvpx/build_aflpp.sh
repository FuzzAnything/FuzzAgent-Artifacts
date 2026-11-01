#!/bin/bash
set -e

# Build libvpx
build_dir=/mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libvpx/.build-work/build_aflpp
rm -rf ${build_dir}
mkdir -p ${build_dir}
cd ${build_dir}

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

# Append our flags to CFLAGS and CXXFLAGS
export CFLAGS="$CFLAGS ${extra_c_flags}"
export CXXFLAGS="$CXXFLAGS ${extra_c_flags}"

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
    "${extra_configure_flags[@]}"

make -j$(nproc) all
make install

# Ensure lib directory exists (should be created by make install)
mkdir -p $WORK/lib
mkdir -p $WORK/include
