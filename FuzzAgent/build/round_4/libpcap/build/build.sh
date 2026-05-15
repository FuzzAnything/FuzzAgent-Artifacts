#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir -p /tmp/build_tmp && cd /tmp/build_tmp

# Ensure we have flex and bison installed for scanner.l and grammar.y
apt-get update && apt-get install -y flex bison

# Create install directories
mkdir -p "$WORK/lib" "$WORK/include"

# Add -DSANITIZE cmake flag to avoid undefined symbol errors with ASAN
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   extra_cmake_flags="-DSANITIZE=address"
fi

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
    -DDISABLE_NETMAP=OFF \
    -DDISABLE_BLUETOOTH=OFF \
    -DDISABLE_DAG=OFF \
    -DDISABLE_SEPTEL=OFF \
    -DDISABLE_SNF=OFF \
    -DDISABLE_TC=OFF \
    -DDISABLE_DBUS=OFF \
    -DDISABLE_RDMA=OFF \
    $extra_cmake_flags \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the static library exists
if [ -f "$WORK/lib/libpcap.a" ]; then
    echo "Successfully built libpcap.a"
else
    echo "Error: libpcap.a not found in $WORK/lib"
    exit 1
fi
