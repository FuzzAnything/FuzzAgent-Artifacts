#!/bin/bash
set -e
set -u

# Install build dependencies
apt-get update && apt-get install -y nasm

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Handle memory sanitizer like OSS-Fuzz does
if [[ $CFLAGS = *sanitize=memory* ]]; then
   export CFLAGS="$CFLAGS -DZERO_BUFFERS=1"
   export CXXFLAGS="$CXXFLAGS -DZERO_BUFFERS=1"
fi

# For sanitizer builds with LTO compilers, disable SIMD completely
# to avoid section group issues when creating shared libraries
simd_enabled="ON"
if [[ $CFLAGS = *sanitize=address* ]] || [[ $CFLAGS = *sanitize=undefined* ]]; then
    if [[ "$CC" = *lto* ]]; then
        simd_enabled="OFF"
        echo "Disabling SIMD for sanitizer+LTO build to avoid section group issues"
    fi
fi

# Configure CMake - build both static and shared libraries
# This ensures proper shared library creation instead of system trying to create from static archives
cmake "$SRC" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DENABLE_STATIC=ON \
    -DENABLE_SHARED=ON \
    -DCMAKE_C_FLAGS_RELWITHDEBINFO="-g -DNDEBUG" \
    -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-g -DNDEBUG" \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DWITH_TURBOJPEG=ON \
    -DWITH_SIMD=$simd_enabled \
    -DWITH_TESTS=OFF \
    -DWITH_TOOLS=OFF \
    -DWITH_JNA=OFF \
    -DWITH_FUZZ=OFF

# Build and install
make -j$(nproc)
make install

# Ensure proper shared library naming for the system
cd "$WORK/lib"
if [ -f "libturbojpeg.so.0.5.0" ] && [ ! -f "libjpeg-turbo.so" ]; then
    cp "libturbojpeg.so.0.5.0" "libjpeg-turbo.so"
    echo "Created libjpeg-turbo.so from libturbojpeg.so.0.5.0"
fi

# Also create libjpeg.so symlink if needed
if [ -f "libjpeg.so.62.3.0" ] && [ ! -f "libjpeg.so" ]; then
    ln -sf "libjpeg.so.62.3.0" "libjpeg.so"
fi

# Create combined static library if needed (for compatibility)
if [ -f "libjpeg.a" ] && [ -f "libturbojpeg.a" ] && [ ! -f "libjpeg-turbo.a" ]; then
    mkdir -p /tmp/combine_tmp && cd /tmp/combine_tmp
    ar x "$WORK/lib/libjpeg.a"
    ar x "$WORK/lib/libturbojpeg.a"
    ar rcs "$WORK/lib/libjpeg-turbo.a" *.o
    cd "$WORK/lib"
    rm -rf /tmp/combine_tmp
    echo "Created combined libjpeg-turbo.a"
fi

echo "Build completed successfully"
echo "Static libraries: $(ls -la $WORK/lib/*.a 2>/dev/null | wc -l) found"
echo "Shared libraries: $(ls -la $WORK/lib/*.so* 2>/dev/null | wc -l) found"

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
