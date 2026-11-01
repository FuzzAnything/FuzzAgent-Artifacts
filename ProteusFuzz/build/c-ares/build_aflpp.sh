#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/c-ares/.build-work/build_aflpp && mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/c-ares/.build-work/build_aflpp && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/c-ares/.build-work/build_aflpp

# Build c-ares with autotools
# First run buildconf to generate configure script if needed
if [ -f "$SRC/buildconf" ] && [ ! -f "$SRC/configure" ]; then
    cd "$SRC"
    ./buildconf
    cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/c-ares/.build-work/build_aflpp
fi

# Configure with static library and install prefix
# Append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"

# Configure for static library build
"$SRC/configure" \
    --prefix="$WORK" \
    --disable-shared \
    --enable-static \
    --enable-debug \
    --disable-tests \
    --enable-symbol-hiding \
    --enable-threads \
    CC="$CC" \
    CXX="$CXX"

# Build and install
make -j$(nproc)
make install

# Verify the library was installed
if [ -f "$WORK/lib/libcares.a" ]; then
    echo "Successfully built and installed libcares.a to $WORK/lib"
else
    echo "ERROR: libcares.a not found in $WORK/lib"
    exit 1
fi
