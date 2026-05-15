#!/bin/bash
set -e  # Fail immediately on error

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Ensure $WORK/lib and $WORK/include directories exist
mkdir -p "$WORK/lib" "$WORK/include"

# Configure with appropriate flags
# Note: We append to CFLAGS/CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

# Run configure with prefix set to $WORK for installation
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-shared=no \
    --enable-static=yes \
    --with-jpeg=no \
    --with-tiff=no \
    --without-zlib \
    --without-pthreads

# Build and install
make -j$(nproc)
make install

# The build directory will be kept for postprocessing
# rm -rf /tmp/build_tmp
