#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Use configure (autotools) approach as it's standard for zlib
# Append to CFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

# Run configure with proper prefix
"$SRC/configure" --prefix="$WORK" --static

# Build and install
make -j$(nproc)
make install

# Verify installation
if [ -f "$WORK/lib/libz.a" ]; then
    echo "Successfully built libz.a at $WORK/lib/libz.a"
else
    echo "ERROR: libz.a not found in $WORK/lib"
    ls -la "$WORK/lib/" || true
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
