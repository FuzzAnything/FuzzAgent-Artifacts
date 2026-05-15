#!/bin/bash
set -e

# Skip package installation to avoid network issues
# Dependencies are optional and may already be installed

# Clean source directory if previously configured
cd "$SRC"
if [ -f Makefile ]; then
    make distclean || true
fi

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Configure with appropriate flags
# Append to CFLAGS and CXXFLAGS, don't overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

# Configure the project
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-shared=no \
    --enable-static=yes \
    --disable-dependency-tracking \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS"

# Build and install
bear -- make -j$(nproc)
make install

# Verify the library was created
if [ -f "$WORK/lib/liblcms2.a" ]; then
    echo "Build successful: $WORK/lib/liblcms2.a created"
else
    echo "ERROR: liblcms2.a not found in $WORK/lib"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
