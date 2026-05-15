#!/bin/bash
set -e # Fail immediately on error

# 1. Install build dependencies
apt-get update && apt-get install -y \
    libjpeg-turbo8-dev \
    libjbig-dev \
    liblzma-dev \
    zlib1g-dev \
    cmake \
    build-essential \
    pkg-config

# 2. Copy system static libraries to $WORK/lib for harness linking
mkdir -p "$WORK/lib"
mkdir -p "$WORK/include"

# Copy libjpeg static library
if [ -f "/usr/lib/x86_64-linux-gnu/libjpeg.a" ]; then
    cp "/usr/lib/x86_64-linux-gnu/libjpeg.a" "$WORK/lib/"
fi

# Copy libjbig static libraries
if [ -f "/usr/lib/x86_64-linux-gnu/libjbig.a" ]; then
    cp "/usr/lib/x86_64-linux-gnu/libjbig.a" "$WORK/lib/"
fi
if [ -f "/usr/lib/x86_64-linux-gnu/libjbig85.a" ]; then
    cp "/usr/lib/x86_64-linux-gnu/libjbig85.a" "$WORK/lib/"
fi

# Copy libz static library
if [ -f "/usr/lib/x86_64-linux-gnu/libz.a" ]; then
    cp "/usr/lib/x86_64-linux-gnu/libz.a" "$WORK/lib/"
fi

# Copy liblzma static library
if [ -f "/usr/lib/x86_64-linux-gnu/liblzma.a" ]; then
    cp "/usr/lib/x86_64-linux-gnu/liblzma.a" "$WORK/lib/"
fi

# 3. Clean and Prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 4. Configure with CMake
# Note: We append to CFLAGS/CXXFLAGS, never overwrite
# Set CMAKE_PREFIX_PATH to help find dependencies in $WORK
export CMAKE_PREFIX_PATH="$WORK:$CMAKE_PREFIX_PATH"
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_PREFIX_PATH="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=OFF \
    -Djbig=ON \
    -Dzlib=ON \
    -Dlzma=ON \
    -Dwebp=OFF \
    -Dzstd=OFF \
    -Dtiff-tools=OFF \
    -Dtiff-tests=OFF \
    -Dtiff-contrib=OFF \
    -Dtiff-docs=OFF \
    -Dtiff-deprecated=OFF \
    "$SRC"

# 5. Build & Install
make -j$(nproc)
make install

# 6. Verify installation
if [ -f "$WORK/lib/libtiff.a" ]; then
    echo "Success: Static library built at $WORK/lib/libtiff.a"
else
    echo "Error: Static library not found at $WORK/lib/libtiff.a" >&2
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
