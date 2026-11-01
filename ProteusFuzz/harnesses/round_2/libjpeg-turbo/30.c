// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void *safe_malloc(size_t size) {
    if (size == 0 || size > (1024 * 1024 * 1024)) return NULL;
    void *ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Extract parameters from fuzz input
    uint32_t width = (Data[0] << 8) | Data[1];
    uint32_t height = (Data[2] << 8) | Data[3];
    int pixelFormat = Data[4] % 5; // 0-4: TJPF_GRAY, TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX
    int subsamp = Data[5] % 4;     // 0-3: TJCS_GRAY, TJCS_RGB, etc. mapped to subsamp
    int align = 1 << (Data[6] % 5); // 1, 2, 4, 8, 16
    int flags = Data[7];
    
    // Clamp dimensions to reasonable limits to prevent OOM
    if (width == 0 || width > 2048) width = 100;
    if (height == 0 || height > 2048) height = 100;

    // Calculate source buffer size
    int pixelSize = tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)width * pixelSize * height;
    if (srcSize == 0 || srcSize > (1024 * 1024 * 50)) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = safe_malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }

    // Fill source buffer with fuzz data (padded if necessary)
    size_t copyLen = (Size - 8) < srcSize ? (Size - 8) : srcSize;
    if (copyLen > 0) {
        memcpy(srcBuf, Data + 8, copyLen);
    }
    // Fill rest with pattern to ensure non-zero data
    for (size_t i = copyLen; i < srcSize; i++) {
        srcBuf[i] = (i % 256);
    }

    int pitch = width * pixelSize;
    if (Data[8] & 1) pitch = 0; // Let API calculate
    if (Data[8] & 2) pitch = width * pixelSize + (Data[9] % 16); // Add padding

    // Calculate destination buffer sizes
    size_t dstBufSize = tjBufSize(width, height, subsamp);
    unsigned char *dstBuf = safe_malloc(dstBufSize);

    // Prepare plane buffers for planar functions
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    size_t planeSizes[3];
    
    // Calculate plane sizes manually based on subsampling
    int hSubsamp = 1, vSubsamp = 1;
    if (subsamp == 1) { hSubsamp = 2; vSubsamp = 2; } // 4:2:0
    else if (subsamp == 2) { hSubsamp = 2; vSubsamp = 1; } // 4:2:2
    else if (subsamp == 3) { hSubsamp = 1; vSubsamp = 2; } // 4:4:0

    int planeWidth = width;
    int planeHeight = height;
    int uWidth = width / hSubsamp;
    int uHeight = height / vSubsamp;

    // Y plane
    planeSizes[0] = (size_t)planeWidth * planeHeight;
    planes[0] = safe_malloc(planeSizes[0]);
    strides[0] = planeWidth;

    // U plane
    if (subsamp != 0) { // Not grayscale
        planeSizes[1] = (size_t)uWidth * uHeight;
        planes[1] = safe_malloc(planeSizes[1]);
        strides[1] = uWidth;
    }
    // V plane
    if (subsamp != 0) {
        planeSizes[2] = (size_t)uWidth * uHeight;
        planes[2] = safe_malloc(planeSizes[2]);
        strides[2] = uWidth;
    }

    // 1. tjEncodeYUVPlanes
    if (planes[0]) {
        tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat, planes, strides, subsamp, flags);
    }

    // 2. tj3EncodeYUV8
    if (dstBuf) {
        tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);
    }

    // 3. tjEncodeYUV3
    if (dstBuf) {
        tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align, subsamp, flags);
    }

    // 4. tjEncodeYUV
    if (dstBuf) {
        tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, subsamp, flags);
    }

    // 5. tj3EncodeYUVPlanes8
    if (planes[0]) {
        tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height, pixelFormat, planes, strides);
    }

    // 6. tjEncodeYUV2
    if (dstBuf) {
        tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);
    }

    // Cleanup
    free(srcBuf);
    free(dstBuf);
    if (planes[0]) free(planes[0]);
    if (planes[1]) free(planes[1]);
    if (planes[2]) free(planes[2]);
    tjDestroy(handle);

    return 0;
}