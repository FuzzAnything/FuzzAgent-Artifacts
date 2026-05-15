#!/bin/bash
set -e

# 1. Clean and prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Install basic build dependencies
apt-get update && apt-get install -y \
    cmake \
    nasm \
    pkg-config \
    build-essential

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

# Install other development packages for dependencies
apt-get install -y \
    libjbig-dev \
    liblzma-dev \
    libzstd-dev \
    libwebp-dev \
    libdeflate-dev

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
      -Djbig=ON \
      -Dlzma=ON \
      -Dzstd=ON \
      -Dwebp=ON \
      -Dlibdeflate=ON \
      $extra_cmake_flags \
      "$SRC"

# 5. Build & Install
make -j$(nproc)
make install

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
