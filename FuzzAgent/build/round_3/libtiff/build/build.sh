#!/bin/bash
set -e # Fail immediately on error

# Ensure WORK directory structure exists
mkdir -p "$WORK/lib"
mkdir -p "$WORK/include"
mkdir -p "$WORK/3rdparty"

# Install required dependencies from apt packages
echo "Installing dependencies..."
apt-get update
apt-get install -y \
    zlib1g-dev \
    libjpeg-turbo8-dev \
    libjbig-dev \
    liblzma-dev

# Build libtiff using autotools
echo "Building libtiff..."

# Clean and create build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Copy source to build directory (to avoid polluting source)
cp -r "$SRC"/* .

# Run autogen.sh to generate configure script if needed
if [ -f autogen.sh ]; then
    ./autogen.sh
fi

# Configure libtiff with static library build
# Append to CFLAGS and CXXFLAGS to preserve fuzzing instrumentation
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

./configure \
    --prefix="$WORK" \
    --enable-static \
    --disable-shared \
    --disable-tools \
    --disable-tests \
    --disable-contrib \
    --disable-docs \
    --disable-deprecated \
    --with-zlib \
    --with-jpeg \
    --with-jbig \
    --with-lzma

# Build and install
make -j$(nproc)
make install

# Verify the libraries were created
echo "Verifying build artifacts..."
ls -la "$WORK/lib/" || echo "No libraries found in $WORK/lib"
ls -la "$WORK/include/" || echo "No headers found in $WORK/include"

echo "Build completed successfully!"
