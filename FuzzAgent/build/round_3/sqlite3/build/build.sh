#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Set ASAN options to prevent leak detection issues
export ASAN_OPTIONS=detect_leaks=0

# Append SQLite-specific flags to CFLAGS
# Limit max length of data blobs and sql queries to prevent irrelevant OOMs.
# Also limit max memory page count to avoid creating large databases.
export CFLAGS="$CFLAGS -DSQLITE_MAX_LENGTH=128000000 \
               -DSQLITE_MAX_SQL_LENGTH=128000000 \
               -DSQLITE_MAX_MEMORY=25000000 \
               -DSQLITE_PRINTF_PRECISION_LIMIT=1048576 \
               -DSQLITE_DEBUG=1 \
               -DSQLITE_MAX_PAGE_COUNT=16384"

# Configure SQLite
# Use --shared=0 for static library only, but we want both static and shared
# Actually, we need static library for fuzzing, but can build both
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-shared \
    --enable-static \
    --disable-readline \
    --disable-tcl

# Build
make -j$(nproc)

# Install headers and libraries
make install

# The standard SQLite make install may not install static library properly
# Ensure libsqlite3.a is in $WORK/lib
if [ -f .libs/libsqlite3.a ]; then
    cp .libs/libsqlite3.a "$WORK/lib/"
fi
if [ -f .libs/libsqlite3.so ]; then
    cp .libs/libsqlite3.so "$WORK/lib/"
fi

# Also copy sqlite3.h and sqlite3ext.h to include directory
if [ -f sqlite3.h ]; then
    cp sqlite3.h "$WORK/include/"
fi
if [ -f sqlite3ext.h ]; then
    cp sqlite3ext.h "$WORK/include/"
fi

# Verify installation
echo "Build completed. Checking installed artifacts:"
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true
