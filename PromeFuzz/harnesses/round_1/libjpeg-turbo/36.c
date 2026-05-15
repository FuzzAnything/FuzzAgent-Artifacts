// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

#define MAX_DIMENSION 4096
#define MAX_PLANES 3

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static void cleanup_buffer(void *buf) {
    if (buf) {
        tj3Free(buf);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int subsamp = Data[4] % 6;  // 0-5 for TJSAMP values
    int align = 1 << (Data[5] % 4);  // 1, 2, 4, 8
    int jpegQual = Data[6] % 101;  // 0-100
    int flags = Data[7];
    int function_selector = Data[8] % 6;  // Which function to call

    // Limit dimensions to avoid excessive memory usage
    if (width <= 0 || height <= 0 || width > MAX_DIMENSION || height > MAX_DIMENSION) {
        width = 64;
        height = 64;
    }

    // Create TurboJPEG instance
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }

    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, jpegQual);

    // Prepare output buffers
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned long jpegSizeOld = 0;

    // Calculate required buffer sizes
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);

    if (yuvBufSize == 0 || jpegBufSize == 0) {
        cleanup_handle(handle);
        return 0;
    }

    // Allocate and prepare source buffers
    unsigned char *srcBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    if (!srcBuf) {
        cleanup_handle(handle);
        return 0;
    }

    // Fill source buffer with fuzzer data (wrap around if needed)
    for (size_t i = 0; i < yuvBufSize; i++) {
        srcBuf[i] = Data[(i + 9) % Size];
    }

    // Prepare plane pointers for planar functions
    const unsigned char *srcPlanes[MAX_PLANES];
    const unsigned char *srcPlanesOld[MAX_PLANES];
    int strides[MAX_PLANES];
    
    // Calculate plane sizes and set up planes
    size_t yPlaneSize = tj3YUVPlaneSize(width, 0, height, 0, subsamp);
    size_t uPlaneSize = tj3YUVPlaneSize(width, 1, height, 1, subsamp);
    size_t vPlaneSize = tj3YUVPlaneSize(width, 2, height, 2, subsamp);
    
    if (yPlaneSize == 0) {
        cleanup_buffer(srcBuf);
        cleanup_handle(handle);
        return 0;
    }

    // Set up planar data
    unsigned char *planeData = srcBuf;
    srcPlanes[0] = planeData;
    srcPlanesOld[0] = planeData;
    strides[0] = width;
    
    if (subsamp != TJSAMP_GRAY && uPlaneSize > 0 && vPlaneSize > 0) {
        srcPlanes[1] = planeData + yPlaneSize;
        srcPlanes[2] = planeData + yPlaneSize + uPlaneSize;
        srcPlanesOld[1] = srcPlanes[1];
        srcPlanesOld[2] = srcPlanes[2];
        strides[1] = width / (subsamp == TJSAMP_420 || subsamp == TJSAMP_422 ? 2 : 1);
        strides[2] = strides[1];
    } else {
        srcPlanes[1] = NULL;
        srcPlanes[2] = NULL;
        srcPlanesOld[1] = NULL;
        srcPlanesOld[2] = NULL;
        strides[1] = 0;
        strides[2] = 0;
    }

    // Call selected function based on fuzzer input
    switch (function_selector) {
        case 0:  // tj3CompressFromYUVPlanes8
            jpegBuf = NULL;
            jpegSize = 0;
            tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, &jpegBuf, &jpegSize);
            break;
            
        case 1:  // tj3CompressFromYUV8
            jpegBuf = NULL;
            jpegSize = 0;
            tj3CompressFromYUV8(handle, srcBuf, width, align, height, &jpegBuf, &jpegSize);
            break;
            
        case 2:  // tjCompressFromYUV (legacy API)
            jpegBuf = NULL;
            jpegSizeOld = 0;
            tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp, 
                             &jpegBuf, &jpegSizeOld, jpegQual, flags);
            break;
            
        case 3:  // tjCompressFromYUVPlanes (legacy API)
            jpegBuf = NULL;
            jpegSizeOld = 0;
            tjCompressFromYUVPlanes(handle, srcPlanesOld, width, strides, height, subsamp,
                                   &jpegBuf, &jpegSizeOld, jpegQual, flags);
            break;
            
        case 4:  // Test tj3Alloc and tj3JPEGBufSize
            jpegBuf = tj3Alloc(jpegBufSize);
            if (jpegBuf) {
                // Just test allocation, don't actually compress
                memset(jpegBuf, 0, jpegBufSize);
            }
            break;
            
        case 5:  // Test tj3JPEGBufSize with various parameters
            // Test with different dimensions
            for (int i = 0; i < 4; i++) {
                int testWidth = width >> i;
                int testHeight = height >> i;
                if (testWidth > 0 && testHeight > 0) {
                    tj3JPEGBufSize(testWidth, testHeight, subsamp);
                }
            }
            break;
    }

    // Cleanup
    cleanup_buffer(srcBuf);
    cleanup_buffer(jpegBuf);
    cleanup_handle(handle);

    return 0;
}