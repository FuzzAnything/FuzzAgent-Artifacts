// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int readInt(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        *size = 0;
        return 0;
    }
    int value = *(const int *)(*data);
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static int readByte(const uint8_t **data, size_t *size) {
    if (*size == 0) {
        return 0;
    }
    int value = **data;
    *data += 1;
    *size -= 1;
    return value;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    const uint8_t *data_ptr = Data;
    size_t data_size = Size;

    int componentID = readByte(&data_ptr, &data_size) % 3;
    int width = readInt(&data_ptr, &data_size);
    int height = readInt(&data_ptr, &data_size);
    int stride = readInt(&data_ptr, &data_size);
    int subsamp = readByte(&data_ptr, &data_size) % 6;

    tjPlaneHeight(componentID, height, subsamp);
    tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    tjPlaneWidth(componentID, width, subsamp);
    tj3YUVPlaneWidth(componentID, width, subsamp);
    tj3YUVPlaneHeight(componentID, height, subsamp);

    return 0;
}