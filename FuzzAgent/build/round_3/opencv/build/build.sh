#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Append extra flags for ASAN compatibility if needed
extra_cmake_flags=""
if [[ $CFLAGS = *sanitize=address* ]]; then
   extra_cmake_flags="-DOPENCV_SANITIZE_ADDRESS=ON"
fi

# Configure OpenCV with CMake based on oss-fuzz reference
# Build static libraries only for fuzzing harness linking
# Force 3rdparty build to ensure all dependencies are available
# Disable tests, apps, and documentation to speed up compilation
cmake \
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
  $extra_cmake_flags \
  "$SRC"

# Build and install
make -j$(nproc)
make install

# After installation, check for 3rdparty libraries and copy them to $WORK/3rdparty/lib
# OpenCV might install 3rdparty libraries to $WORK/lib/opencv4/3rdparty or similar
mkdir -p "$WORK/3rdparty/lib"

if [ -d "$WORK/lib/opencv4/3rdparty" ]; then
    echo "Found 3rdparty libraries in $WORK/lib/opencv4/3rdparty"
    # Copy all .a files from opencv4/3rdparty to 3rdparty/lib
    find "$WORK/lib/opencv4/3rdparty" -name "*.a" -exec cp {} "$WORK/3rdparty/lib/" \;
    # Also copy any .so files if they exist
    find "$WORK/lib/opencv4/3rdparty" -name "*.so" -exec cp {} "$WORK/3rdparty/lib/" 2>/dev/null || true
    echo "Copied from opencv4/3rdparty:"
    ls -la "$WORK/3rdparty/lib/" 2>/dev/null || echo "No files copied"
fi

# Also check the build directory for any built 3rdparty libraries
if [ -d "/tmp/build_tmp/3rdparty" ]; then
    echo "Checking build directory for 3rdparty libraries"
    find "/tmp/build_tmp/3rdparty" -name "*.a" -exec cp {} "$WORK/3rdparty/lib/" 2>/dev/null || true
    echo "Copied from build directory:"
    ls -la "$WORK/3rdparty/lib/" 2>/dev/null || echo "No files copied"
fi

# Copy any 3rdparty libraries that were installed directly to $WORK/lib to $WORK/3rdparty/lib
# These are typically libraries like libade.a, libzlib.a, liblibjpeg-turbo.a, etc.
echo "Checking for 3rdparty libraries in $WORK/lib"
for lib in "$WORK/lib"/*.a; do
    if [ -f "$lib" ]; then
        libname=$(basename "$lib")
        # Skip OpenCV libraries (they start with libopencv_)
        if [[ ! "$libname" =~ ^libopencv_ ]]; then
            echo "Copying 3rdparty library: $libname"
            cp "$lib" "$WORK/3rdparty/lib/"
        fi
    fi
done

# List all 3rdparty libraries for debugging
echo "All 3rdparty libraries in $WORK/3rdparty/lib:"
ls -la "$WORK/3rdparty/lib/" 2>/dev/null || echo "No 3rdparty libraries found"

# Verify installation
if [ -f "$WORK/lib/libopencv_core.a" ]; then
    echo "OpenCV static library built successfully"
else
    echo "ERROR: OpenCV static library not found"
    exit 1
fi
