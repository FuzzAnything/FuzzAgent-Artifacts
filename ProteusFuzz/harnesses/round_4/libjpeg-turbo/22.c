// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_tjBufSizeYUV(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    // Ensure we have enough bytes for 3 integers (12 bytes)
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    int subsamp = *(const int *)(Data + 8);
    
    // Clamp values to reasonable ranges to avoid overflow or extreme allocations
    if (width <= 0 || width > 8192) width = 64;
    if (height <= 0 || height > 8192) height = 64;
    if (subsamp < 0 || subsamp > 4) subsamp = 0;

    unsigned long size = tjBufSizeYUV(width, height, subsamp);
    if (size > 0 && size < (1ULL << 30)) {
        unsigned char *buf = malloc(size);
        if (buf) {
            memset(buf, 0, size);
            free(buf);
        }
    }
    return 0;
}

static int fuzz_tjDecodeYUV(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;
    
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    int subsamp = *(const int *)(Data + 8);
    int pixelFormat = *(const int *)(Data + 12);
    int flags = *(const int *)(Data + 16);
    int align = *(const int *)(Data + 20);

    if (width <= 0 || width > 4096) width = 64;
    if (height <= 0 || height > 4096) height = 64;
    if (subsamp < 0 || subsamp > 4) subsamp = 0;
    if (pixelFormat < 0 || pixelFormat > 5) pixelFormat = 0;
    if (align < 0 || align > 16) align = 1;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    unsigned long yuvSize = tjBufSizeYUV(width, height, subsamp);
    if (yuvSize == 0) {
        tjDestroy(handle);
        return 0;
    }
    
    unsigned char *srcBuf = malloc(yuvSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, yuvSize);

    int pitch = width * tjPixelSize[pixelFormat];
    unsigned long rgbSize = (unsigned long)height * pitch;
    unsigned char *dstBuf = malloc(rgbSize);
    if (!dstBuf) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }
    memset(dstBuf, 0, rgbSize);

    tjDecodeYUV(handle, srcBuf, align, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3DecodeYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;
    
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    int pixelFormat = *(const int *)(Data + 8);
    
    if (width <= 0 || width > 4096) width = 64;
    if (height <= 0 || height > 4096) height = 64;
    if (pixelFormat < 0 || pixelFormat > 5) pixelFormat = 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int subsamp = 0; // Default to 4:4:4 for simplicity in plane calculation
    int planeWidths[3];
    int planeHeights[3];
    
    planeWidths[0] = tj3YUVPlaneWidth(0, width, subsamp);
    planeWidths[1] = tj3YUVPlaneWidth(1, width, subsamp);
    planeWidths[2] = tj3YUVPlaneWidth(2, width, subsamp);
    planeHeights[0] = height;
    planeHeights[1] = height;
    planeHeights[2] = height;

    if (planeWidths[0] <= 0 || planeWidths[1] <= 0 || planeWidths[2] <= 0) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *planes[3];
    int strides[3];
    unsigned long planeSizes[3];
    
    planeSizes[0] = (unsigned long)planeWidths[0] * planeHeights[0];
    planeSizes[1] = (unsigned long)planeWidths[1] * planeHeights[1];
    planeSizes[2] = (unsigned long)planeWidths[2] * planeHeights[2];

    for (int i = 0; i < 3; i++) {
        planes[i] = malloc(planeSizes[i]);
        if (!planes[i]) {
            for (int j = 0; j < i; j++) free(planes[j]);
            tjDestroy(handle);
            return 0;
        }
        memset(planes[i], 0, planeSizes[i]);
        strides[i] = planeWidths[i];
    }

    int pitch = width * tjPixelSize[pixelFormat];
    unsigned long rgbSize = (unsigned long)height * pitch;
    unsigned char *dstBuf = malloc(rgbSize);
    if (!dstBuf) {
        for (int i = 0; i < 3; i++) free(planes[i]);
        tjDestroy(handle);
        return 0;
    }
    memset(dstBuf, 0, rgbSize);

    tj3DecodeYUVPlanes8(handle, (const unsigned char *const *)planes, strides, dstBuf, width, pitch, height, pixelFormat);

    for (int i = 0; i < 3; i++) free(planes[i]);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjDecompressToYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    
    if (width <= 0 || width > 4096) width = 64;
    if (height <= 0 || height > 4096) height = 64;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    // Create a dummy JPEG buffer (not a valid JPEG, but tests error handling)
    // We use a minimal valid JPEG header if possible, or just random data
    size_t jpegSize = Size > 8 ? Size - 8 : 1;
    unsigned char *jpegBuf = malloc(jpegSize);
    if (!jpegBuf) {
        tjDestroy(handle);
        return 0;
    }
    if (Size > 8) memcpy(jpegBuf, Data + 8, jpegSize);
    else jpegBuf[0] = 0xFF; // Minimal garbage

    unsigned char *dstPlanes[3];
    int strides[3] = {0, 0, 0};
    int subsamp = 0;

    int planeWidths[3];
    int planeHeights[3];
    planeWidths[0] = tj3YUVPlaneWidth(0, width, subsamp);
    planeWidths[1] = tj3YUVPlaneWidth(1, width, subsamp);
    planeWidths[2] = tj3YUVPlaneWidth(2, width, subsamp);
    planeHeights[0] = height;
    planeHeights[1] = height;
    planeHeights[2] = height;

    if (planeWidths[0] <= 0 || planeWidths[1] <= 0 || planeWidths[2] <= 0) {
        free(jpegBuf);
        tjDestroy(handle);
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        unsigned long size = (unsigned long)planeWidths[i] * planeHeights[i];
        dstPlanes[i] = malloc(size);
        if (!dstPlanes[i]) {
            for (int j = 0; j < i; j++) free(dstPlanes[j]);
            free(jpegBuf);
            tjDestroy(handle);
            return 0;
        }
        memset(dstPlanes[i], 0, size);
        strides[i] = planeWidths[i];
    }

    tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, dstPlanes, width, strides, height, 0);

    for (int i = 0; i < 3; i++) free(dstPlanes[i]);
    free(jpegBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjDecodeYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;
    
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    int subsamp = *(const int *)(Data + 8);
    int pixelFormat = *(const int *)(Data + 12);
    int flags = *(const int *)(Data + 16);

    if (width <= 0 || width > 4096) width = 64;
    if (height <= 0 || height > 4096) height = 64;
    if (subsamp < 0 || subsamp > 4) subsamp = 0;
    if (pixelFormat < 0 || pixelFormat > 5) pixelFormat = 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int planeWidths[3];
    int planeHeights[3];
    
    planeWidths[0] = tj3YUVPlaneWidth(0, width, subsamp);
    planeWidths[1] = tj3YUVPlaneWidth(1, width, subsamp);
    planeWidths[2] = tj3YUVPlaneWidth(2, width, subsamp);
    planeHeights[0] = height;
    planeHeights[1] = height;
    planeHeights[2] = height;

    if (planeWidths[0] <= 0 || planeWidths[1] <= 0 || planeWidths[2] <= 0) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *planes[3];
    int strides[3];
    unsigned long planeSizes[3];
    
    planeSizes[0] = (unsigned long)planeWidths[0] * planeHeights[0];
    planeSizes[1] = (unsigned long)planeWidths[1] * planeHeights[1];
    planeSizes[2] = (unsigned long)planeWidths[2] * planeHeights[2];

    for (int i = 0; i < 3; i++) {
        planes[i] = malloc(planeSizes[i]);
        if (!planes[i]) {
            for (int j = 0; j < i; j++) free(planes[j]);
            tjDestroy(handle);
            return 0;
        }
        memset(planes[i], 0, planeSizes[i]);
        strides[i] = planeWidths[i];
    }

    int pitch = width * tjPixelSize[pixelFormat];
    unsigned long rgbSize = (unsigned long)height * pitch;
    unsigned char *dstBuf = malloc(rgbSize);
    if (!dstBuf) {
        for (int i = 0; i < 3; i++) free(planes[i]);
        tjDestroy(handle);
        return 0;
    }
    memset(dstBuf, 0, rgbSize);

    tjDecodeYUVPlanes(handle, (const unsigned char **)planes, strides, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);

    for (int i = 0; i < 3; i++) free(planes[i]);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3YUVPlaneWidth(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    int componentID = *(const int *)(Data);
    int width = *(const int *)(Data + 4);
    int subsamp = *(const int *)(Data + 8);

    if (componentID < 0 || componentID > 2) componentID = 0;
    if (width <= 0 || width > 8192) width = 64;
    if (subsamp < 0 || subsamp > 4) subsamp = 0;

    int result = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)result; // Use result to suppress unused variable warning
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to select function
    int funcSelector = Data[0] % 6;
    
    // We need at least 1 byte for selector + minimum required bytes for the function
    // Minimums:
    // 0: 12 bytes (tjBufSizeYUV)
    // 1: 24 bytes (tjDecodeYUV)
    // 2: 24 bytes (tj3DecodeYUVPlanes8)
    // 3: 8 bytes (tjDecompressToYUVPlanes)
    // 4: 24 bytes (tjDecodeYUVPlanes)
    // 5: 12 bytes (tj3YUVPlaneWidth)
    
    size_t minSize = 0;
    switch (funcSelector) {
        case 0: minSize = 13; break; // 1 + 12
        case 1: minSize = 25; break; // 1 + 24
        case 2: minSize = 25; break; // 1 + 24
        case 3: minSize = 9; break;  // 1 + 8
        case 4: minSize = 25; break; // 1 + 24
        case 5: minSize = 13; break; // 1 + 12
    }
    
    if (Size < minSize) return 0;
    
    switch (funcSelector) {
        case 0:
            fuzz_tjBufSizeYUV(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_tjDecodeYUV(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_tj3DecodeYUVPlanes8(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_tjDecompressToYUVPlanes(Data + 1, Size - 1);
            break;
        case 4:
            fuzz_tjDecodeYUVPlanes(Data + 1, Size - 1);
            break;
        case 5:
            fuzz_tj3YUVPlaneWidth(Data + 1, Size - 1);
            break;
    }
    return 0;
}