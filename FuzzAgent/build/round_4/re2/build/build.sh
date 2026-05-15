#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Set up compiler flags - append to existing flags
export CXXFLAGS="$CXXFLAGS -O2"
export CFLAGS="$CFLAGS -O2"

# Build Abseil from source for fuzzing (as per oss-fuzz recommendation)
echo "Building Abseil from source for fuzzing..."
git clone --depth 1 https://github.com/abseil/abseil-cpp.git
cd abseil-cpp
mkdir build && cd build
cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      ..
make -j$(nproc)
make install  # Install to /usr/local
ldconfig
cd /tmp/build_tmp

# Ensure pkg-config can find Abseil
export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"
echo "PKG_CONFIG_PATH: $PKG_CONFIG_PATH"
echo "Testing pkg-config for Abseil:"
pkg-config --exists absl_base && echo "Found absl_base" || echo "ERROR: absl_base not found"
pkg-config --cflags absl_base | head -5

# Build RE2
cd "$SRC"

# Add additional warning suppression for sanitizer builds
EXTRA_CXXFLAGS=""
if [[ $CXXFLAGS = *sanitize* ]]; then
    EXTRA_CXXFLAGS="-Wno-error=conversion -Wno-error=sign-conversion -Wno-error=shorten-64-to-32 -Wno-error=unused-parameter -Wno-error=missing-field-initializers"
fi

# Build the static library with our compiler and flags
echo "Building RE2..."
make -j$(nproc) CXX="$CXX" CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS" obj/libre2.a

# Install common files (headers, pkg-config) to $WORK
echo "Installing headers and pkg-config to $WORK..."
make common-install prefix="$WORK"

# Create lib directory if it doesn't exist
mkdir -p "$WORK/lib"

# Copy the static library to $WORK/lib
cp obj/libre2.a "$WORK/lib/"

# Also copy Abseil libraries and headers to $WORK/3rdparty for completeness
mkdir -p "$WORK/3rdparty/lib" "$WORK/3rdparty/include"
cp -r /usr/local/lib/libabsl*.a "$WORK/3rdparty/lib/" 2>/dev/null || true
cp -r /usr/local/include/absl "$WORK/3rdparty/include/" 2>/dev/null || true

# Verify installation
if [ -f "$WORK/lib/libre2.a" ]; then
    echo "Build completed successfully"
    echo "Library installed to: $WORK/lib/libre2.a"
    
    # Check if headers are installed
    if [ -f "$WORK/include/re2/re2.h" ]; then
        echo "Headers installed to: $WORK/include/re2/"
    else
        echo "Warning: Headers may not be installed correctly"
        ls -la "$WORK/include/" 2>/dev/null || true
    fi
else
    echo "Error: Library not found at $WORK/lib/libre2.a"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
