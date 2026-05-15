#!/bin/bash
set -e

# Install build dependencies
apt-get update && apt-get install -y flex bison

# Clean and prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure using CMake with fuzzing instrumentation flags
# Append to CFLAGS/CXXFLAGS, never overwrite
export EXTRA_CFLAGS="$CFLAGS"
export EXTRA_CXXFLAGS="$CXXFLAGS"

cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$EXTRA_CFLAGS" \
      -DCMAKE_CXX_FLAGS="$EXTRA_CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_TESTING=OFF \
      -DENABLE_REMOTE=ON \
      -DENABLE_PROTOCHAIN=ON \
      "$SRC"

# Build & Install
make -j$(nproc)
make install

# Verify the installation
if [ -f "$WORK/lib/libpcap.a" ]; then
    echo "Build successful: $WORK/lib/libpcap.a created"
    ls -la "$WORK/lib/"
else
    echo "ERROR: libpcap.a not found in $WORK/lib/"
    exit 1
fi
