// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void free_planes(unsigned char **planes, int count) {
    for (int i = 0; i < count; i++) {
        if (planes[i]) {
            free(planes[i]);
            planes[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Need at least 5 bytes to safely read width, height, pixelFormat, subsamp, flags
    if (Size < 5) return 0;

    // Create a handle for decompression
    tjhandle handleDecompress = tjInitDecompress();
    if (!handleDecompress) return 0;

    // Create a handle for compression
    tjhandle handleCompress = tjInitCompress();
    if (!handleCompress) {
        tjDestroy(handleDecompress);
        return 0;
    }

    int result = 0;
    unsigned char *jpegBuf = NULL;
    unsigned char **dstPlanes = NULL;
    unsigned char **srcPlanes = NULL;
    int *strides = NULL;
    unsigned char *rgbBuf = NULL;
    unsigned char *yuvBuf = NULL;

    // Parse input for dimensions and format safely
    int width = (Data[0] % 1024) + 1;
    int height = (Data[1] % 1024) + 1;
    int pixelFormat = Data[2] % 4; // TJPF_GRAY, TJPF_RGB, TJPF_BGR, TJPF_XBGR
    int subsamp = Data[3] % 4;     // TJSAMP_GRAY, TJSAMP_444, TJSAMP_422, TJSAMP_420
    int flags = Data[4] % 2;

    // Ensure minimum size for valid operations
    if (width > 1000 || height > 1000) {
        width = 1000;
        height = 1000;
    }

    // Allocate buffers
    size_t rgbSize = width * height * tjPixelSize[pixelFormat];
    rgbBuf = (unsigned char *)malloc(rgbSize);
    if (!rgbBuf) goto cleanup;
    
    // Initialize RGB buffer with fuzz data or pattern
    memset(rgbBuf, 0x80, rgbSize);
    if (Size > 10) {
        size_t copyLen = (Size - 10) < rgbSize ? (Size - 10) : rgbSize;
        memcpy(rgbBuf, Data + 10, copyLen);
    }

    // Allocate YUV planes (assuming 4:2:0 worst case for size estimation)
    // We need 3 planes for color, 1 for gray. Let's allocate 3 to be safe.
    dstPlanes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
    srcPlanes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
    strides = (int *)malloc(3 * sizeof(int));
    
    if (!dstPlanes || !srcPlanes || !strides) goto cleanup;

    // Calculate plane sizes for 4:2:0 (worst case alignment)
    // tj3YUVPlaneSize requires width, height, subsamp
    // We use TJSAMP_420 for max size estimation
    int planeSizes[3];
    planeSizes[0] = tj3YUVPlaneSize(width, height, TJSAMP_420, 0, 0);
    planeSizes[1] = tj3YUVPlaneSize(width, height, TJSAMP_420, 0, 1);
    planeSizes[2] = tj3YUVPlaneSize(width, height, TJSAMP_420, 0, 2);

    for (int i = 0; i < 3; i++) {
        dstPlanes[i] = (unsigned char *)malloc(planeSizes[i] + 100); // Extra padding
        srcPlanes[i] = (unsigned char *)malloc(planeSizes[i] + 100);
        if (!dstPlanes[i] || !srcPlanes[i]) goto cleanup;
        memset(dstPlanes[i], 0, planeSizes[i]);
        memset(srcPlanes[i], 0, planeSizes[i]);
        strides[i] = width; // Default stride
    }

    // --- Test 1: tj3EncodeYUVPlanes8 ---
    // Encode RGB to YUV planes
    if (tj3EncodeYUVPlanes8(handleCompress, rgbBuf, width, width * tjPixelSize[pixelFormat], height, pixelFormat, dstPlanes, strides) == -1) {
        int errCode = tj3GetErrorCode(handleCompress);
        // Check error code
        if (errCode == TJERR_FATAL || errCode == TJERR_WARNING) {}
    }

    // --- Test 2: tjEncodeYUV2 ---
    // Encode RGB to packed YUV (requires dstBuf)
    size_t yuvSize = tjBufSize(width, height, subsamp);
    yuvBuf = (unsigned char *)malloc(yuvSize);
    if (yuvBuf) {
        if (tjEncodeYUV2(handleCompress, rgbBuf, width, width * tjPixelSize[pixelFormat], height, pixelFormat, yuvBuf, subsamp, flags) == -1) {
            int errCode = tjGetErrorCode(handleCompress);
            if (errCode == TJERR_FATAL || errCode == TJERR_WARNING) {}
        }
    }

    // --- Test 3: tjDecodeYUVPlanes ---
    // Decode YUV planes to RGB
    // Reuse dstPlanes as srcPlanes for decoding
    if (tjDecodeYUVPlanes(handleCompress, (const unsigned char **)dstPlanes, strides, subsamp, rgbBuf, width, width * tjPixelSize[pixelFormat], height, pixelFormat, flags) == -1) {
        int errCode = tjGetErrorCode(handleCompress);
        if (errCode == TJERR_FATAL || errCode == TJERR_WARNING) {}
    }

    // --- Test 4: tj3DecompressToYUVPlanes8 ---
    // We need a valid JPEG buffer. If the input data looks like a JPEG (starts with FF D8), use it.
    // Otherwise, we can't easily generate a valid JPEG on the fly without a compressor.
    // However, we can pass the input data directly to test error handling on invalid JPEGs.
    size_t jpegSize = Size;
    const unsigned char *jpegData = Data;
    
    // Check for JPEG header
    if (Size >= 2 && Data[0] == 0xFF && Data[1] == 0xD8) {
        // Valid JPEG start
        if (tj3DecompressToYUVPlanes8(handleDecompress, jpegData, jpegSize, dstPlanes, strides) == -1) {
            int errCode = tj3GetErrorCode(handleDecompress);
            if (errCode == TJERR_FATAL || errCode == TJERR_WARNING) {}
        }
    } else {
        // Invalid JPEG, test error path
        if (tj3DecompressToYUVPlanes8(handleDecompress, jpegData, jpegSize, dstPlanes, strides) == -1) {
            int errCode = tj3GetErrorCode(handleDecompress);
            if (errCode == TJERR_FATAL || errCode == TJERR_WARNING) {}
        }
    }

    // --- Test 5: tjGetErrorCode and tj3GetErrorCode on NULL handles ---
    // Test with NULL handle (should return TJERR_FATAL)
    int errCodeNull = tj3GetErrorCode(NULL);
    int errCodeNullOld = tjGetErrorCode(NULL);
    if (errCodeNull == TJERR_FATAL && errCodeNullOld == TJERR_FATAL) {}

    // Removed test with invalid handle (0x12345678) as it causes ASAN SEGV
    // The library attempts to dereference the pointer to check internal state.

cleanup:
    if (jpegBuf) free(jpegBuf);
    if (rgbBuf) free(rgbBuf);
    if (yuvBuf) free(yuvBuf);
    if (dstPlanes) {
        free_planes(dstPlanes, 3);
        free(dstPlanes);
    }
    if (srcPlanes) {
        free_planes(srcPlanes, 3);
        free(srcPlanes);
    }
    if (strides) free(strides);
    
    tjDestroy(handleDecompress);
    tjDestroy(handleCompress);

    return 0;
}