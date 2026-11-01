#!/bin/bash
set -e  # Fail immediately on error

# Clean and prepare build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/pugixml/.build-work/build_asan
mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/pugixml/.build-work/build_asan
cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/pugixml/.build-work/build_asan

# Append additional flags for pugixml
# Fix float-divide-by-zero false positives during XPath evaluation
# This is disabled by default in clang but oss-fuzz reenables it, so we need to disable it again
export CXXFLAGS="$CXXFLAGS -fno-sanitize=float-divide-by-zero"

# Configure pugixml with CMake
# Enable both shared and static libraries, install headers and libraries to $WORK
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=ON \
    -DPUGIXML_BUILD_SHARED_AND_STATIC_LIBS=ON \
    -DPUGIXML_BUILD_TESTS=OFF \
    -DPUGIXML_INSTALL=ON \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify installation
echo "Build completed. Checking installed artifacts..."
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/pugixml/.build-work/build_asan
