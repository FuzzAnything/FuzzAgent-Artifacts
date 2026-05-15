#!/bin/bash
set -e  # Fail immediately on error

# Debug: Print environment variables
echo "SRC: $SRC"
echo "WORK: $WORK"
echo "CC: $CC"
echo "CXX: $CXX"
echo "CFLAGS: $CFLAGS"
echo "CXXFLAGS: $CXXFLAGS"
echo "OUTPUT: $OUTPUT"

# Install build dependencies
export DEBIAN_FRONTEND=noninteractive
apt-get update
apt-get install -y --no-install-recommends \
    apt-utils \
    cmake \
    ninja-build \
    zlib1g-dev \
    git \
    pkg-config \
    autoconf \
    automake \
    libtool \
    curl \
    ca-certificates

# Get abseil version from protobuf's dependencies file
ABSEIL_VERSION="20250512.1"  # From /root/src/protobuf/cmake/dependencies.cmake
echo "Building abseil version: $ABSEIL_VERSION"

# Build and install abseil first
cd /tmp
rm -rf abseil-cpp
git clone --depth 1 --branch "$ABSEIL_VERSION" https://github.com/abseil/abseil-cpp.git
cd abseil-cpp

# Build abseil with static libraries for protobuf
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX="$WORK/3rdparty" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DABSL_BUILD_TESTING=OFF \
      -DABSL_USE_EXTERNAL_GOOGLETEST=OFF \
      -DABSL_FIND_GOOGLETEST=OFF \
      -DABSL_PROPAGATE_CXX_STD=ON \
      -DABSL_ENABLE_INSTALL=ON \
      ..

make -j$(nproc)
make install

# Verify abseil installation
echo "Abseil installed to: $WORK/3rdparty"
find "$WORK/3rdparty" -name "*.a" | xargs -I {} ls -la {} || true

# Clean and create temporary build directory for protobuf
rm -rf /tmp/build_tmp
mkdir -p /tmp/build_tmp
cd /tmp/build_tmp

# Check if ASAN is enabled and add appropriate flags
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
    echo "ASAN detected in CFLAGS"
    # For protobuf, we might need special handling if required
fi

# Configure protobuf with CMake, pointing to our abseil installation
# Use CMAKE_PREFIX_PATH to help find abseil
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_PREFIX_PATH="$WORK/3rdparty" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -Dprotobuf_BUILD_SHARED_LIBS=OFF \
      -Dprotobuf_BUILD_TESTS=OFF \
      -Dprotobuf_BUILD_CONFORMANCE=OFF \
      -Dprotobuf_BUILD_EXAMPLES=OFF \
      -Dprotobuf_BUILD_PROTOBUF_BINARIES=ON \
      -Dprotobuf_BUILD_PROTOC_BINARIES=ON \
      -Dprotobuf_BUILD_LIBPROTOC=ON \
      -Dprotobuf_BUILD_LIBUPB=ON \
      -Dprotobuf_WITH_ZLIB=ON \
      -Dprotobuf_INSTALL=ON \
      -Dprotobuf_MSVC_STATIC_RUNTIME=OFF \
      -Dprotobuf_LOCAL_DEPENDENCIES_ONLY=ON \
      -Dprotobuf_FORCE_FETCH_DEPENDENCIES=OFF \
      $extra_cmake_flags \
      "$SRC"

# Build with all available cores
make -j$(nproc)

# Install to $WORK directory
make install

# Verify the installation
echo "Checking installation in $WORK:"
ls -la "$WORK" || true
ls -la "$WORK/lib" || true
ls -la "$WORK/lib64" || true
ls -la "$WORK/include" || true

# Check for created libraries
find "$WORK" -name "*.a" -o -name "*.so" | xargs -I {} ls -la {} || true

# Check if we have the required static libraries
if find "$WORK" -name "*.a" | grep -q .; then
    echo "Static libraries found successfully!"
else
    echo "ERROR: No static libraries found!"
    exit 1
fi

echo "Build completed successfully!"
