#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Copy source to build directory
cp -r "$SRC"/* .
cp -r "$SRC"/.??* . 2>/dev/null || true

# Generate configure script if needed
if [ ! -f configure ]; then
    ./autogen.sh
fi

# Check if we need to pass coverage flag
extra_flags=""
if [[ "$CFLAGS" == *"coverage"* ]] || [[ "$CFLAGS" == *"profile"* ]]; then
    extra_flags="--with-coverage"
fi

# Configure with prefix set to WORK directory
# Pass compiler flags through environment variables
export CC="$CC"
export CXX="$CXX"
export CFLAGS="$CFLAGS"
export CXXFLAGS="$CXXFLAGS"
export LDFLAGS="$LDFLAGS"

./configure \
    --prefix="$WORK" \
    --enable-static \
    --disable-shared \
    --with-yaml=no \
    $extra_flags

# Build everything from top level
make -j$(nproc)

# Install
make install

# Verify installation
echo "Checking installation:"
find "$WORK" -type f -name "*.a" -o -name "*.so" 2>/dev/null

# Check for static library
if [ -f "$WORK/lib/liblouis.a" ]; then
    echo "SUCCESS: Static library installed at $WORK/lib/liblouis.a"
elif [ -f "$WORK/lib/liblouis.la" ]; then
    echo "SUCCESS: Libtool library installed, checking for .a file..."
    # Check if .a file exists in .libs directory
    if [ -f "liblouis/.libs/liblouis.a" ]; then
        cp liblouis/.libs/liblouis.a "$WORK/lib/"
        echo "Copied static library from build directory"
    fi
fi

# If still no library, try to find it
if [ ! -f "$WORK/lib/liblouis.a" ]; then
    echo "WARNING: liblouis.a not found in $WORK/lib, searching..."
    find . -name "liblouis.a" -type f 2>/dev/null | while read f; do
        echo "Found: $f"
        cp "$f" "$WORK/lib/" 2>/dev/null && echo "Copied to $WORK/lib/"
    done
fi

# Final check
if [ -f "$WORK/lib/liblouis.a" ]; then
    echo "Build successful: $WORK/lib/liblouis.a exists"
else
    echo "ERROR: Failed to install static library"
    exit 1
fi
