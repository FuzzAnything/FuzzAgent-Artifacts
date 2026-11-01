// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
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

static int fuzz_encode(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4; // TJPF_RGB, TJPF_BGR, TJPF_GRAY, TJPF_CMYK
    int subsamp = Data[3] % 5; // TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY, TJSAMP_444
    int flags = Data[4] % 8;
    
    // Ensure reasonable bounds to avoid allocation failures or invalid states
    if (width > 4096 || height > 4096) return 0;
    if (width == 0 || height == 0) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    size_t srcSize = width * height * (pixelFormat == TJPF_GRAY ? 1 : 4);
    if (Size < srcSize + 10) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memcpy(srcBuf, Data + 10, srcSize);

    // Calculate plane dimensions
    int yWidth = tjPlaneWidth(0, width, subsamp);
    int yHeight = tjPlaneHeight(0, height, subsamp);
    int uWidth = tjPlaneWidth(1, width, subsamp);
    int uHeight = tjPlaneHeight(1, height, subsamp);
    int vWidth = tjPlaneWidth(2, width, subsamp);
    int vHeight = tjPlaneHeight(2, height, subsamp);

    if (yWidth <= 0 || yHeight <= 0 || uWidth <= 0 || uHeight <= 0 || vWidth <= 0 || vHeight <= 0) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    size_t ySize = (size_t)yWidth * yHeight;
    size_t uSize = (size_t)uWidth * uHeight;
    size_t vSize = (size_t)vWidth * vHeight;

    unsigned char *yPlane = (unsigned char *)malloc(ySize);
    unsigned char *uPlane = (unsigned char *)malloc(uSize);
    unsigned char *vPlane = (unsigned char *)malloc(vSize);

    if (!yPlane || !uPlane || !vPlane) {
        free(srcBuf);
        free(yPlane);
        free(uPlane);
        free(vPlane);
        tjDestroy(handle);
        return 0;
    }

    unsigned char *dstPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {yWidth, uWidth, vWidth};

    int result = tjEncodeYUVPlanes(handle, srcBuf, width, width * (pixelFormat == TJPF_GRAY ? 1 : 4), height, pixelFormat, dstPlanes, strides, subsamp, flags);

    free(srcBuf);
    free(yPlane);
    free(uPlane);
    free(vPlane);
    tjDestroy(handle);

    return result;
}

static int fuzz_decode(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4;
    int subsamp = Data[3] % 5;
    int flags = Data[4] % 8;

    if (width > 4096 || height > 4096) return 0;
    if (width == 0 || height == 0) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int yWidth = tjPlaneWidth(0, width, subsamp);
    int yHeight = tjPlaneHeight(0, height, subsamp);
    int uWidth = tjPlaneWidth(1, width, subsamp);
    int uHeight = tjPlaneHeight(1, height, subsamp);
    int vWidth = tjPlaneWidth(2, width, subsamp);
    int vHeight = tjPlaneHeight(2, height, subsamp);

    if (yWidth <= 0 || yHeight <= 0 || uWidth <= 0 || uHeight <= 0 || vWidth <= 0 || vHeight <= 0) {
        tjDestroy(handle);
        return 0;
    }

    size_t ySize = (size_t)yWidth * yHeight;
    size_t uSize = (size_t)uWidth * uHeight;
    size_t vSize = (size_t)vWidth * vHeight;

    unsigned char *yPlane = (unsigned char *)malloc(ySize);
    unsigned char *uPlane = (unsigned char *)malloc(uSize);
    unsigned char *vPlane = (unsigned char *)malloc(vSize);

    if (!yPlane || !uPlane || !vPlane) {
        free(yPlane);
        free(uPlane);
        free(vPlane);
        tjDestroy(handle);
        return 0;
    }

    // Fill planes with fuzz data
    size_t totalPlaneSize = ySize + uSize + vSize;
    if (Size < 10 + totalPlaneSize) {
        free(yPlane);
        free(uPlane);
        free(vPlane);
        tjDestroy(handle);
        return 0;
    }

    memcpy(yPlane, Data + 10, ySize);
    memcpy(uPlane, Data + 10 + ySize, uSize);
    memcpy(vPlane, Data + 10 + ySize + uSize, vSize);

    unsigned char *dstBuf = (unsigned char *)malloc(width * height * (pixelFormat == TJPF_GRAY ? 1 : 4));
    if (!dstBuf) {
        free(yPlane);
        free(uPlane);
        free(vPlane);
        tjDestroy(handle);
        return 0;
    }

    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {yWidth, uWidth, vWidth};
    int pitch = width * (pixelFormat == TJPF_GRAY ? 1 : 4);

    int result = tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);

    free(yPlane);
    free(uPlane);
    free(vPlane);
    free(dstBuf);
    tjDestroy(handle);

    return result;
}

static int fuzz_plane_calc(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    int componentID = Data[0] % 3;
    int width = Data[1] * 100 + 1;
    int subsamp = Data[2] % 5;

    // Test tj3YUVPlaneWidth
    int w1 = tj3YUVPlaneWidth(componentID, width, subsamp);
    int w2 = tjPlaneWidth(componentID, width, subsamp);

    // Test tj3YUVPlaneHeight
    int height = Data[1] * 100 + 1;
    int h1 = tj3YUVPlaneHeight(componentID, height, subsamp);
    int h2 = tjPlaneHeight(componentID, height, subsamp);

    // Trigger potential edge cases with large values
    if (Size > 4) {
        int largeWidth = (Data[3] << 8) | Data[4];
        if (largeWidth > 0) {
            tj3YUVPlaneWidth(componentID, largeWidth, subsamp);
            tjPlaneWidth(componentID, largeWidth, subsamp);
            tj3YUVPlaneHeight(componentID, largeWidth, subsamp);
            tjPlaneHeight(componentID, largeWidth, subsamp);
        }
    }

    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int mode = Data[0] % 3;

    switch (mode) {
        case 0:
            fuzz_encode(Data, Size);
            break;
        case 1:
            fuzz_decode(Data, Size);
            break;
        case 2:
            fuzz_plane_calc(Data, Size);
            break;
    }

    return 0;
}