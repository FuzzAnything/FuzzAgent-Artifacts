#!/bin/bash
set -e

# Install dependencies if needed
command -v nasm >/dev/null 2>&1 || { echo "NASM-compatible assembler is unavailable"; exit 1; }

# Create build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libaom/.build-work/build_aflpp && mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libaom/.build-work/build_aflpp && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libaom/.build-work/build_aflpp

# Handle sanitizer flags for ASAN
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DSANITIZE=address"
fi

# Memory limits for oss-fuzz (2GB total, 1GB per allocation)
extra_c_flags=""
if [[ $CFLAGS = *sanitize=memory* ]]; then
    extra_c_flags="-DAOM_MAX_ALLOCABLE_MEMORY=536870912"
else
    extra_c_flags="-DAOM_MAX_ALLOCABLE_MEMORY=1073741824"
fi
# Suppress integer overflow noise in transform functions
extra_c_flags+=" -DDO_RANGE_CHECK_CLAMP=1"

# MemorySanitizer requires all code instrumented, disable assembly
if [[ $CFLAGS = *sanitize=memory* ]]; then
    extra_cmake_flags+=" -DAOM_TARGET_CPU=generic"
fi

# Enable PIC for shared libraries, enable low bitdepth, enable encoder
# Disable examples, docs, tests, tools for faster compilation
# Enable size limits for fuzzing safety
cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS $extra_c_flags" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS $extra_c_flags" \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_STATIC_LIBS=ON \
    -DENABLE_EXAMPLES=OFF \
    -DENABLE_DOCS=OFF \
    -DENABLE_TESTS=OFF \
    -DENABLE_TOOLS=OFF \
    -DCONFIG_PIC=1 \
    -DCONFIG_LOWBITDEPTH=1 \
    -DCONFIG_AV1_ENCODER=1 \
    -DCONFIG_SIZE_LIMIT=1 \
    -DDECODE_HEIGHT_LIMIT=12288 \
    -DDECODE_WIDTH_LIMIT=12288 \
    $extra_cmake_flags

# Build and install
make -j$(nproc)
make install

# Verify artifacts exist
echo "Checking installed artifacts in $WORK"
ls -la "$WORK/lib" || echo "No lib directory created"
ls -la "$WORK/include" || echo "No include directory created"
