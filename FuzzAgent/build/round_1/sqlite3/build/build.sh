#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Export ASAN options to detect_leaks=0 (common for fuzzing)
export ASAN_OPTIONS=detect_leaks=0

# Append SQLite fuzzing limits to CFLAGS
# Limit max length of data blobs and sql queries to prevent irrelevant OOMs.
# Also limit max memory page count to avoid creating large databases.
export CFLAGS="$CFLAGS -DSQLITE_MAX_LENGTH=128000000 \
               -DSQLITE_MAX_SQL_LENGTH=128000000 \
               -DSQLITE_MAX_MEMORY=25000000 \
               -DSQLITE_PRINTF_PRECISION_LIMIT=1048576 \
               -DSQLITE_DEBUG=1 \
               -DSQLITE_MAX_PAGE_COUNT=16384"

# Run configure with static library and install prefix
"$SRC"/configure --prefix="$WORK" --shared=0

# Build and install
make -j$(nproc)
make install

# Verify the library was created
if [ -f "$WORK/lib/libsqlite3.a" ]; then
    echo "Build successful: $WORK/lib/libsqlite3.a created"
else
    echo "ERROR: libsqlite3.a not found in $WORK/lib"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
