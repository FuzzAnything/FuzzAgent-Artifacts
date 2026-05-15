#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Copy source to build directory
echo "Copying source files..."
cp -r "$SRC"/. .

# Run autogen.sh to generate configure script
echo "Running autogen.sh..."
./autogen.sh

# Configure with appropriate flags for fuzzing
echo "Configuring liblouis..."
./configure \
    --prefix="$WORK" \
    --enable-static \
    --disable-shared \
    --disable-yaml \
    --disable-macros \
    --enable-fuzzer=no \
    --enable-coverage=no \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS"

# Build and install
echo "Building liblouis..."
make -j$(nproc) V=1

echo "Installing liblouis..."
make install

# Verify installation
echo "Verifying installation..."
if [ -f "$WORK/lib/liblouis.a" ]; then
    echo "Successfully built liblouis.a"
else
    echo "ERROR: liblouis.a not found in $WORK/lib"
    exit 1
fi

echo "Build completed successfully!"
