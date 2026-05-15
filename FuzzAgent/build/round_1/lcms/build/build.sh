#!/bin/bash
set -e  # Fail immediately on error

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure the project with autotools
# Use --prefix to specify installation directory
# Use --enable-shared=no to build only static libraries
# Append to CFLAGS and CXXFLAGS rather than overwriting them
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

"$SRC/configure" \
    --prefix="$WORK" \
    --enable-shared=no \
    --enable-static=yes \
    CC="$CC" \
    CXX="$CXX"

# Build the library
make -j$(nproc) all

# Install the library to $WORK
make install

# Verify installation
if [ -f "$WORK/lib/liblcms2.a" ]; then
    echo "Successfully built liblcms2.a"
    ls -la "$WORK/lib/"
else
    echo "ERROR: liblcms2.a not found in $WORK/lib/"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
