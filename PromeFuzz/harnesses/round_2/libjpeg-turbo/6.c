// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 14) return 0;
    
    // Extract parameters from fuzzer input
    int componentID = Data[0] % 3;  // 0, 1, or 2
    int width = *(int*)(Data + 1);
    int height = *(int*)(Data + 5);
    int stride = *(int*)(Data + 9);
    int subsamp = Data[13] % 6;  // 0-5 for TJSAMP values
    
    // Ensure non-negative values for width, height, stride
    if (width < 0) width = -width;
    if (height < 0) height = -height;
    if (stride < 0) stride = -stride;
    
    // Call tj3YUVPlaneWidth
    int planeWidth1 = tj3YUVPlaneWidth(componentID, width, subsamp);
    
    // Call tj3YUVPlaneSize
    size_t planeSize1 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    
    // Call tj3YUVPlaneHeight
    int planeHeight1 = tj3YUVPlaneHeight(componentID, height, subsamp);
    
    // Call tjPlaneWidth
    int planeWidth2 = tjPlaneWidth(componentID, width, subsamp);
    
    // Call tjPlaneSizeYUV
    unsigned long planeSize2 = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    
    // Call tjPlaneHeight
    int planeHeight2 = tjPlaneHeight(componentID, height, subsamp);
    
    // Additional calls with modified parameters to explore more states
    if (Size >= 20) {
        int componentID2 = Data[14] % 3;
        int width2 = *(int*)(Data + 15);
        if (width2 < 0) width2 = -width2;
        
        tj3YUVPlaneWidth(componentID2, width2, subsamp);
        tjPlaneWidth(componentID2, width2, subsamp);
    }
    
    return 0;
}