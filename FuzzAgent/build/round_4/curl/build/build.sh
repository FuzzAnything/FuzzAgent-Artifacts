#!/bin/bash
set -e

# Install required dependencies
apt-get update && apt-get install -y \
    autoconf \
    automake \
    libtool \
    pkg-config \
    zlib1g-dev \
    libssl-dev \
    libidn2-dev

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir -p /tmp/build_tmp && cd /tmp/build_tmp

# Generate configure script if needed
if [ ! -f "$SRC/configure" ]; then
    cd "$SRC"
    autoreconf -fi
    cd /tmp/build_tmp
fi

# Configure curl based on OSS-Fuzz successful build
"$SRC/configure" \
    --prefix="$WORK" \
    --disable-shared \
    --enable-static \
    --with-openssl \
    --with-zlib \
    --enable-debug \
    --disable-curldebug \
    --disable-ares \
    --disable-threaded-resolver \
    --disable-pthreads \
    --disable-verbose \
    --disable-sspi \
    --disable-crypto-auth \
    --disable-ntlm-wb \
    --disable-tls-srp \
    --disable-unix-sockets \
    --disable-dependency-tracking \
    --disable-silent-rules \
    --disable-libtool-lock \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS" \
    LDFLAGS="$LDFLAGS"

# Build and install
make -j$(nproc)
make install

# Verify the library was built
if [ -f "$WORK/lib/libcurl.a" ]; then
    echo "Successfully built libcurl.a"
else
    echo "ERROR: libcurl.a not found in $WORK/lib"
    exit 1
fi

echo "Build completed successfully!"
ls -la "$WORK/lib/"
