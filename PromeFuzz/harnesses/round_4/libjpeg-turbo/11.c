// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>

static int get_subsamp(const uint8_t value) {
    const int subsamp_options[] = {TJSAMP_444, TJSAMP_422, TJSAMP_420, 
                                   TJSAMP_GRAY, TJSAMP_440, TJSAMP_411};
    return subsamp_options[value % 6];
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;
    
    // Extract parameters from fuzzer input
    int componentID = Data[0] % 3;  // 0, 1, or 2
    int width = abs((int)Data[1]) + 1;  // Ensure positive width
    int height = abs((int)Data[2]) + 1; // Ensure positive height
    int stride = abs((int)Data[3]);     // Can be zero
    int subsamp_val = Data[4] % 6;
    int subsamp = get_subsamp(subsamp_val);
    
    // Test tjPlaneWidth
    int result1 = tjPlaneWidth(componentID, width, subsamp);
    (void)result1; // Use result to avoid unused variable warning
    
    // Test tj3YUVPlaneWidth
    int result2 = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)result2;
    
    // Test tj3YUVPlaneHeight
    int result3 = tj3YUVPlaneHeight(componentID, height, subsamp);
    (void)result3;
    
    // Test tj3YUVPlaneSize
    size_t result4 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    (void)result4;
    
    // Test tjPlaneHeight
    int result5 = tjPlaneHeight(componentID, height, subsamp);
    (void)result5;
    
    // Test tjPlaneSizeYUV
    unsigned long result6 = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    (void)result6;
    
    // Test edge cases with different parameter combinations
    if (Size > 10) {
        // Test with extreme values
        tjPlaneWidth(componentID, 0, subsamp);
        tj3YUVPlaneWidth(componentID, 0, subsamp);
        tj3YUVPlaneHeight(componentID, 0, subsamp);
        tj3YUVPlaneSize(componentID, 0, 0, 0, subsamp);
        tjPlaneHeight(componentID, 0, subsamp);
        tjPlaneSizeYUV(componentID, 0, 0, 0, subsamp);
        
        // Test with large values
        int large_val = 10000;
        tjPlaneWidth(componentID, large_val, subsamp);
        tj3YUVPlaneWidth(componentID, large_val, subsamp);
        tj3YUVPlaneHeight(componentID, large_val, subsamp);
        tj3YUVPlaneSize(componentID, large_val, large_val, large_val, subsamp);
        tjPlaneHeight(componentID, large_val, subsamp);
        tjPlaneSizeYUV(componentID, large_val, large_val, large_val, subsamp);
    }
    
    return 0;
}