// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

static const int MAX_DIMENSION = 4096;
static const int MAX_SUBSAMP = 4;
static const int MAX_COMPONENT = 2;
static const int MAX_PIXEL_FORMAT = 11;

static void fuzz_tj3YUVPlaneHeight(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    int componentID = data[0] % 3;
    int height = (data[1] * 256 + data[2]) % (MAX_DIMENSION + 1);
    int subsamp = data[0] % (MAX_SUBSAMP + 1);
    
    tj3YUVPlaneHeight(componentID, height, subsamp);
}

static void fuzz_tj3YUVPlaneWidth(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    int componentID = data[0] % 3;
    int width = (data[1] * 256 + data[2]) % (MAX_DIMENSION + 1);
    int subsamp = data[0] % (MAX_SUBSAMP + 1);
    
    tj3YUVPlaneWidth(componentID, width, subsamp);
}

static void fuzz_tj3YUVPlaneSize(const uint8_t *data, size_t size) {
    if (size < 5) return;
    
    int componentID = data[0] % 3;
    int width = (data[1] * 256 + data[2]) % (MAX_DIMENSION + 1);
    int stride = (data[3] * 256 + data[4]) % (MAX_DIMENSION * 2 + 1);
    int height = (data[0] * 256 + data[1]) % (MAX_DIMENSION + 1);
    int subsamp = data[2] % (MAX_SUBSAMP + 1);
    
    tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
}

static void fuzz_tjEncodeYUVPlanes(const uint8_t *data, size_t size) {
    if (size < 20) return;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    int width = (data[0] * 256 + data[1]) % (MAX_DIMENSION + 1);
    int height = (data[2] * 256 + data[3]) % (MAX_DIMENSION + 1);
    int pitch = width * 3;
    int pixelFormat = data[4] % (MAX_PIXEL_FORMAT + 1);
    int subsamp = data[5] % (MAX_SUBSAMP + 1);
    int flags = data[6] % 256;
    
    if (width <= 0 || height <= 0) {
        tj3Destroy(handle);
        return;
    }
    
    size_t srcBufSize = (size_t)pitch * height;
    if (srcBufSize > size - 20) {
        tj3Destroy(handle);
        return;
    }
    
    const unsigned char *srcBuf = data + 20;
    
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    for (int i = 0; i < 3; i++) {
        int planeWidth = tj3YUVPlaneWidth(i, width, subsamp);
        int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
        if (planeWidth > 0 && planeHeight > 0) {
            dstPlanes[i] = (unsigned char *)malloc(planeWidth * planeHeight);
            if (dstPlanes[i]) {
                strides[i] = planeWidth;
            }
        }
    }
    
    tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides, subsamp, flags);
    
    for (int i = 0; i < 3; i++) {
        free(dstPlanes[i]);
    }
    
    tj3Destroy(handle);
}

static void fuzz_tj3DecompressToYUVPlanes8(const uint8_t *data, size_t size) {
    if (size < 10) return;
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return;
    
    const unsigned char *jpegBuf = data;
    size_t jpegSize = size;
    
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    int width = 64;
    int height = 64;
    int subsamp = TJSAMP_420;
    
    for (int i = 0; i < 3; i++) {
        int planeWidth = tj3YUVPlaneWidth(i, width, subsamp);
        int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
        if (planeWidth > 0 && planeHeight > 0) {
            dstPlanes[i] = (unsigned char *)malloc(planeWidth * planeHeight);
            if (dstPlanes[i]) {
                strides[i] = planeWidth;
            }
        }
    }
    
    tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, dstPlanes, strides);
    
    for (int i = 0; i < 3; i++) {
        free(dstPlanes[i]);
    }
    
    tj3Destroy(handle);
}

static void fuzz_tj3CompressFromYUVPlanes8(const uint8_t *data, size_t size) {
    if (size < 100) return;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    int width = (data[0] * 256 + data[1]) % 256 + 1;
    int height = (data[2] * 256 + data[3]) % 256 + 1;
    int subsamp = data[4] % (MAX_SUBSAMP + 1);
    
    unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    size_t totalPlaneSize = 0;
    
    for (int i = 0; i < 3; i++) {
        int planeWidth = tj3YUVPlaneWidth(i, width, subsamp);
        int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
        if (planeWidth > 0 && planeHeight > 0) {
            size_t planeSize = (size_t)planeWidth * planeHeight;
            totalPlaneSize += planeSize;
            if (totalPlaneSize > size - 5) {
                for (int j = 0; j <= i; j++) {
                    free(srcPlanes[j]);
                }
                tj3Destroy(handle);
                return;
            }
            srcPlanes[i] = (unsigned char *)malloc(planeSize);
            if (srcPlanes[i]) {
                memcpy(srcPlanes[i], data + 5 + (totalPlaneSize - planeSize), planeSize);
                strides[i] = planeWidth;
            }
        }
    }
    
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    if (totalPlaneSize > 0) {
        tj3CompressFromYUVPlanes8(handle, (const unsigned char *const *)srcPlanes, width, strides, height, &jpegBuf, &jpegSize);
        free(jpegBuf);
    }
    
    for (int i = 0; i < 3; i++) {
        free(srcPlanes[i]);
    }
    
    tj3Destroy(handle);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uint8_t selector = data[0];
    
    switch (selector % 5) {
        case 0:
            fuzz_tj3YUVPlaneHeight(data + 1, size - 1);
            break;
        case 1:
            fuzz_tj3YUVPlaneWidth(data + 1, size - 1);
            break;
        case 2:
            fuzz_tj3YUVPlaneSize(data + 1, size - 1);
            break;
        case 3:
            fuzz_tjEncodeYUVPlanes(data + 1, size - 1);
            break;
        case 4:
            if (selector % 2 == 0) {
                fuzz_tj3DecompressToYUVPlanes8(data + 1, size - 1);
            } else {
                fuzz_tj3CompressFromYUVPlanes8(data + 1, size - 1);
            }
            break;
    }
    
    return 0;
}