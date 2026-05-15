// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    int value = 0;
    if (*offset + sizeof(int) <= size) {
        memcpy(&value, data + *offset, sizeof(int));
        *offset += sizeof(int);
    }
    return value;
}

static size_t extract_size_t(const uint8_t *data, size_t size, size_t *offset) {
    size_t value = 0;
    if (*offset + sizeof(size_t) <= size) {
        memcpy(&value, data + *offset, sizeof(size_t));
        *offset += sizeof(size_t);
    }
    return value;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 4 * sizeof(int) + sizeof(size_t)) {
        return 0;
    }

    size_t offset = 0;
    
    int componentID = extract_int(Data, Size, &offset);
    int width = extract_int(Data, Size, &offset);
    int height = extract_int(Data, Size, &offset);
    int subsamp = extract_int(Data, Size, &offset);
    size_t stride = extract_size_t(Data, Size, &offset);
    
    int plane_width = tjPlaneWidth(componentID, width, subsamp);
    int plane_height = tjPlaneHeight(componentID, height, subsamp);
    
    int yuv_plane_width = tj3YUVPlaneWidth(componentID, width, subsamp);
    int yuv_plane_height = tj3YUVPlaneHeight(componentID, height, subsamp);
    
    size_t yuv_plane_size = tj3YUVPlaneSize(componentID, width, (int)stride, height, subsamp);
    unsigned long plane_size_yuv = tjPlaneSizeYUV(componentID, width, (int)stride, height, subsamp);
    
    (void)plane_width;
    (void)plane_height;
    (void)yuv_plane_width;
    (void)yuv_plane_height;
    (void)yuv_plane_size;
    (void)plane_size_yuv;
    
    return 0;
}