#!/bin/bash
set -e

# Clean and prepare temporary build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/zlib/.build-work/build_cov && mkdir -p /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/zlib/.build-work/build_cov && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/zlib/.build-work/build_cov

# Extract source to build directory
cp -r "$SRC"/* .

# Append WORK include directory to CFLAGS
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Configure zlib with appropriate options
# We want both static and shared libraries (default)
./configure --prefix="$WORK"

# Build
make -j$(nproc)

# Install (this will install to $WORK)
make install

# Verify the installation
echo "Build completed. Checking installation..."
ls -la "$WORK/lib" || echo "No lib directory created"
ls -la "$WORK/include" || echo "No include directory created"

# The temporary build directory should not be deleted for postprocessing
# rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/zlib/.build-work/build_cov
