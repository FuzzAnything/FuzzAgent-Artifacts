// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Parse parameters from fuzzer input
    // Use first byte for componentID (0-2) and subsampling (0-5)
    int componentID = Data[0] % 3;
    int subsamp = Data[0] % 6;
    
    // Use next bytes for width, height, stride (if available)
    int width = 0, height = 0, stride = 0;
    
    if (Size >= 5) {
        width = *(int*)(Data + 1);
        if (width < 0) width = -width;
    }
    
    if (Size >= 9) {
        height = *(int*)(Data + 5);
        if (height < 0) height = -height;
    }
    
    if (Size >= 13) {
        stride = *(int*)(Data + 9);
        if (stride < 0) stride = -stride;
    }

    // Test tj3YUVPlaneWidth
    int planeWidth = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)planeWidth; // Use result to avoid compiler warnings

    // Test tj3YUVPlaneSize
    size_t planeSize = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    (void)planeSize;

    // Test tj3YUVPlaneHeight
    int planeHeight = tj3YUVPlaneHeight(componentID, height, subsamp);
    (void)planeHeight;

    // Test tjBufSizeYUV
    unsigned long bufSizeYUV = tjBufSizeYUV(width, height, subsamp);
    (void)bufSizeYUV;

    // Test tjPlaneSizeYUV
    unsigned long planeSizeYUV = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    (void)planeSizeYUV;

    // Test TJBUFSIZEYUV macro
    unsigned long TJBufSizeYUV = TJBUFSIZEYUV(width, height, subsamp);
    (void)TJBufSizeYUV;

    // Test with different combinations of parameters
    // Test with zero values
    tj3YUVPlaneWidth(0, 0, 0);
    tj3YUVPlaneSize(0, 0, 0, 0, 0);
    tj3YUVPlaneHeight(0, 0, 0);
    tjBufSizeYUV(0, 0, 0);
    tjPlaneSizeYUV(0, 0, 0, 0, 0);
    TJBUFSIZEYUV(0, 0, 0);

    // Test with maximum values (using modulo to keep within reasonable bounds)
    int maxComponentID = 2;
    int maxSubsamp = 5;
    int maxWidth = width % 4096;
    int maxHeight = height % 4096;
    int maxStride = stride % 8192;
    
    tj3YUVPlaneWidth(maxComponentID, maxWidth, maxSubsamp);
    tj3YUVPlaneSize(maxComponentID, maxWidth, maxStride, maxHeight, maxSubsamp);
    tj3YUVPlaneHeight(maxComponentID, maxHeight, maxSubsamp);
    tjBufSizeYUV(maxWidth, maxHeight, maxSubsamp);
    tjPlaneSizeYUV(maxComponentID, maxWidth, maxStride, maxHeight, maxSubsamp);
    TJBUFSIZEYUV(maxWidth, maxHeight, maxSubsamp);

    // Test with negative values (should be handled by library)
    tj3YUVPlaneWidth(-1, -1, -1);
    tj3YUVPlaneSize(-1, -1, -1, -1, -1);
    tj3YUVPlaneHeight(-1, -1, -1);
    tjBufSizeYUV(-1, -1, -1);
    tjPlaneSizeYUV(-1, -1, -1, -1, -1);
    TJBUFSIZEYUV(-1, -1, -1);

    // Test with edge cases for stride (0 means plane width)
    tj3YUVPlaneSize(componentID, width, 0, height, subsamp);

    return 0;
}