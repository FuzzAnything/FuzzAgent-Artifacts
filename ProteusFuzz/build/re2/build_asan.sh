#!/bin/bash
set -e

# Based on OSS-Fuzz successful build script
CXXFLAGS="$CXXFLAGS -O2"

echo "=== Building RE2 with instrumentation ==="
echo "WORK: $WORK"
echo "SRC: $SRC"

# Clean previous builds
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/re2/.build-work/build_asan && mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/re2/.build-work/build_asan && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/re2/.build-work/build_asan

# First, build and install Abseil from source (dependency for RE2)
echo "Building Abseil from source..."
mkdir -p abseil-cpp
find "$ABSEIL_SOURCE" -mindepth 1 -maxdepth 1 ! -name .git ! -name build -exec cp -r {} abseil-cpp/ \;
cd abseil-cpp
mkdir build && cd build

# Build Abseil with CMake
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_TESTING=OFF \
      ..

make -j$(nproc)
make install
cd ../..

# Build RE2
echo "Building RE2..."
mkdir -p re2_build && cd re2_build

# Copy RE2 source to build directory
cp -r "$SRC"/* .

# Set up environment for RE2 build with Abseil from our build
export CXX="$CXX"
export CXXFLAGS="$CXXFLAGS -std=c++17 -pthread -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib -pthread"

# Build RE2 using Makefile
# The Makefile uses pkg-config to find Abseil, so we need to set PKG_CONFIG_PATH
export PKG_CONFIG_PATH="$WORK/lib/pkgconfig:$PKG_CONFIG_PATH"

# Build the library
make -j$(nproc) obj/libre2.a

# Install RE2 to WORK directory
# Create necessary directories
mkdir -p "$WORK/lib" "$WORK/include/re2" "$WORK/lib/pkgconfig"

# Install static library
install obj/libre2.a "$WORK/lib/libre2.a"

# Install headers
INSTALL_HFILES="re2.h regexp.h set.h stringpiece.h filtered_re2.h"
for header in $INSTALL_HFILES; do
    if [ -f "$header" ]; then
        install -m 644 "$header" "$WORK/include/re2/"
    fi
done

# Install pkg-config file if available
if [ -f "re2.pc.in" ]; then
    # Create and install pkg-config file
    sed -e "s#@CMAKE_INSTALL_FULL_INCLUDEDIR@#$WORK/include#" \
        -e "s#@CMAKE_INSTALL_FULL_LIBDIR@#$WORK/lib#" \
        -e "s#@REQUIRES@#absl_absl_check absl_absl_log absl_base absl_core_headers absl_fixed_array absl_flags absl_flat_hash_map absl_flat_hash_set absl_hash absl_inlined_vector absl_optional absl_span absl_str_format absl_strings absl_synchronization#" \
        -e "s#@SONAME@#11#" \
        re2.pc.in > "$WORK/lib/pkgconfig/re2.pc"
fi

echo "=== Verification ==="
if [ -f "$WORK/lib/libre2.a" ]; then
    echo "SUCCESS: libre2.a installed to $WORK/lib/"
    echo "Library size: $(stat -c%s "$WORK/lib/libre2.a") bytes"
    
    # Test that it's a valid static library
    if ar t "$WORK/lib/libre2.a" >/dev/null 2>&1; then
        echo "SUCCESS: libre2.a is a valid static library"
    else
        echo "WARNING: libre2.a may not be a valid static library"
    fi
else
    echo "ERROR: libre2.a not found!"
    exit 1
fi

echo "Build completed successfully!"
