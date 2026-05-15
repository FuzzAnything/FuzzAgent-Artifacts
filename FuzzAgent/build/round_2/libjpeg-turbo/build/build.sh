#!/bin/bash
set -e # Fail immediately on error

# 1. Clean and Prep - build in temporary directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Handle special sanitizer flags - exactly like OSS-Fuzz
if [[ "$SANITIZER" == "memory" ]]; then
    export CFLAGS="$CFLAGS -DZERO_BUFFERS=1"
fi

# 3. Determine if we need to disable SIMD
# SIMD creates COMDAT sections that cause linker errors when creating shared libraries
# from static libraries with certain compiler flags (LTO, sanitizers)
# Disable SIMD for: AFL compiler (LTO), or any build with sanitizer flags
extra_cmake_flags=""
if [[ "$CC" == *afl* ]] || [[ "$CXX" == *afl* ]] || [[ "$CFLAGS" == *sanitize* ]]; then
    echo "Disabling SIMD to avoid LTO/sanitizer COMDAT issues"
    extra_cmake_flags="-DWITH_SIMD=OFF -DREQUIRE_SIMD=OFF"
else
    echo "SIMD enabled by default"
    # SIMD is ON by default, no need to set it
fi

# 4. Configure with CMake - EXACTLY like OSS-Fuzz fuzz/build.sh
# OSS-Fuzz builds ONLY static libraries: -DENABLE_STATIC=1 -DENABLE_SHARED=0
cmake "$SRC" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_FLAGS_RELWITHDEBINFO="-g -DNDEBUG" \
    -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-g -DNDEBUG" \
    -DENABLE_STATIC=ON \
    -DENABLE_SHARED=OFF \
    $extra_cmake_flags

# 5. Build & Install - same as OSS-Fuzz
make "-j$(nproc)" "--load-average=$(nproc)"
make install

# 6. Verify installation - check for static libraries only
if [ -f "$WORK/lib/libjpeg.a" ]; then
    echo "Static library installed: $WORK/lib/libjpeg.a"
else
    echo "ERROR: libjpeg.a not found!" >&2
    exit 1
fi

if [ -f "$WORK/lib/libturbojpeg.a" ]; then
    echo "TurboJPEG static library installed: $WORK/lib/libturbojpeg.a"
else
    echo "ERROR: libturbojpeg.a not found!" >&2
    exit 1
fi

echo "Build successful - static libraries installed to $WORK/lib/"

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
