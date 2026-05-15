#!/bin/bash
set -e

# Clean and prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Handle sanitizer flags for ASAN
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DSANITIZE=address"
fi

# Memory limits for sanitizers
if [[ $CFLAGS = *sanitize=memory* ]]; then
    extra_c_flags='-DAOM_MAX_ALLOCABLE_MEMORY=536870912'
    extra_cmake_flags+=" -DAOM_TARGET_CPU=generic"
else
    extra_c_flags='-DAOM_MAX_ALLOCABLE_MEMORY=1073741824'
fi

# Enable DO_RANGE_CHECK_CLAMP to suppress integer overflow noise
extra_c_flags+=' -DDO_RANGE_CHECK_CLAMP=1'

# Configure with CMake
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS $extra_c_flags" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS $extra_c_flags" \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_TESTING=OFF \
    -DENABLE_EXAMPLES=OFF \
    -DENABLE_DOCS=OFF \
    -DENABLE_TESTS=OFF \
    -DENABLE_TOOLS=OFF \
    -DCONFIG_PIC=ON \
    -DCONFIG_LOWBITDEPTH=ON \
    -DCONFIG_AV1_ENCODER=ON \
    -DCONFIG_AV1_DECODER=ON \
    -DCONFIG_SIZE_LIMIT=ON \
    -DDECODE_HEIGHT_LIMIT=12288 \
    -DDECODE_WIDTH_LIMIT=12288 \
    $extra_cmake_flags \
    "$SRC"

# Build & Install
make -j$(nproc)
make install

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
