#!/bin/bash
set -e

# Install build dependencies
apt-get update && apt-get install -y autoconf automake libtool pkg-config

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Copy source to build directory
cp -r "$SRC" /tmp/build_tmp/source
cd /tmp/build_tmp/source

# Generate configure script if needed
if [ -f ./buildconf ]; then
    ./buildconf
elif [ -f ./autogen.sh ]; then
    ./autogen.sh
fi

# Configure with static library and install to $WORK
# Append system CFLAGS/CXXFLAGS to our flags
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

./configure \
    --prefix="$WORK" \
    --enable-debug \
    --disable-shared \
    --enable-static \
    --disable-tests \
    --disable-dependency-tracking

# Build and install
make -j$(nproc)
make install

# Ensure lib directory exists
mkdir -p "$WORK/lib"
mkdir -p "$WORK/include"

# Verify static library was created
if [ -f "$WORK/lib/libcares.a" ]; then
    echo "Success: Static library built at $WORK/lib/libcares.a"
else
    echo "Error: libcares.a not found in $WORK/lib"
    # Try to find and copy the library from build directory
    find . -name "libcares.a" -exec cp {} "$WORK/lib/" \; 2>/dev/null || true
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
