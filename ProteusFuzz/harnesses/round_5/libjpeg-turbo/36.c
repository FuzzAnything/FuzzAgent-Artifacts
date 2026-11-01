// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_resources(tjhandle handle, unsigned char *srcBuf, unsigned char *dstBuf) {
    if (handle) {
        tjDestroy(handle);
    }
    if (srcBuf) {
        free(srcBuf);
    }
    if (dstBuf) {
        tjFree(dstBuf);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    tjhandle handle = NULL;
    unsigned char *srcBuf = NULL;
    unsigned char *dstBuf = NULL;
    int result = 0;

    // Extract parameters from fuzz data
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 5; // TJPF_RGB, TJPF_YUY2, TJPF_UYVY, TJPF_GRAY, TJPF_BGR
    int subsamp = Data[3] % 4;     // TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY
    int align = Data[4] + 1;
    int flags = Data[5];
    
    // Ensure align is a power of 2 for valid testing, or test invalid cases
    if (align & (align - 1)) {
        // Test invalid alignment case
        align = (align & -align); // Force to power of 2 for allocation, then test invalid
    }
    
    // Calculate source buffer size
    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) pixelSize = 3; // Fallback for invalid format
    int pitch = width * pixelSize;
    size_t srcSize = (size_t)height * pitch;
    
    // Limit buffer sizes to prevent OOM
    if (srcSize > 1024 * 1024) srcSize = 1024 * 1024;
    
    srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) return 0;
    
    // Fill source buffer with fuzz data
    memcpy(srcBuf, Data, Size < srcSize ? Size : srcSize);
    memset(srcBuf + (Size < srcSize ? Size : srcSize), 0, srcSize - (Size < srcSize ? Size : srcSize));

    // Initialize handle
    handle = tjInitCompress();
    if (!handle) {
        cleanup_resources(handle, srcBuf, dstBuf);
        return 0;
    }

    // Test tjPlaneWidth and tjPlaneHeight using correct component IDs (0=Y, 1=U, 2=V)
    int yWidth = tjPlaneWidth(0, width, subsamp);
    int uWidth = tjPlaneWidth(1, width, subsamp);
    int vWidth = tjPlaneWidth(2, width, subsamp);
    int yHeight = tjPlaneHeight(0, height, subsamp);
    int uHeight = tjPlaneHeight(1, height, subsamp);
    int vHeight = tjPlaneHeight(2, height, subsamp);

    if (yWidth <= 0 || yHeight <= 0) {
        cleanup_resources(handle, srcBuf, dstBuf);
        return 0;
    }

    // Allocate destination buffer using tj3YUVBufSize logic
    int dstSize = tj3YUVBufSize(width, height, subsamp, align);
    if (dstSize <= 0) {
        cleanup_resources(handle, srcBuf, dstBuf);
        return 0;
    }
    
    if (dstSize > 1024 * 1024) dstSize = 1024 * 1024; // Limit size
    
    dstBuf = tjAlloc(dstSize);
    if (!dstBuf) {
        cleanup_resources(handle, srcBuf, dstBuf);
        return 0;
    }

    // 1. Test tjEncodeYUV3
    result = tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align, subsamp, flags);
    
    // 2. Test tj3EncodeYUV8 (requires handle initialized for compression)
    // Re-initialize or reuse handle. Note: tj3EncodeYUV8 expects specific setup.
    // We reuse the handle.
    result = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);

    // 3. Test tjEncodeYUV (legacy)
    // Note: pixelSize is derived from pixelFormat
    result = tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, subsamp, flags);

    // Cleanup
    cleanup_resources(handle, srcBuf, dstBuf);

    return 0;
}