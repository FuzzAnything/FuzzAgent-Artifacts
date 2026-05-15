#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_libpcap && mkdir /tmp/build_libpcap && cd /tmp/build_libpcap

# Append any additional flags needed
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Configure with CMake
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=OFF \
    -DENABLE_REMOTE=OFF \
    -DBUILD_WITH_LIBNL=OFF \
    -DDISABLE_USB=OFF \
    -DDISABLE_BLUETOOTH=OFF \
    -DDISABLE_NETMAP=OFF \
    -DDISABLE_RDMA=OFF \
    -DDISABLE_DAG=OFF \
    -DDISABLE_SEPTEL=OFF \
    -DDISABLE_SNF=OFF \
    -DDISABLE_TC=OFF \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify installation
if [ -f "$WORK/lib/libpcap.a" ]; then
    echo "Successfully built libpcap static library"
    ls -la "$WORK/lib/"
else
    echo "ERROR: libpcap.a not found in $WORK/lib/"
    exit 1
fi
