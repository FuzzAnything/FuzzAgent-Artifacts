// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_free(unsigned char *buffer) {
    // Helper function to safely free buffers
    if (buffer) {
        free(buffer);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Initialize compressor handle
    tjhandle handle = tjInitCompress();
    if (!handle) {
        return 0;
    }

    // Parse input data for parameters
    if (Size < 32) {
        tjDestroy(handle);
        return 0;
    }

    // Extract parameters from fuzz data
    int width = *(const int *)(Data + 0) & 0xFFF;  // Limit to reasonable size
    int height = *(const int *)(Data + 4) & 0xFFF;
    int pitch = *(const int *)(Data + 8) & 0xFFFF;
    int pixelFormat = *(const int *)(Data + 12) % 12;  // TJPF_* values 0-11
    int subsamp = *(const int *)(Data + 16) % 5;      // TJSAMP_* values 0-4
    int jpegQual = *(const int *)(Data + 20) % 101;   // 0-100
    int flags = *(const int *)(Data + 24);
    int align = *(const int *)(Data + 28) & 0xF;      // 1-16 alignment

    // Ensure minimum dimensions
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (pitch <= 0) pitch = width * tjPixelSize[pixelFormat];
    if (align <= 0) align = 4;
    if (align > 16) align = 16;

    // Calculate required buffer sizes
    size_t rgbSize = pitch * height;
    size_t yuvSize = tjBufSizeYUV2(width, align, height, subsamp);
    
    // Check if we have enough data
    if (Size < 32 + rgbSize + yuvSize) {
        tjDestroy(handle);
        return 0;
    }

    const uint8_t *rgbData = Data + 32;
    const uint8_t *yuvData = rgbData + rgbSize;

    // Test tjCompress2
    unsigned char *jpegBuf1 = NULL;
    unsigned long jpegSize1 = 0;
    if (tjCompress2(handle, rgbData, width, pitch, height, pixelFormat,
                   &jpegBuf1, &jpegSize1, subsamp, jpegQual, flags) == 0) {
        if (jpegBuf1) {
            tjFree(jpegBuf1);
        }
    }

    // Test tjCompressFromYUV
    unsigned char *jpegBuf2 = NULL;
    unsigned long jpegSize2 = 0;
    if (tjCompressFromYUV(handle, yuvData, width, align, height, subsamp,
                         &jpegBuf2, &jpegSize2, jpegQual, flags) == 0) {
        if (jpegBuf2) {
            tjFree(jpegBuf2);
        }
    }

    // Test tjCompress with pre-allocated buffer
    unsigned long compressedSize = tjBufSize(width, height, subsamp);
    unsigned char *dstBuf = (unsigned char *)malloc(compressedSize);
    if (dstBuf) {
        int pixelSize = tjPixelSize[pixelFormat];
        tjCompress(handle, (unsigned char *)rgbData, width, pitch, height,
                  pixelSize, dstBuf, &compressedSize, subsamp, jpegQual, flags);
        free(dstBuf);
    }

    // Test tjDecodeYUVPlanes with dummy data
    // Create dummy YUV planes
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    int yuvWidth = width;
    int yuvHeight = height;
    
    // Calculate plane sizes
    unsigned long yPlaneSize = tjPlaneSizeYUV(0, yuvWidth, 0, yuvHeight, subsamp);
    unsigned long uPlaneSize = tjPlaneSizeYUV(1, yuvWidth, 0, yuvHeight, subsamp);
    unsigned long vPlaneSize = tjPlaneSizeYUV(2, yuvWidth, 0, yuvHeight, subsamp);
    
    if (Size >= 32 + rgbSize + yuvSize + yPlaneSize + uPlaneSize + vPlaneSize) {
        const uint8_t *planeData = yuvData + yuvSize;
        srcPlanes[0] = planeData;
        srcPlanes[1] = planeData + yPlaneSize;
        srcPlanes[2] = planeData + yPlaneSize + uPlaneSize;
        
        strides[0] = yuvWidth;
        strides[1] = tjPlaneWidth(1, yuvWidth, subsamp);
        strides[2] = tjPlaneWidth(2, yuvWidth, subsamp);
        
        unsigned char *dstBuf2 = (unsigned char *)malloc(rgbSize);
        if (dstBuf2) {
            tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                             dstBuf2, yuvWidth, pitch, yuvHeight,
                             pixelFormat, flags);
            free(dstBuf2);
        }
    }

    // Cleanup
    tjDestroy(handle);
    return 0;
}