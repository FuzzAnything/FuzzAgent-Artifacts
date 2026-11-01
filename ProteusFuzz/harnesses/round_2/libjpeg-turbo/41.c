// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle *handle) {
    if (handle && *handle) {
        tj3Destroy(*handle);
        *handle = NULL;
    }
}

static void cleanup_buffer(unsigned char **buf) {
    if (buf && *buf) {
        tj3Free(*buf);
        *buf = NULL;
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Basic fuzzing parameters derived from input
    int width = (Data[0] % 256) + 1;
    int height = (Size > 1 ? (Data[1] % 256) + 1 : 1);
    int subsamp = (Size > 2 ? Data[2] % 4 : 0); // TJSAMP_444 to TJSAMP_GRAY
    int componentID = (Size > 3 ? Data[3] % 3 : 0);
    int pixelFormat = (Size > 4 ? Data[4] % 6 : 0); // TJPF_RGB to TJPF_GRAY

    // Clamp dimensions to reasonable limits to prevent OOM
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Test 1: tj3YUVPlaneWidth (No handle required)
    int planeWidth = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)planeWidth; // Use return value to suppress warnings

    // Initialize handle for compression
    tjhandle compHandle = tj3Init(TJINIT_COMPRESS);
    if (!compHandle) return 0;

    // Initialize handle for decompression
    tjhandle decompHandle = tj3Init(TJINIT_DECOMPRESS);
    if (!decompHandle) {
        cleanup_handle(&compHandle);
        return 0;
    }

    // Allocate YUV planes
    // Calculate plane sizes based on subsampling
    // Fixed: tj3YUVPlaneHeight requires componentID as first argument
    int planeWidthY = tj3YUVPlaneWidth(0, width, subsamp);
    int planeHeightY = tj3YUVPlaneHeight(0, height, subsamp);
    int planeWidthUV = tj3YUVPlaneWidth(1, width, subsamp);
    int planeHeightUV = tj3YUVPlaneHeight(1, height, subsamp);

    size_t sizeY = (size_t)planeWidthY * planeHeightY;
    size_t sizeUV = (size_t)planeWidthUV * planeHeightUV;

    unsigned char *planeY = (unsigned char *)tj3Alloc(sizeY);
    unsigned char *planeU = (unsigned char *)tj3Alloc(sizeUV);
    unsigned char *planeV = (unsigned char *)tj3Alloc(sizeUV);

    if (!planeY || !planeU || !planeV) {
        cleanup_buffer(&planeY);
        cleanup_buffer(&planeU);
        cleanup_buffer(&planeV);
        cleanup_handle(&compHandle);
        cleanup_handle(&decompHandle);
        return 0;
    }

    // Fill planes with fuzz data
    memset(planeY, 0, sizeY);
    memset(planeU, 0, sizeUV);
    memset(planeV, 0, sizeUV);

    if (Size > 0) {
        size_t copyLen = Size;
        if (copyLen > sizeY) copyLen = sizeY;
        memcpy(planeY, Data, copyLen);
    }
    if (Size > sizeY) {
        size_t copyLen = Size - sizeY;
        if (copyLen > sizeUV) copyLen = sizeUV;
        memcpy(planeU, Data + sizeY, copyLen);
    }
    if (Size > sizeY + sizeUV) {
        size_t copyLen = Size - (sizeY + sizeUV);
        if (copyLen > sizeUV) copyLen = sizeUV;
        memcpy(planeV, Data + sizeY + sizeUV, copyLen);
    }

    const unsigned char *srcPlanes[3] = {planeY, planeU, planeV};
    int strides[3] = {planeWidthY, planeWidthUV, planeWidthUV};

    // Test 2: tj3CompressFromYUVPlanes8
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    int res1 = tj3CompressFromYUVPlanes8(compHandle, srcPlanes, width, strides, height, &jpegBuf, &jpegSize);
    (void)res1;

    // Test 3: tjCompressFromYUVPlanes (Wrapper)
    unsigned char *jpegBuf2 = NULL;
    unsigned long jpegSize2 = 0;
    int res2 = tjCompressFromYUVPlanes(compHandle, srcPlanes, width, strides, height, subsamp, &jpegBuf2, &jpegSize2, 75, 0);
    (void)res2;

    // Test 4: tjDecompressToYUVPlanes (Requires valid JPEG)
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int dstStrides[3] = {0, 0, 0};
    
    // Only attempt decompression if we have a valid JPEG from previous step
    if (jpegBuf && jpegSize > 0) {
        // Allocate destination planes
        dstPlanes[0] = (unsigned char *)tj3Alloc(sizeY);
        dstPlanes[1] = (unsigned char *)tj3Alloc(sizeUV);
        dstPlanes[2] = (unsigned char *)tj3Alloc(sizeUV);
        
        if (dstPlanes[0] && dstPlanes[1] && dstPlanes[2]) {
            int res3 = tjDecompressToYUVPlanes(decompHandle, jpegBuf, jpegSize, dstPlanes, width, dstStrides, height, 0);
            (void)res3;
        }
    }

    // Test 5: tj3DecodeYUVPlanes8
    // Allocate RGB buffer
    int pixelSize = tjPixelSize[pixelFormat];
    size_t rgbSize = (size_t)width * height * pixelSize;
    unsigned char *rgbBuf = (unsigned char *)tj3Alloc(rgbSize);
    
    if (rgbBuf) {
        int res4 = tj3DecodeYUVPlanes8(decompHandle, srcPlanes, strides, rgbBuf, width, width * pixelSize, height, pixelFormat);
        (void)res4;
    }

    // Test 6: tjDecodeYUVPlanes (Wrapper)
    unsigned char *rgbBuf2 = (unsigned char *)tj3Alloc(rgbSize);
    if (rgbBuf2) {
        int res5 = tjDecodeYUVPlanes(decompHandle, srcPlanes, strides, subsamp, rgbBuf2, width, width * pixelSize, height, pixelFormat, 0);
        (void)res5;
    }

    // Cleanup
    cleanup_buffer(&planeY);
    cleanup_buffer(&planeU);
    cleanup_buffer(&planeV);
    cleanup_buffer(&jpegBuf);
    cleanup_buffer(&jpegBuf2);
    
    if (dstPlanes[0]) tj3Free(dstPlanes[0]);
    if (dstPlanes[1]) tj3Free(dstPlanes[1]);
    if (dstPlanes[2]) tj3Free(dstPlanes[2]);
    
    cleanup_buffer(&rgbBuf);
    cleanup_buffer(&rgbBuf2);
    
    cleanup_handle(&compHandle);
    cleanup_handle(&decompHandle);

    return 0;
}