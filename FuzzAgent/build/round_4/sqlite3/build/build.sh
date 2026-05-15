#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Set up environment variables for fuzzing
export ASAN_OPTIONS=detect_leaks=0

# Append SQLite-specific fuzzing flags to CFLAGS
# Limit max length of data blobs and sql queries to prevent irrelevant OOMs.
# Also limit max memory page count to avoid creating large databases.
export CFLAGS="$CFLAGS -DSQLITE_MAX_LENGTH=128000000 \
               -DSQLITE_MAX_SQL_LENGTH=128000000 \
               -DSQLITE_MAX_MEMORY=25000000 \
               -DSQLITE_PRINTF_PRECISION_LIMIT=1048576 \
               -DSQLITE_DEBUG=1 \
               -DSQLITE_MAX_PAGE_COUNT=16384"

# Configure SQLite with static library build
echo "Configuring SQLite..."
"$SRC/configure" --shared=0 \
                 --prefix="$WORK"

# Build
echo "Building SQLite..."
make -j$(nproc)

# Install
echo "Installing SQLite..."
make install

# Verify installation
echo "Verifying installation..."
if [ -f "$WORK/lib/libsqlite3.a" ]; then
    echo "Build successful: libsqlite3.a installed to $WORK/lib"
else
    echo "ERROR: libsqlite3.a not found in $WORK/lib"
    exit 1
fi

if [ -f "$WORK/include/sqlite3.h" ]; then
    echo "Build successful: sqlite3.h installed to $WORK/include"
else
    echo "ERROR: sqlite3.h not found in $WORK/include"
    exit 1
fi

echo "SQLite build completed successfully!"
