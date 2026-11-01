#!/bin/bash
set -e

# 1. Clean and prep
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libtiff/.build-work/build_cov && mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libtiff/.build-work/build_cov && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libtiff/.build-work/build_cov

# Build tools are supplied by the experiment environment; no root package installation.

# 3. Try to build dependencies from source if available, otherwise install from apt
# We'll check for source directories and build them with the same instrumentation

# Check for zlib source
if [ -d "$SRC/../zlib" ] || [ -d "$SRC/zlib" ]; then
    echo "Building zlib from source..."
    ZLIB_SRC=$(find "$SRC/.." -maxdepth 1 -name "zlib" -type d | head -1)
    if [ -z "$ZLIB_SRC" ] && [ -d "$SRC/zlib" ]; then
        ZLIB_SRC="$SRC/zlib"
    fi
    
    if [ -n "$ZLIB_SRC" ] && [ -d "$ZLIB_SRC" ]; then
        mkdir -p zlib_build && cd zlib_build
        # Use the same compiler and flags
        CC="$CC" CFLAGS="$CFLAGS -fPIC" "$ZLIB_SRC/configure" --static --prefix="$WORK"
        make -j$(nproc)
        make install
        cd ..
    fi
else
    echo "Installing zlib development package..."
    apt-get install -y zlib1g-dev
fi

# Check for libjpeg-turbo source  
if [ -d "$SRC/../libjpeg-turbo" ] || [ -d "$SRC/libjpeg-turbo" ]; then
    echo "Building libjpeg-turbo from source..."
    JPEG_SRC=$(find "$SRC/.." -maxdepth 1 -name "libjpeg-turbo" -type d | head -1)
    if [ -z "$JPEG_SRC" ] && [ -d "$SRC/libjpeg-turbo" ]; then
        JPEG_SRC="$SRC/libjpeg-turbo"
    fi
    
    if [ -n "$JPEG_SRC" ] && [ -d "$JPEG_SRC" ]; then
        mkdir -p jpeg_build && cd jpeg_build
        cmake "$JPEG_SRC" \
            -DCMAKE_INSTALL_PREFIX="$WORK" \
            -DCMAKE_C_COMPILER="$CC" \
            -DCMAKE_CXX_COMPILER="$CXX" \
            -DCMAKE_C_FLAGS="$CFLAGS" \
            -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
            -DENABLE_STATIC=ON \
            -DENABLE_SHARED=OFF
        make -j$(nproc)
        make install
        cd ..
    fi
else
    echo "Installing libjpeg development package..."
    apt-get install -y libjpeg-dev
fi

# Optional codec development packages are handled by the feature flags below.

# Keep codec support aligned with headers available on this non-root build node.
TIFF_JBIG=OFF; if [ -f /usr/include/jbig.h ]; then TIFF_JBIG=ON; fi
TIFF_LZMA=OFF; if [ -f /usr/include/lzma.h ]; then TIFF_LZMA=ON; fi
TIFF_ZSTD=OFF; if [ -f /usr/include/zstd.h ]; then TIFF_ZSTD=ON; fi
TIFF_WEBP=OFF; if [ -f /usr/include/webp/decode.h ]; then TIFF_WEBP=ON; fi
TIFF_LIBDEFLATE=OFF; if [ -f /usr/include/libdeflate.h ]; then TIFF_LIBDEFLATE=ON; fi

# 4. Configure libtiff with CMake
# Append to CFLAGS/CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Handle ASAN linker flags issue
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   echo "ASAN detected, adding appropriate flags..."
   # libtiff might need special handling for ASAN
   extra_cmake_flags="-DCMAKE_EXE_LINKER_FLAGS=-Wl,--allow-shlib-undefined"
fi

cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -Dtiff-tools=OFF \
      -Dtiff-tests=OFF \
      -Dtiff-contrib=OFF \
      -Dtiff-docs=OFF \
      -Dtiff-static=ON \
      -Dzlib=ON \
      -Djpeg=ON \
      -Djbig=$TIFF_JBIG \
      -Dlzma=$TIFF_LZMA \
      -Dzstd=$TIFF_ZSTD \
      -Dwebp=$TIFF_WEBP \
      -Dlibdeflate=$TIFF_LIBDEFLATE \
      $extra_cmake_flags \
      "$SRC"

# 5. Build & Install
make -j$(nproc)
make install

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/libtiff/.build-work/build_cov
