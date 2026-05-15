// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    // Parse input data to get parameters
    // We'll use the first 20 bytes for 5 integers (4 bytes each)
    int componentID = *(int*)Data;
    int width = *(int*)(Data + 4);
    int height = *(int*)(Data + 8);
    int stride = *(int*)(Data + 12);
    int subsamp = *(int*)(Data + 16);

    // Test tj3YUVPlaneWidth
    int planeWidth = tj3YUVPlaneWidth(componentID, width, subsamp);
    
    // Test tj3YUVPlaneHeight
    int planeHeight = tj3YUVPlaneHeight(componentID, height, subsamp);
    
    // Test tj3YUVPlaneSize
    size_t planeSize = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    
    // Test TJBUFSIZEYUV macro
    unsigned long bufSizeMacro = TJBUFSIZEYUV(width, height, subsamp);
    
    // Test tjBufSizeYUV
    unsigned long bufSize = tjBufSizeYUV(width, height, subsamp);
    
    // Test tjPlaneSizeYUV
    long planeSizeOld = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);

    // Try different combinations to explore more states
    // Use modulo operations to get valid ranges for testing
    for (int i = 0; i < 3; i++) {
        int testCompID = (componentID + i) % 3;
        int testWidth = abs(width + i * 10);
        int testHeight = abs(height + i * 10);
        int testStride = abs(stride + i * 4);
        int testSubsamp = abs(subsamp + i) % 5;  // TJSAMP_* values are 0-4
        
        tj3YUVPlaneWidth(testCompID, testWidth, testSubsamp);
        tj3YUVPlaneHeight(testCompID, testHeight, testSubsamp);
        tj3YUVPlaneSize(testCompID, testWidth, testStride, testHeight, testSubsamp);
        TJBUFSIZEYUV(testWidth, testHeight, testSubsamp);
        tjBufSizeYUV(testWidth, testHeight, testSubsamp);
        tjPlaneSizeYUV(testCompID, testWidth, testStride, testHeight, testSubsamp);
    }

    // Test edge cases
    tj3YUVPlaneWidth(0, 0, 0);
    tj3YUVPlaneHeight(0, 0, 0);
    tj3YUVPlaneSize(0, 0, 0, 0, 0);
    TJBUFSIZEYUV(0, 0, 0);
    tjBufSizeYUV(0, 0, 0);
    tjPlaneSizeYUV(0, 0, 0, 0, 0);

    // Test negative values
    tj3YUVPlaneWidth(-1, width, subsamp);
    tj3YUVPlaneHeight(-1, height, subsamp);
    tj3YUVPlaneSize(-1, width, stride, height, subsamp);
    tjPlaneSizeYUV(-1, width, stride, height, subsamp);

    // Test large values
    tj3YUVPlaneWidth(2, 10000, 2);
    tj3YUVPlaneHeight(2, 10000, 2);
    tj3YUVPlaneSize(2, 10000, 10000, 10000, 2);
    TJBUFSIZEYUV(10000, 10000, 2);
    tjBufSizeYUV(10000, 10000, 2);
    tjPlaneSizeYUV(2, 10000, 10000, 10000, 2);

    return 0;
}