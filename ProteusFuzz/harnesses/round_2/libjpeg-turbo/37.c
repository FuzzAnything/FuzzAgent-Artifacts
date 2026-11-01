// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "turbojpeg.h"

static int fuzz_encode(const uint8_t *Data, size_t Size, int subsamp, int flags) {
    if (Size < 4) return 0;
    
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] & 0x03; // 0: RGB, 1: BGR, 2: RGBA, 3: BGRA
    
    // Limit dimensions to avoid excessive allocation
    if (width > 4096 || height > 4096) return 0;
    
    // Calculate required source buffer size
    int pitch = width * (pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR ? 3 : 4);
    size_t srcSize = (size_t)height * pitch;
    
    if (Size < srcSize) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    // Calculate plane dimensions
    int planeWidths[3];
    int planeHeights[3];
    int strides[3];
    
    for (int i = 0; i < 3; i++) {
        planeWidths[i] = tjPlaneWidth(i, width, subsamp);
        planeHeights[i] = tjPlaneHeight(i, height, subsamp);
        if (planeWidths[i] <= 0 || planeHeights[i] <= 0) {
            tjDestroy(handle);
            return 0;
        }
        strides[i] = planeWidths[i];
    }
    
    // Allocate destination planes
    unsigned char *planes[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3; i++) {
        size_t planeSize = (size_t)planeWidths[i] * planeHeights[i];
        planes[i] = (unsigned char *)malloc(planeSize);
        if (!planes[i]) {
            for (int j = 0; j < i; j++) free(planes[j]);
            tjDestroy(handle);
            return 0;
        }
    }
    
    // Call target function
    int result = tjEncodeYUVPlanes(handle, Data, width, pitch, height, pixelFormat, planes, strides, subsamp, flags);
    
    // Cleanup
    for (int i = 0; i < 3; i++) free(planes[i]);
    tjDestroy(handle);
    
    return result;
}

static int fuzz_decode(const uint8_t *Data, size_t Size, int subsamp, int flags) {
    if (Size < 4) return 0;
    
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] & 0x03;
    
    if (width > 4096 || height > 4096) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    // Calculate plane dimensions
    int planeWidths[3];
    int planeHeights[3];
    int strides[3];
    
    for (int i = 0; i < 3; i++) {
        planeWidths[i] = tjPlaneWidth(i, width, subsamp);
        planeHeights[i] = tjPlaneHeight(i, height, subsamp);
        if (planeWidths[i] <= 0 || planeHeights[i] <= 0) {
            tjDestroy(handle);
            return 0;
        }
        strides[i] = planeWidths[i];
    }
    
    // Allocate source planes
    const unsigned char *planes[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3; i++) {
        size_t planeSize = (size_t)planeWidths[i] * planeHeights[i];
        planes[i] = (const unsigned char *)malloc(planeSize);
        if (!planes[i]) {
            for (int j = 0; j < i; j++) free((void *)planes[j]);
            tjDestroy(handle);
            return 0;
        }
        // Fill with data from fuzz input if available
        size_t available = Size - 4;
        size_t toCopy = (available > planeSize) ? planeSize : available;
        if (toCopy > 0) {
            memcpy((void *)planes[i], Data + 4, toCopy);
        }
    }
    
    // Calculate destination buffer size
    int dstPitch = width * (pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR ? 3 : 4);
    size_t dstSize = (size_t)height * dstPitch;
    unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
    if (!dstBuf) {
        for (int i = 0; i < 3; i++) free((void *)planes[i]);
        tjDestroy(handle);
        return 0;
    }
    
    // Call target function
    int result = tjDecodeYUVPlanes(handle, planes, strides, subsamp, dstBuf, width, dstPitch, height, pixelFormat, flags);
    
    // Cleanup
    free(dstBuf);
    for (int i = 0; i < 3; i++) free((void *)planes[i]);
    tjDestroy(handle);
    
    return result;
}

static int fuzz_plane_functions(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    int componentID = Data[0] & 0x03; // 0, 1, or 2
    int width = Data[1] + 1;
    int height = Data[2] + 1;
    int subsamp = (Size > 3) ? (Data[3] & 0x0F) : 0; // Limit subsamp to 0-15
    
    // Test tjPlaneWidth
    int pw = tjPlaneWidth(componentID, width, subsamp);
    
    // Test tjPlaneHeight
    int ph = tjPlaneHeight(componentID, height, subsamp);
    
    // Test tj3YUVPlaneWidth
    int pw3 = tj3YUVPlaneWidth(componentID, width, subsamp);
    
    // Test tj3YUVPlaneHeight
    int ph3 = tj3YUVPlaneHeight(componentID, height, subsamp);
    
    // Return success if any function returned valid results
    return (pw >= 0 || ph >= 0 || pw3 >= 0 || ph3 >= 0) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    int mode = Data[0] & 0x03;
    
    switch (mode) {
        case 0: // Fuzz encode
            return fuzz_encode(Data + 1, Size - 1, (Size > 1) ? (Data[1] & 0x0F) : 0, (Size > 2) ? (Data[2] & 0xFF) : 0);
        case 1: // Fuzz decode
            return fuzz_decode(Data + 1, Size - 1, (Size > 1) ? (Data[1] & 0x0F) : 0, (Size > 2) ? (Data[2] & 0xFF) : 0);
        case 2: // Fuzz plane functions
            return fuzz_plane_functions(Data + 1, Size - 1);
        case 3: // Combined fuzzing
            fuzz_encode(Data + 1, Size - 1, (Size > 1) ? (Data[1] & 0x0F) : 0, (Size > 2) ? (Data[2] & 0xFF) : 0);
            fuzz_decode(Data + 1, Size - 1, (Size > 1) ? (Data[1] & 0x0F) : 0, (Size > 2) ? (Data[2] & 0xFF) : 0);
            return fuzz_plane_functions(Data + 1, Size - 1);
        default:
            return 0;
    }
}