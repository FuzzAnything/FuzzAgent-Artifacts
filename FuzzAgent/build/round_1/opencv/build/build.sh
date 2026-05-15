#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure CMake for OpenCV based on oss-fuzz script
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$WORK" \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_C_FLAGS="$CFLAGS" \
    -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
    -DBUILD_SHARED_LIBS=OFF \
    -DOPENCV_GENERATE_PKGCONFIG=ON \
    -DOPENCV_FORCE_3RDPARTY_BUILD=ON \
    -DBUILD_TESTS=OFF \
    -DBUILD_PERF_TESTS=OFF \
    -DBUILD_opencv_apps=OFF \
    -DWITH_IMGCODEC_GIF=ON \
    -DWITH_IPP=OFF \
    -DWITH_GTK=OFF \
    -DWITH_QT=OFF \
    -DWITH_VTK=OFF \
    -DWITH_CUDA=OFF \
    -DWITH_OPENCL=OFF \
    -DWITH_EIGEN=OFF \
    -DWITH_FFMPEG=OFF \
    -DWITH_GSTREAMER=OFF \
    -DWITH_OPENEXR=ON \
    -DWITH_JPEG=ON \
    -DWITH_PNG=ON \
    -DWITH_TIFF=ON \
    -DWITH_WEBP=ON \
    -DWITH_JASPER=ON \
    -DWITH_OPENJPEG=ON \
    "$SRC"

# Build and install
make -j$(nproc)
make install

# After installation, check for 3rdparty libraries and copy them to $WORK/3rdparty
# OpenCV might install 3rdparty libraries to $WORK/lib/opencv4/3rdparty or similar
if [ -d "$WORK/lib/opencv4/3rdparty" ]; then
    echo "Found 3rdparty libraries in $WORK/lib/opencv4/3rdparty"
    mkdir -p "$WORK/3rdparty/lib"
    # Copy all .a files from opencv4/3rdparty to 3rdparty/lib
    find "$WORK/lib/opencv4/3rdparty" -name "*.a" -exec cp {} "$WORK/3rdparty/lib/" \;
    # Also copy any .so files if they exist
    find "$WORK/lib/opencv4/3rdparty" -name "*.so" -exec cp {} "$WORK/3rdparty/lib/" 2>/dev/null || true
fi

# Also check the build directory for any built 3rdparty libraries
if [ -d "/tmp/build_tmp/3rdparty" ]; then
    echo "Checking build directory for 3rdparty libraries"
    mkdir -p "$WORK/3rdparty/lib"
    find "/tmp/build_tmp/3rdparty" -name "*.a" -exec cp {} "$WORK/3rdparty/lib/" 2>/dev/null || true
fi

# Verify installation
if [ -f "$WORK/lib/libopencv_core.a" ]; then
    echo "OpenCV static library built successfully"
else
    echo "ERROR: OpenCV static library not found"
    exit 1
fi

# List the 3rdparty libraries for debugging
echo "3rdparty libraries in $WORK/3rdparty/lib:"
ls -la "$WORK/3rdparty/lib/" 2>/dev/null || echo "No 3rdparty libraries found"
