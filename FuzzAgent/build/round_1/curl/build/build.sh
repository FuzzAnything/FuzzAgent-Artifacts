#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Export CFLAGS and CXXFLAGS by appending, not overwriting
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Configure curl with CMake
# Enable static libraries, disable shared libraries for fuzzing
# Disable unnecessary features to speed up compilation
# Disable optional dependencies that may not be available
cmake -DCMAKE_INSTALL_PREFIX="$WORK" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_STATIC_LIBS=ON \
      -DBUILD_CURL_EXE=OFF \
      -DBUILD_TESTING=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_LIBCURL_DOCS=OFF \
      -DBUILD_MISC_DOCS=OFF \
      -DCURL_BUILD_EVERYTHING=OFF \
      -DCURL_ENABLE_SSL=ON \
      -DENABLE_IPV6=ON \
      -DENABLE_THREADED_RESOLVER=ON \
      -DENABLE_UNIX_SOCKETS=ON \
      -DCURL_DISABLE_ALTSVC=OFF \
      -DCURL_DISABLE_AWS=OFF \
      -DCURL_DISABLE_BASIC_AUTH=OFF \
      -DCURL_DISABLE_BEARER_AUTH=OFF \
      -DCURL_DISABLE_BINDLOCAL=OFF \
      -DCURL_DISABLE_COOKIES=OFF \
      -DCURL_DISABLE_DICT=OFF \
      -DCURL_DISABLE_DIGEST_AUTH=OFF \
      -DCURL_USE_LIBPSL=OFF \
      -DCURL_USE_LIBSSH2=OFF \
      -DCURL_USE_GSSAPI=OFF \
      -DCURL_USE_GSASL=OFF \
      -DCURL_USE_LIBSSH=OFF \
      "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the libraries were installed
if [ -f "$WORK/lib/libcurl.a" ]; then
    echo "Successfully built libcurl.a"
else
    echo "ERROR: libcurl.a not found in $WORK/lib"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
