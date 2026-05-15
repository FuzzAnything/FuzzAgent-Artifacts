// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_subsampling(int value) {
    /* Map fuzzed byte to valid subsampling values */
    switch (value % 6) {
        case 0: return TJSAMP_444;
        case 1: return TJSAMP_422;
        case 2: return TJSAMP_420;
        case 3: return TJSAMP_GRAY;
        case 4: return TJSAMP_440;
        case 5: return TJSAMP_411;
        default: return TJSAMP_444;
    }
}

static int get_component_id(int value) {
    /* Map fuzzed byte to component IDs (0,1,2) */
    return value % 3;
}

static int get_alignment(int value) {
    /* Map fuzzed byte to valid alignment values (1, 2, 4, 8) */
    switch (value % 4) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        default: return 4;
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    /* Extract parameters from fuzzer input */
    int component_id = get_component_id(Data[0]);
    int width = (int)((Data[1] << 8) | Data[2]) % 4096;
    int height = (int)((Data[3] << 8) | Data[4]) % 4096;
    int subsamp = get_subsampling(Data[5]);
    int align = get_alignment(Data[6]);
    int stride = (int)((Data[7] << 8) | Data[8]) % 4096;
    
    /* Ensure non-zero dimensions for meaningful testing */
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    if (stride <= 0) stride = width;

    /* Test tj3YUVPlaneWidth */
    int plane_width = tj3YUVPlaneWidth(component_id, width, subsamp);
    (void)plane_width; /* Use result to avoid unused variable warning */

    /* Test tjPlaneSizeYUV */
    unsigned long plane_size = tjPlaneSizeYUV(component_id, width, stride, height, subsamp);
    (void)plane_size;

    /* Test tj3YUVPlaneHeight */
    int plane_height = tj3YUVPlaneHeight(component_id, height, subsamp);
    (void)plane_height;

    /* Test tjBufSizeYUV2 */
    unsigned long buf_size = tjBufSizeYUV2(width, align, height, subsamp);
    (void)buf_size;

    /* Test tjPlaneWidth */
    int old_plane_width = tjPlaneWidth(component_id, width, subsamp);
    (void)old_plane_width;

    /* Test tjPlaneHeight */
    int old_plane_height = tjPlaneHeight(component_id, height, subsamp);
    (void)old_plane_height;

    /* Test with edge cases */
    if (Size > 20) {
        /* Test with invalid component IDs */
        tj3YUVPlaneWidth(Data[9] % 10, width, subsamp);
        tjPlaneSizeYUV(Data[10] % 10, width, stride, height, subsamp);
        tj3YUVPlaneHeight(Data[11] % 10, height, subsamp);
        
        /* Test with invalid subsampling */
        tj3YUVPlaneWidth(component_id, width, Data[12] % 10);
        tjPlaneSizeYUV(component_id, width, stride, height, Data[13] % 10);
        tj3YUVPlaneHeight(component_id, height, Data[14] % 10);
        
        /* Test with zero or negative dimensions */
        tj3YUVPlaneWidth(component_id, 0, subsamp);
        tjPlaneSizeYUV(component_id, 0, 0, 0, subsamp);
        tj3YUVPlaneHeight(component_id, 0, subsamp);
        tjBufSizeYUV2(0, align, 0, subsamp);
    }

    return 0;
}