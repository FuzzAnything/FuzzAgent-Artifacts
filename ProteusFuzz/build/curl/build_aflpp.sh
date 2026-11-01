#!/bin/bash
set -e # Fail immediately on error

# 1. Clean and Prep
rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/curl/.build-work/build_aflpp && mkdir /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/curl/.build-work/build_aflpp && cd /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/curl/.build-work/build_aflpp

# 2. Configure with CMake for curl with proper feature configuration
# According to user request: rebuild with CURL_BROTLI=ON and CURL_ZSTD=ON since those libraries are now installed
# Keep CURL_USE_LIBPSL=ON, USE_NGHTTP2=ON, USE_LIBIDN2=ON
# Enable minimal features: CURL_ZLIB=ON, CURL_USE_OPENSSL=ON
# Disable other optional features: CURL_USE_GSSAPI=OFF, CURL_USE_LIBSSH2=OFF
# NOTE: We strictly use $CC, $CXX, and append to $CFLAGS/$CXXFLAGS
cmake \
  -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DZLIB_LIBRARY="$ZLIB_LIBRARY" \
    -DZLIB_INCLUDE_DIR="$ZLIB_INCLUDE_DIR" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_C_COMPILER="$CC" \
  -DCMAKE_CXX_COMPILER="$CXX" \
  -DCMAKE_C_FLAGS="$CFLAGS" \
  -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DBUILD_CURL_EXE=OFF \
  -DBUILD_STATIC_CURL=OFF \
  -DENABLE_MANUAL=OFF \
  -DCURL_USE_OPENSSL=ON \
  -DCURL_ZLIB=ON \
  -DHTTP_ONLY=OFF \
  -DCURL_BROTLI=ON \
  -DCURL_ZSTD=ON \
  -DCURL_USE_LIBPSL=ON \
  -DCURL_USE_GSSAPI=OFF \
  -DCURL_USE_LIBSSH2=OFF \
  -DUSE_LIBIDN2=ON \
  -DUSE_NGHTTP2=ON \
  -DUSE_NGTCP2=OFF \
  -DUSE_QUICHE=OFF \
  -DENABLE_ARES=OFF \
  "$SRC"

# 3. Build & Install
make -j$(nproc)
make install

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/targets/curl/.build-work/build_aflpp
