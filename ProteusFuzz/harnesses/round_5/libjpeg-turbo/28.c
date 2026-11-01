// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <turbojpeg.h>

static int fuzz_tj3YUVPlaneWidth(const uint8_t *Data, size_t Size) {
    int componentID, width, subsamp;
    if (Size < 12) return 0;
    memcpy(&componentID, Data, sizeof(int));
    memcpy(&width, Data + sizeof(int), sizeof(int));
    memcpy(&subsamp, Data + 2 * sizeof(int), sizeof(int));
    return tj3YUVPlaneWidth(componentID, width, subsamp);
}

static int fuzz_tj3YUVPlaneHeight(const uint8_t *Data, size_t Size) {
    int componentID, height, subsamp;
    if (Size < 12) return 0;
    memcpy(&componentID, Data, sizeof(int));
    memcpy(&height, Data + sizeof(int), sizeof(int));
    memcpy(&subsamp, Data + 2 * sizeof(int), sizeof(int));
    return tj3YUVPlaneHeight(componentID, height, subsamp);
}

static int fuzz_tjPlaneWidth(const uint8_t *Data, size_t Size) {
    int componentID, width, subsamp;
    if (Size < 12) return 0;
    memcpy(&componentID, Data, sizeof(int));
    memcpy(&width, Data + sizeof(int), sizeof(int));
    memcpy(&subsamp, Data + 2 * sizeof(int), sizeof(int));
    return tjPlaneWidth(componentID, width, subsamp);
}

static int fuzz_tjPlaneHeight(const uint8_t *Data, size_t Size) {
    int componentID, height, subsamp;
    if (Size < 12) return 0;
    memcpy(&componentID, Data, sizeof(int));
    memcpy(&height, Data + sizeof(int), sizeof(int));
    memcpy(&subsamp, Data + 2 * sizeof(int), sizeof(int));
    return tjPlaneHeight(componentID, height, subsamp);
}

static int fuzz_tjEncodeYUVPlanes(const uint8_t *Data, size_t Size) {
    tjhandle handle;
    int width, height, pixelFormat, subsamp, flags;
    unsigned char *srcBuf = NULL;
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    int pitch;

    if (Size < 32) return 0;

    memcpy(&width, Data, sizeof(int));
    memcpy(&height, Data + sizeof(int), sizeof(int));
    memcpy(&pixelFormat, Data + 2 * sizeof(int), sizeof(int));
    memcpy(&subsamp, Data + 3 * sizeof(int), sizeof(int));
    memcpy(&flags, Data + 4 * sizeof(int), sizeof(int));
    memcpy(&pitch, Data + 5 * sizeof(int), sizeof(int));

    if (width <= 0 || height <= 0) return 0;

    handle = tjInitCompress();
    if (!handle) return 0;

    size_t srcSize = (size_t)width * pitch;
    if (srcSize > 1024 * 1024 * 10) srcSize = 1024 * 1024 * 10;
    
    size_t availableData = Size - 6 * sizeof(int);
    if (srcSize > availableData) srcSize = availableData;
    
    if (srcSize == 0) {
        tjDestroy(handle);
        return 0;
    }

    srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memcpy(srcBuf, Data + 6 * sizeof(int), srcSize);

    int planeWidths[3], planeHeights[3];
    planeWidths[0] = tjPlaneWidth(0, width, subsamp);
    planeWidths[1] = tjPlaneWidth(1, width, subsamp);
    planeWidths[2] = tjPlaneWidth(2, width, subsamp);
    planeHeights[0] = tjPlaneHeight(0, height, subsamp);
    planeHeights[1] = tjPlaneHeight(1, height, subsamp);
    planeHeights[2] = tjPlaneHeight(2, height, subsamp);

    for (int i = 0; i < 3; i++) {
        if (planeWidths[i] <= 0 || planeHeights[i] <= 0) continue;
        size_t planeSize = (size_t)planeWidths[i] * planeHeights[i];
        if (planeSize > 1024 * 1024 * 10) planeSize = 1024 * 1024 * 10;
        dstPlanes[i] = malloc(planeSize);
        if (dstPlanes[i]) {
            strides[i] = planeWidths[i];
        }
    }

    int ret = tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides, subsamp, flags);

    free(srcBuf);
    for (int i = 0; i < 3; i++) free(dstPlanes[i]);
    tjDestroy(handle);

    return ret;
}

static int fuzz_tjEncodeYUV3(const uint8_t *Data, size_t Size) {
    tjhandle handle;
    int width, height, pixelFormat, subsamp, flags, align;
    unsigned char *srcBuf = NULL;
    unsigned char *dstBuf = NULL;

    if (Size < 32) return 0;

    memcpy(&width, Data, sizeof(int));
    memcpy(&height, Data + sizeof(int), sizeof(int));
    memcpy(&pixelFormat, Data + 2 * sizeof(int), sizeof(int));
    memcpy(&subsamp, Data + 3 * sizeof(int), sizeof(int));
    memcpy(&flags, Data + 4 * sizeof(int), sizeof(int));
    memcpy(&align, Data + 5 * sizeof(int), sizeof(int));

    if (width <= 0 || height <= 0) return 0;

    handle = tjInitCompress();
    if (!handle) return 0;

    size_t srcSize = (size_t)width * height * 3;
    if (srcSize > 1024 * 1024 * 10) srcSize = 1024 * 1024 * 10;
    
    size_t availableData = Size - 6 * sizeof(int);
    if (srcSize > availableData) srcSize = availableData;

    if (srcSize == 0) {
        tjDestroy(handle);
        return 0;
    }

    srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memcpy(srcBuf, Data + 6 * sizeof(int), srcSize);

    int planeWidth = tjPlaneWidth(0, width, subsamp);
    int planeHeight = tjPlaneHeight(0, height, subsamp);
    if (planeWidth <= 0 || planeHeight <= 0) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }
    size_t dstSize = (size_t)planeWidth * planeHeight * 3;
    if (dstSize > 1024 * 1024 * 10) dstSize = 1024 * 1024 * 10;
    dstBuf = malloc(dstSize);
    if (!dstBuf) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    int ret = tjEncodeYUV3(handle, srcBuf, width, width * 3, height, pixelFormat, dstBuf, align, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);

    return ret;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int mode = Data[0] % 6;

    switch (mode) {
        case 0:
            fuzz_tj3YUVPlaneWidth(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_tj3YUVPlaneHeight(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_tjPlaneWidth(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_tjPlaneHeight(Data + 1, Size - 1);
            break;
        case 4:
            fuzz_tjEncodeYUVPlanes(Data + 1, Size - 1);
            break;
        case 5:
            fuzz_tjEncodeYUV3(Data + 1, Size - 1);
            break;
        default:
            break;
    }

    return 0;
}