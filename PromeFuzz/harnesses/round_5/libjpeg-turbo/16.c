// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
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

static int get_subsampling(const uint8_t value) {
    const int subsamp_options[] = {
        TJSAMP_444,
        TJSAMP_422,
        TJSAMP_420,
        TJSAMP_GRAY,
        TJSAMP_440,
        TJSAMP_411
    };
    return subsamp_options[value % 6];
}

static int get_component_id(const uint8_t value) {
    return value % 3;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Extract parameters from fuzzer input
    int width = ((int)Data[0] << 8) | Data[1];
    int height = ((int)Data[2] << 8) | Data[3];
    int subsamp = get_subsampling(Data[4]);
    int componentID = get_component_id(Data[5]);
    int stride = ((int)Data[6] << 8) | Data[7];
    
    // Ensure non-negative values
    if (width < 0) width = -width;
    if (height < 0) height = -height;
    if (stride < 0) stride = -stride;
    
    // Cap values to reasonable bounds to avoid excessive memory calculations
    if (width > 16384) width = 16384;
    if (height > 16384) height = 16384;
    if (stride > 65536) stride = 65536;
    
    // Call target functions with volatile variables to prevent optimization
    volatile unsigned long ret1 = TJBUFSIZEYUV(width, height, subsamp);
    volatile int ret2 = tj3YUVPlaneWidth(componentID, width, subsamp);
    volatile unsigned long ret3 = tjBufSizeYUV(width, height, subsamp);
    volatile size_t ret4 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    volatile unsigned long ret5 = tjBufSize(width, height, subsamp);
    volatile unsigned long ret6 = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    
    // Use the results in a way that can't be optimized away
    if (ret1 == 0 || ret2 == 0 || ret3 == 0 || ret4 == 0 || ret5 == 0 || ret6 == (unsigned long)-1) {
        // This branch exists to use the results
    }
    
    return 0;
}