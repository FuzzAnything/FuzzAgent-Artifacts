#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp
mkdir -p /tmp/build_tmp
cd /tmp/build_tmp

# Configure CMake for OpenCV based on OSS-Fuzz reference
# Note: We append to CFLAGS and CXXFLAGS, never overwrite
export CFLAGS="$CFLAGS -I$WORK/include"
export CXXFLAGS="$CXXFLAGS -I$WORK/include"

# Enable as many features as possible for comprehensive testing
# Disable tests, docs, and examples to speed up compilation
cmake \
  -DCMAKE_BUILD_TYPE=Release \
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
  -DOPENCV_GENERATE_PKGCONFIG=ON \
  -DOPENCV_FORCE_3RDPARTY_BUILD=ON \
  -DWITH_IPP=OFF \
  -DWITH_IMGCODEC_GIF=ON \
  -DWITH_1394=OFF \
  -DWITH_ARAVIS=OFF \
  -DWITH_ARITH_DEC=ON \
  -DWITH_ARITH_ENC=ON \
  -DWITH_CLP=OFF \
  -DWITH_CUBLAS=OFF \
  -DWITH_CUDA=OFF \
  -DWITH_CUDNN=OFF \
  -DWITH_DIRECTX=OFF \
  -DWITH_DSHOW=OFF \
  -DWITH_EIGEN=OFF \
  -DWITH_FFMPEG=OFF \
  -DWITH_GDAL=OFF \
  -DWITH_GDCM=OFF \
  -DWITH_GPHOTO2=OFF \
  -DWITH_GSTREAMER=OFF \
  -DWITH_HALIDE=OFF \
  -DWITH_IMGCODEC_HDR=ON \
  -DWITH_IMGCODEC_PFM=ON \
  -DWITH_IMGCODEC_PXM=ON \
  -DWITH_IMGCODEC_SUNRASTER=ON \
  -DWITH_INF_ENGINE=OFF \
  -DWITH_IPP=OFF \
  -DWITH_ITT=OFF \
  -DWITH_JASPER=OFF \
  -DWITH_JPEG=ON \
  -DWITH_LAPACK=OFF \
  -DWITH_LIBREALSENSE=OFF \
  -DWITH_MFX=OFF \
  -DWITH_MSMF=OFF \
  -DWITH_MSMF_DXVA=OFF \
  -DWITH_NVCUVID=OFF \
  -DWITH_OPENCL=OFF \
  -DWITH_OPENCLAMDBLAS=OFF \
  -DWITH_OPENCLAMDFFT=OFF \
  -DWITH_OPENCL_SVM=OFF \
  -DWITH_OPENEXR=ON \
  -DWITH_OPENGL=OFF \
  -DWITH_OPENJPEG=ON \
  -DWITH_OPENMP=OFF \
  -DWITH_OPENNN=OFF \
  -DWITH_OPENNI=OFF \
  -DWITH_OPENNI2=OFF \
  -DWITH_OPENVX=OFF \
  -DWITH_PNG=ON \
  -DWITH_PROTOBUF=ON \
  -DWITH_PTHREADS_PF=ON \
  -DWITH_PVAPI=OFF \
  -DWITH_QT=OFF \
  -DWITH_QUIRC=OFF \
  -DWITH_TBB=OFF \
  -DWITH_TIFF=ON \
  -DWITH_UNICAP=OFF \
  -DWITH_V4L=OFF \
  -DWITH_VA=OFF \
  -DWITH_VA_INTEL=OFF \
  -DWITH_VTK=OFF \
  -DWITH_VULKAN=OFF \
  -DWITH_WEBP=ON \
  -DWITH_WIN32UI=OFF \
  -DWITH_XIMEA=OFF \
  -DWITH_XINE=OFF \
  "$SRC"

# Build and install
make -j$(nproc)
make install

# Copy 3rdparty libraries to $WORK/3rdparty/lib
# OpenCV installs 3rdparty libraries to $WORK/lib/opencv4/3rdparty
mkdir -p "$WORK/3rdparty/lib"

if [ -d "$WORK/lib/opencv4/3rdparty" ]; then
    echo "Copying 3rdparty libraries from $WORK/lib/opencv4/3rdparty"
    find "$WORK/lib/opencv4/3rdparty" -name "*.a" -exec cp {} "$WORK/3rdparty/lib/" \;
    find "$WORK/lib/opencv4/3rdparty" -name "*.so" -exec cp {} "$WORK/3rdparty/lib/" 2>/dev/null || true
fi

# Also look for 3rdparty libraries in the build directory
if [ -d "/tmp/build_tmp/3rdparty" ]; then
    echo "Copying 3rdparty libraries from build directory"
    find "/tmp/build_tmp/3rdparty" -name "*.a" -exec cp {} "$WORK/3rdparty/lib/" 2>/dev/null || true
fi

# Copy any non-opencv libraries from $WORK/lib to 3rdparty
for lib in "$WORK/lib"/*.a; do
    if [ -f "$lib" ]; then
        libname=$(basename "$lib")
        if [[ ! "$libname" =~ ^libopencv_ ]]; then
            echo "Copying 3rdparty library: $libname"
            cp "$lib" "$WORK/3rdparty/lib/"
        fi
    fi
done

echo "3rdparty libraries copied to $WORK/3rdparty/lib:"
ls -la "$WORK/3rdparty/lib/" 2>/dev/null || echo "No 3rdparty libraries found"

# Don't delete the temporal build directory after building! It will be used for postprocessing.
# rm -rf /tmp/build_tmp
