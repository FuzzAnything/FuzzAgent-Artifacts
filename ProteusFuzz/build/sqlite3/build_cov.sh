#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/sqlite3/.build-work/build_cov
mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/sqlite3/.build-work/build_cov
cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/sqlite3/.build-work/build_cov

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
"$SRC/configure" --shared=0 --disable-tcl \
                 --prefix="$WORK"

# Build
make -j$(nproc)

# Install
make install

# Verify installation
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

# Don't delete the temporal build directory after building! It will be used for postprocessing.
