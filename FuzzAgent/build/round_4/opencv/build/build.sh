#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure CMake for OpenCV
# Note: We append to CFLAGS/CXXFLAGS to preserve fuzzing instrumentation
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Check for ASAN to handle potential -Wl,-z,defs conflicts
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   echo "Detected address sanitizer, adding compatibility flags"
   extra_cmake_flags="-DSANITIZE=address"
fi

cmake \
  -DCMAKE_INSTALL_PREFIX="$WORK" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_C_COMPILER="$CC" \
  -DCMAKE_CXX_COMPILER="$CXX" \
  -DCMAKE_C_FLAGS="$CFLAGS" \
  -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DBUILD_PERF_TESTS=OFF \
  -DBUILD_opencv_apps=OFF \
  -DOPENCV_GENERATE_PKGCONFIG=ON \
  -DOPENCV_FORCE_3RDPARTY_BUILD=ON \
  -DWITH_IPP=OFF \
  -DWITH_GTK=OFF \
  -DWITH_QT=OFF \
  -DWITH_WEBP=ON \
  -DWITH_JPEG=ON \
  -DWITH_PNG=ON \
  -DWITH_TIFF=ON \
  -DWITH_JASPER=ON \
  -DWITH_OPENJPEG=ON \
  -DWITH_OPENEXR=ON \
  -DWITH_FFMPEG=ON \
  -DWITH_GSTREAMER=OFF \
  -DWITH_1394=OFF \
  -DWITH_V4L=OFF \
  -DWITH_LAPACK=OFF \
  -DWITH_EIGEN=OFF \
  -DWITH_OPENCL=OFF \
  -DWITH_OPENCL_SVM=OFF \
  -DWITH_OPENCLAMDFFT=OFF \
  -DWITH_OPENCLAMDBLAS=OFF \
  $extra_cmake_flags \
  "$SRC"

# Build and install
make -j$(nproc)
make install

# Copy 3rdparty libraries to $WORK/3rdparty directory
mkdir -p "$WORK/3rdparty/lib"
# Find all 3rdparty static libraries in the build directory
find /tmp/build_tmp/3rdparty -name "*.a" -type f | while read lib; do
    cp -f "$lib" "$WORK/3rdparty/lib/"
done

# Also check for libraries in the installed location
if [ -d "$WORK/lib/opencv4/3rdparty" ]; then
    cp -f "$WORK/lib/opencv4/3rdparty"/*.a "$WORK/3rdparty/lib/" 2>/dev/null || true
fi

# Verify installation
if [ -f "$WORK/lib/libopencv_core.a" ]; then
    echo "Build successful: OpenCV static libraries installed to $WORK/lib"
    echo "3rdparty libraries copied to $WORK/3rdparty/lib"
    ls -la "$WORK/3rdparty/lib/"*.a 2>/dev/null | wc -l || echo "No 3rdparty libraries found"
else
    echo "ERROR: OpenCV core library not found at $WORK/lib/libopencv_core.a"
    exit 1
fi
