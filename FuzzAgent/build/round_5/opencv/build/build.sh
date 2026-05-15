#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir -p /tmp/build_tmp && cd /tmp/build_tmp

# Use environment variables properly
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Configure OpenCV with CMake
# Enable as many features as possible for comprehensive testing
# Disable tests, docs, and examples to speed up compilation
cmake \
  -DCMAKE_INSTALL_PREFIX="$WORK" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_C_COMPILER="$CC" \
  -DCMAKE_CXX_COMPILER="$CXX" \
  -DCMAKE_C_FLAGS="$CFLAGS" \
  -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTS=OFF \
  -DBUILD_PERF_TESTS=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_DOCS=OFF \
  -DBUILD_opencv_apps=OFF \
  -DWITH_IPP=OFF \
  -DWITH_OPENCL=OFF \
  -DWITH_CUDA=OFF \
  -DWITH_OPENCLAMDFFT=OFF \
  -DWITH_OPENCLAMDBLAS=OFF \
  -DWITH_1394=OFF \
  -DWITH_ARITH_DEC=OFF \
  -DWITH_ARITH_ENC=OFF \
  -DWITH_CLP=OFF \
  -DWITH_CSTRIPES=OFF \
  -DWITH_EIGEN=OFF \
  -DWITH_FFMPEG=OFF \
  -DWITH_GDAL=OFF \
  -DWITH_GIGEAPI=OFF \
  -DWITH_GPHOTO2=OFF \
  -DWITH_GSTREAMER=OFF \
  -DWITH_GTK=OFF \
  -DWITH_HALIDE=OFF \
  -DWITH_IMGCODEC_GIF=ON \
  -DWITH_IMGCODEC_HDR=ON \
  -DWITH_IMGCODEC_PFM=ON \
  -DWITH_IMGCODEC_PXM=ON \
  -DWITH_IMGCODEC_SUNRASTER=ON \
  -DWITH_INF_ENGINE=OFF \
  -DWITH_ITT=OFF \
  -DWITH_JASPER=OFF \
  -DWITH_JPEG=ON \
  -DWITH_LAPACK=OFF \
  -DWITH_MATLAB=OFF \
  -DWITH_MFX=OFF \
  -DWITH_OPENCL_SVM=OFF \
  -DWITH_OPENEXR=OFF \
  -DWITH_OPENGL=OFF \
  -DWITH_OPENJPEG=OFF \
  -DWITH_OPENMP=OFF \
  -DWITH_OPENNI=OFF \
  -DWITH_OPENNI2=OFF \
  -DWITH_OPENVX=OFF \
  -DWITH_PNG=ON \
  -DWITH_PROTOBUF=OFF \
  -DWITH_PTHREADS_PF=OFF \
  -DWITH_PVAPI=OFF \
  -DWITH_QT=OFF \
  -DWITH_QUIRC=OFF \
  -DWITH_TBB=OFF \
  -DWITH_TIFF=ON \
  -DWITH_V4L=OFF \
  -DWITH_VA=OFF \
  -DWITH_VA_INTEL=OFF \
  -DWITH_VTK=OFF \
  -DWITH_WEBP=ON \
  -DWITH_WIN32UI=OFF \
  -DOPENCV_FORCE_3RDPARTY_BUILD=ON \
  "$SRC"

# Build and install
make -j$(nproc)
make install

# Copy 3rdparty libraries to $WORK/3rdparty/lib
mkdir -p "$WORK/3rdparty/lib"
if [ -d "/tmp/build_tmp/3rdparty/lib" ]; then
  cp -f /tmp/build_tmp/3rdparty/lib/*.a "$WORK/3rdparty/lib/" 2>/dev/null || true
fi

# Also ensure they are available in the standard lib directory for linking
# Some builds expect them in lib/opencv4/3rdparty
mkdir -p "$WORK/lib/opencv4/3rdparty"
if [ -d "/tmp/build_tmp/3rdparty/lib" ]; then
  cp -f /tmp/build_tmp/3rdparty/lib/*.a "$WORK/lib/opencv4/3rdparty/" 2>/dev/null || true
fi

# Don't delete the temporal build directory after building! It will be used for postprocessing.
