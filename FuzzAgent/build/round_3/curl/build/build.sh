#!/bin/bash
set -e

# Set non-interactive mode for apt
export DEBIAN_FRONTEND=noninteractive
export APT_LISTCHANGES_FRONTEND=none
export DEBCONF_NOWARNINGS=yes

# 1. Clean and prep
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# 2. Install build dependencies and development libraries
apt-get update && apt-get install -y --no-install-recommends \
    autoconf \
    automake \
    libtool \
    pkg-config \
    libssl-dev \
    zlib1g-dev \
    libssh2-1-dev \
    libnghttp2-dev \
    libpsl-dev \
    libzstd-dev \
    libbrotli-dev \
    libidn2-dev \
    librtmp-dev \
    libkrb5-dev \
    libgssapi-krb5-2 \
    ca-certificates

# 3. Create directories
mkdir -p "$WORK/lib"
mkdir -p "$WORK/include"

# 4. Generate configure script if needed
if [ ! -f "$SRC/configure" ]; then
    echo "Generating configure script..."
    cd "$SRC"
    autoreconf -fi
    cd /tmp/build_tmp
fi

# 5. Append to CFLAGS/CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"
export LDFLAGS="$LDFLAGS -L$WORK/lib"

# 6. Configure curl for static library build
"$SRC/configure" \
    --prefix="$WORK" \
    --enable-static \
    --disable-shared \
    --enable-ipv6 \
    --with-openssl \
    --with-zlib \
    --with-libssh2 \
    --with-nghttp2 \
    --with-libpsl \
    --with-zstd \
    --with-brotli \
    --with-libidn2 \
    --with-librtmp \
    --with-gssapi \
    --disable-ldap \
    --disable-ldaps \
    --disable-manual \
    --disable-dependency-tracking \
    --disable-silent-rules \
    --disable-ares \
    --disable-debug \
    --disable-curldebug \
    --disable-option-checking \
    --disable-werror \
    CC="$CC" \
    CXX="$CXX"

# 7. Build & Install
make -j$(nproc)
make install

# 8. Verify installation
if [ -f "$WORK/lib/libcurl.a" ]; then
    echo "Success: libcurl.a installed to $WORK/lib"
    # Check if headers are installed
    if [ -f "$WORK/include/curl/curl.h" ]; then
        echo "Success: curl headers installed to $WORK/include"
    else
        echo "Warning: curl headers not found in $WORK/include"
        # Copy headers manually if needed
        if [ -d "$SRC/include/curl" ]; then
            mkdir -p "$WORK/include/curl"
            cp -r "$SRC/include/curl/"* "$WORK/include/curl/"
            echo "Copied headers from source to $WORK/include"
        fi
    fi
else
    echo "Error: libcurl.a not found in $WORK/lib"
    exit 1
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
