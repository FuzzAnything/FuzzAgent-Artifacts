// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <turbojpeg.h>

static int fuzz_tj3YUVPlaneHeight(const uint8_t *data, size_t size) {
    int componentID = 0, height = 0, subsamp = 0;
    if (size >= 4) {
        componentID = data[0];
        height = (data[1] << 8) | data[2];
        subsamp = data[3];
    }
    return tj3YUVPlaneHeight(componentID, height, subsamp);
}

static int fuzz_tjPlaneWidth(const uint8_t *data, size_t size) {
    int componentID = 0, width = 0, subsamp = 0;
    if (size >= 4) {
        componentID = data[0];
        width = (data[1] << 8) | data[2];
        subsamp = data[3];
    }
    return tjPlaneWidth(componentID, width, subsamp);
}

static int fuzz_tjPlaneHeight(const uint8_t *data, size_t size) {
    int componentID = 0, height = 0, subsamp = 0;
    if (size >= 4) {
        componentID = data[0];
        height = (data[1] << 8) | data[2];
        subsamp = data[3];
    }
    return tjPlaneHeight(componentID, height, subsamp);
}

static int fuzz_tj3YUVPlaneWidth(const uint8_t *data, size_t size) {
    int componentID = 0, width = 0, subsamp = 0;
    if (size >= 4) {
        componentID = data[0];
        width = (data[1] << 8) | data[2];
        subsamp = data[3];
    }
    return tj3YUVPlaneWidth(componentID, width, subsamp);
}

static int fuzz_tjDecodeYUVPlanes(const uint8_t *data, size_t size) {
    if (size < 10) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int subsamp = data[4];
    int pixelFormat = data[5];
    int flags = (data[6] << 8) | data[7];
    
    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }
    
    int planeWidths[3], planeHeights[3];
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    for (int i = 0; i < 3; i++) {
        planeWidths[i] = tjPlaneWidth(i, width, subsamp);
        planeHeights[i] = tjPlaneHeight(i, height, subsamp);
        if (planeWidths[i] <= 0 || planeHeights[i] <= 0) {
            tjDestroy(handle);
            return 0;
        }
        planes[i] = malloc(planeWidths[i] * planeHeights[i]);
        if (!planes[i]) {
            for (int j = 0; j < i; j++) free(planes[j]);
            tjDestroy(handle);
            return 0;
        }
        strides[i] = planeWidths[i];
    }
    
    int pitch = width * 3;
    size_t dstSize = pitch * height;
    unsigned char *dstBuf = malloc(dstSize);
    if (!dstBuf) {
        for (int i = 0; i < 3; i++) free(planes[i]);
        tjDestroy(handle);
        return 0;
    }
    
    int result = tjDecodeYUVPlanes(handle, (const unsigned char **)planes, strides, subsamp,
                                   dstBuf, width, pitch, height, pixelFormat, flags);
    
    free(dstBuf);
    for (int i = 0; i < 3; i++) free(planes[i]);
    tjDestroy(handle);
    
    return result;
}

static int fuzz_tjEncodeYUVPlanes(const uint8_t *data, size_t size) {
    if (size < 10) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int subsamp = data[4];
    int pixelFormat = data[5];
    int flags = (data[6] << 8) | data[7];
    
    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }
    
    int pitch = width * 3;
    size_t srcSize = pitch * height;
    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, srcSize);
    
    int planeWidths[3], planeHeights[3];
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    for (int i = 0; i < 3; i++) {
        planeWidths[i] = tjPlaneWidth(i, width, subsamp);
        planeHeights[i] = tjPlaneHeight(i, height, subsamp);
        if (planeWidths[i] <= 0 || planeHeights[i] <= 0) {
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        planes[i] = malloc(planeWidths[i] * planeHeights[i]);
        if (!planes[i]) {
            for (int j = 0; j < i; j++) free(planes[j]);
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        strides[i] = planeWidths[i];
    }
    
    int result = tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat,
                                   planes, strides, subsamp, flags);
    
    free(srcBuf);
    for (int i = 0; i < 3; i++) free(planes[i]);
    tjDestroy(handle);
    
    return result;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    int op = Data[0] % 6;
    
    switch (op) {
        case 0:
            fuzz_tj3YUVPlaneHeight(Data, Size);
            break;
        case 1:
            fuzz_tjPlaneWidth(Data, Size);
            break;
        case 2:
            fuzz_tjPlaneHeight(Data, Size);
            break;
        case 3:
            fuzz_tj3YUVPlaneWidth(Data, Size);
            break;
        case 4:
            fuzz_tjDecodeYUVPlanes(Data, Size);
            break;
        case 5:
            fuzz_tjEncodeYUVPlanes(Data, Size);
            break;
    }
    
    return 0;
}