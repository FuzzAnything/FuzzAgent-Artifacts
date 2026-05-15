#!/bin/bash
set -e  # Fail immediately on error

# Clean and create temporary build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Add custom cmake flags for sanitizer compatibility
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   extra_cmake_flags="-DSANITIZE=address"
fi

# Configure using CMake
# We use $CC and $CXX from environment, and append to $CFLAGS/$CXXFLAGS
cmake \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DCARES_STATIC=ON \
    -DCARES_SHARED=OFF \
    -DCARES_STATIC_PIC=ON \
    -DCARES_BUILD_TESTS=OFF \
    -DCARES_BUILD_CONTAINER_TESTS=OFF \
    -DCARES_BUILD_TOOLS=OFF \
    -DCARES_SYMBOL_HIDING=OFF \
    -DCARES_THREADS=ON \
    -DCARES_COVERAGE=OFF \
    $extra_cmake_flags \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# Verify the libraries were created
echo "Checking installed artifacts in $WORK"
ls -la "$WORK/lib/" || true
ls -la "$WORK/include/" || true

# Don't delete the temporal build directory after building! It will be used for postprocessing.
