// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <turbojpeg.h>

static void free_planes(unsigned char **planes, int count) {
    for (int i = 0; i < count; i++) {
        if (planes[i]) {
            free(planes[i]);
            planes[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    // Extract parameters from input data safely
    int width = Data[0] * 16 + Data[1];
    int height = Data[2] * 16 + Data[3];
    
    // Clamp dimensions to reasonable limits to prevent allocation failures
    if (width > 4096 || height > 4096 || width <= 0 || height <= 0) {
        width = 64;
        height = 64;
    }

    int subsamp = (Data[4] % 4); // 0-3
    int pixelFormat = (Data[5] % 6); // 0-5
    int flags = (Data[6] % 4);
    int align = (Data[7] % 16);

    // Calculate plane sizes
    int strides[3] = {0, 0, 0};
    size_t planeSizes[3];
    
    planeSizes[0] = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    planeSizes[1] = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    planeSizes[2] = tj3YUVPlaneSize(2, width, 0, height, subsamp);

    if (planeSizes[0] == 0) {
        tjDestroy(handle);
        return 0;
    }

    // Allocate planes
    unsigned char *planes[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3; i++) {
        if (planeSizes[i] > 0) {
            planes[i] = (unsigned char *)malloc(planeSizes[i]);
            if (!planes[i]) {
                free_planes(planes, 3);
                tjDestroy(handle);
                return 0;
            }
            // Fill with pattern based on input data, ensuring we don't read out of bounds
            // Use modulo Size to safely index into Data
            uint8_t fillVal = 0;
            if (Size > 8 + i) {
                fillVal = Data[8 + i];
            } else {
                fillVal = (Data[i % Size] & 0xFF);
            }
            memset(planes[i], fillVal, planeSizes[i]);
        }
    }

    // Allocate destination buffer
    int pixelSize = tjPixelSize[pixelFormat];
    size_t dstSize = (size_t)width * pixelSize * height;
    unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
    if (!dstBuf) {
        free_planes(planes, 3);
        tjDestroy(handle);
        return 0;
    }

    // Test tjDecodeYUV
    if (Size > 12) {
        size_t totalPlaneSize = planeSizes[0] + planeSizes[1] + planeSizes[2];
        unsigned char *srcBuf = (unsigned char *)malloc(totalPlaneSize);
        if (srcBuf) {
            memcpy(srcBuf, planes[0], planeSizes[0]);
            if (planeSizes[1] > 0) memcpy(srcBuf + planeSizes[0], planes[1], planeSizes[1]);
            if (planeSizes[2] > 0) memcpy(srcBuf + planeSizes[0] + planeSizes[1], planes[2], planeSizes[2]);
            
            tjDecodeYUV(handle, srcBuf, align, subsamp, dstBuf, width, width * pixelSize, height, pixelFormat, flags);
            free(srcBuf);
        }
    }

    // Test tj3DecodeYUVPlanes8
    const unsigned char *const srcPlanes[3] = {planes[0], planes[1], planes[2]};
    const int testStrides[3] = {0, 0, 0};
    tj3DecodeYUVPlanes8(handle, srcPlanes, testStrides, dstBuf, width, width * pixelSize, height, pixelFormat);

    // Test tjDecodeYUVPlanes
    tjDecodeYUVPlanes(handle, srcPlanes, testStrides, subsamp, dstBuf, width, width * pixelSize, height, pixelFormat, flags);

    // Test tjDecompressToYUVPlanes (using input data as fake JPEG)
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int dstStrides[3] = {0, 0, 0};
    // Re-allocate destination planes for decompression test
    for (int i = 0; i < 3; i++) {
        if (planeSizes[i] > 0) {
            dstPlanes[i] = (unsigned char *)malloc(planeSizes[i]);
        }
    }
    
    // Use input data as JPEG buffer (even if invalid, tests error handling)
    tjDecompressToYUVPlanes(handle, Data, Size, dstPlanes, width, dstStrides, height, flags);

    // Test tj3YUVPlaneWidth
    for (int comp = 0; comp < 3; comp++) {
        tj3YUVPlaneWidth(comp, width, subsamp);
        // Test with edge cases
        tj3YUVPlaneWidth(comp, 0, subsamp);
        tj3YUVPlaneWidth(comp, width, -1);
    }

    // Test tj3YUVPlaneSize with various parameters
    for (int comp = 0; comp < 3; comp++) {
        tj3YUVPlaneSize(comp, width, 0, height, subsamp);
        tj3YUVPlaneSize(comp, 0, 0, height, subsamp);
        tj3YUVPlaneSize(comp, width, -1, height, subsamp);
        tj3YUVPlaneSize(comp, width, 0, -1, subsamp);
    }

    // Cleanup
    free(dstBuf);
    free_planes(planes, 3);
    free_planes(dstPlanes, 3);
    tjDestroy(handle);

    return 0;
}