#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpcap/.build-work/build_aflpp && mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpcap/.build-work/build_aflpp && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpcap/.build-work/build_aflpp

# Debug: show installed tools
echo "=== Debug: Checking tools ==="
which flex || echo "flex not found"
which bison || echo "bison not found"
which m4 || echo "m4 not found"
flex --version || echo "flex version failed"
bison --version || echo "bison version failed"
echo "=== End Debug ==="

# Set LEX and YACC environment variables for CMake
export LEX=flex
export YACC=bison
export M4=m4

# Append to CFLAGS/CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Check if we need to handle ASAN undefined symbol errors
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   # libpcap doesn't have a specific SANITIZE flag, but we might need to handle other flags
   # For now, just note that we may need adjustments
   echo "AddressSanitizer enabled"
fi

# Configure with CMake
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=ON \
    -DENABLE_REMOTE=OFF \
    -DBUILD_TESTING=OFF \
    -DBUILD_WITH_LIBNL=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Also build static library if not built by default
# Check if static library was created, if not build it separately
if [ ! -f "$WORK/lib/libpcap.a" ]; then
    echo "Static library not found, building static version..."
    cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpcap/.build-work/build_aflpp
    rm -rf CMakeCache.txt CMakeFiles
    cmake \
        -DCMAKE_INSTALL_PREFIX="$WORK" \
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
        -DCMAKE_C_COMPILER="$CC" \
        -DCMAKE_CXX_COMPILER="$CXX" \
        -DCMAKE_C_FLAGS="$CFLAGS" \
        -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
        -DBUILD_SHARED_LIBS=OFF \
        -DENABLE_REMOTE=OFF \
        -DBUILD_TESTING=OFF \
        -DBUILD_WITH_LIBNL=OFF \
        "$SRC"
    make -j$(nproc)
    cp libpcap.a "$WORK/lib/" || true
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libpcap/.build-work/build_aflpp
