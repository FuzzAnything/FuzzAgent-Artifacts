#!/bin/bash
set -e

# Clean and prep build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Generate configure script if needed
if [ ! -f "$SRC/configure" ]; then
    cd "$SRC"
    autoreconf -fi
    cd /tmp/build_tmp
fi

# Append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

# Configure curl with autotools
$SRC/configure \
    --prefix="$WORK" \
    --disable-shared \
    --enable-static \
    --disable-debug \
    --disable-curldebug \
    --enable-symbol-hiding \
    --disable-manual \
    --disable-ldap \
    --disable-ldaps \
    --disable-rtsp \
    --disable-proxy \
    --disable-dict \
    --disable-telnet \
    --disable-tftp \
    --disable-pop3 \
    --disable-imap \
    --disable-smb \
    --disable-smtp \
    --disable-gopher \
    --without-ssl \
    --without-gnutls \
    --without-mbedtls \
    --without-wolfssl \
    --without-nss \
    --without-libssh2 \
    --without-librtmp \
    --without-nghttp2 \
    --without-zlib \
    --without-brotli \
    --without-zstd \
    --without-libidn2 \
    --without-nghttp3 \
    --without-ngtcp2 \
    --without-quiche \
    --without-libpsl \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS" \
    LDFLAGS="$LDFLAGS"

# Build and install
make -j$(nproc)
make install

# Verify libcurl.a exists
if [ -f "$WORK/lib/libcurl.a" ]; then
    echo "Successfully built libcurl.a"
else
    echo "Error: libcurl.a not found in $WORK/lib"
    exit 1
fi
