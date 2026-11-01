#!/bin/bash
set -e

# Clean and prepare temporary build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libmagic/.build-work/build_cov
mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libmagic/.build-work/build_cov
cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libmagic/.build-work/build_cov

# Ensure we have configure script
if [ ! -f "$SRC/configure" ]; then
    echo "Generating configure script..."
    cd "$SRC"
    autoreconf -i
    cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libmagic/.build-work/build_cov
fi

# Configure with static library support and installation prefix
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-static \
    --enable-fsect-man5 \
    --disable-shared \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS"

# Build and install
make -j$(nproc) V=1
make install

# Verify the installation
echo "Checking installed libraries..."
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true

# The temporal build directory should NOT be deleted after building
# as mentioned in the instructions.
