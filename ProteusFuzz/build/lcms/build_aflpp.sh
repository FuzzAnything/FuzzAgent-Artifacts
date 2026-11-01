#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/lcms/.build-work/build_aflpp && mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/lcms/.build-work/build_aflpp && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/lcms/.build-work/build_aflpp

# Set up environment variables for autotools
export CC="$CC"
export CXX="$CXX"
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"
export CPPFLAGS="$CPPFLAGS"

# Configure with autotools - ensure only static libraries are built
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-shared=no \
    --enable-static=yes \
    --disable-debug \
    --disable-dependency-tracking \
    --without-threaded \
    --without-fastfloat \
    --disable-maintainer-mode

# Build and install
make -j$(nproc)
make install

# Verify the installation
if [ -f "$WORK/lib/liblcms2.a" ]; then
    echo "Successfully built liblcms2.a"
    echo "Library installed to: $WORK/lib"
    ls -la "$WORK/lib/"
else
    echo "Error: liblcms2.a not found in $WORK/lib"
    exit 1
fi
