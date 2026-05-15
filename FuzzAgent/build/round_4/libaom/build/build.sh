#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Add -DSANITIZE cmake flag to avoid the undefined symbol error.
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   extra_cmake_flags="-DSANITIZE=address"
fi

# oss-fuzz has 2 GB total memory allocation limit. So, we limit per-allocation
# limit in libaom to 1 GB to avoid OOM errors. A smaller per-allocation is
# needed for MemorySanitizer (see bug oss-fuzz:9497 and bug oss-fuzz:9499).
if [[ $CFLAGS = *sanitize=memory* ]]; then
  extra_c_flags='-DAOM_MAX_ALLOCABLE_MEMORY=536870912'
else
  extra_c_flags='-DAOM_MAX_ALLOCABLE_MEMORY=1073741824'
fi
# Also, enable DO_RANGE_CHECK_CLAMP to suppress the noise of integer overflows
# in the transform functions.
extra_c_flags+=' -DDO_RANGE_CHECK_CLAMP=1'

# MemorySanitizer requires that all program code is instrumented. Therefore we
# need to replace all inline assembly code that writes to memory with pure C
# code. Disable all assembly code for MemorySanitizer.
if [[ $CFLAGS = *sanitize=memory* ]]; then
  extra_cmake_flags+=" -DAOM_TARGET_CPU=generic"
fi

# Configure with CMake
cmake "$SRC" \
  -DCMAKE_INSTALL_PREFIX="$WORK" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_C_COMPILER="$CC" \
  -DCMAKE_CXX_COMPILER="$CXX" \
  -DCMAKE_C_FLAGS="$CFLAGS $extra_c_flags" \
  -DCMAKE_CXX_FLAGS="$CXXFLAGS $extra_c_flags" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCONFIG_PIC=1 \
  -DCONFIG_LOWBITDEPTH=1 \
  -DCONFIG_AV1_ENCODER=1 \
  -DCONFIG_AV1_DECODER=1 \
  -DENABLE_EXAMPLES=1 \
  -DENABLE_DOCS=0 \
  -DENABLE_TESTS=0 \
  -DENABLE_TOOLS=1 \
  -DCONFIG_SIZE_LIMIT=1 \
  -DDECODE_HEIGHT_LIMIT=12288 \
  -DDECODE_WIDTH_LIMIT=12288 \
  -DBUILD_SHARED_LIBS=ON \
  ${extra_cmake_flags}

# Build and install
make -j$(nproc)
make install

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
