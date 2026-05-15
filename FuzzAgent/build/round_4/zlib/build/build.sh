#!/bin/bash
set -e  # Fail immediately on error

# 1. Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Configure zlib using autotools
# Use the system's CC and CFLAGS, append our flags
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Run configure with proper prefix for installation
$SRC/configure --prefix="$WORK" --static --shared

# 3. Build & Install
make -j$(nproc)
make install

# 4. Verify the libraries are installed
echo "Checking installed libraries in $WORK:"
ls -la $WORK/lib/ || echo "lib directory not found"
ls -la $WORK/include/ || echo "include directory not found"
