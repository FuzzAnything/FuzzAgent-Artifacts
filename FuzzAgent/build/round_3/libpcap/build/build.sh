#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir /tmp/build_tmp
cd /tmp/build_tmp

# Set LEX and YACC environment variables for CMake to find
export LEX=/usr/bin/flex
export YACC=/usr/bin/bison

# Determine if we're building with sanitizers
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    extra_cmake_flags="-DENABLE_SANITIZERS=address"
elif [[ $CFLAGS = *sanitize=undefined* ]]; then
    extra_cmake_flags="-DENABLE_SANITIZERS=undefined"
elif [[ $CFLAGS = *sanitize=thread* ]]; then
    extra_cmake_flags="-DENABLE_SANITIZERS=thread"
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
    -DBUILD_WITH_LIBNL=OFF \
    -DENABLE_REMOTE=OFF \
    -DDISABLE_BLUETOOTH=ON \
    -DDISABLE_DBUS=ON \
    -DDISABLE_DAG=ON \
    -DDISABLE_SNF=ON \
    -DDISABLE_RDMA=ON \
    -DDISABLE_NETMAP=ON \
    -DENABLE_PROFILING=OFF \
    -DUSE_STATIC_RT=ON \
    $extra_cmake_flags \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the libraries were created
if [ -f "$WORK/lib/libpcap.a" ]; then
    echo "Success: libpcap.a created at $WORK/lib/libpcap.a"
else
    echo "Error: libpcap.a not found in $WORK/lib"
    exit 1
fi
