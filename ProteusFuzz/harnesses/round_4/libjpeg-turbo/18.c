// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void *allocate_buffer(size_t size) {
    if (size == 0 || size > 1024 * 1024 * 1024) return NULL;
    void *buf = malloc(size);
    if (buf) memset(buf, 0, size);
    return buf;
}

static void free_buffer(void *buf) {
    if (buf) free(buf);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int pixelFormat = Data[2] % 6; // TJPF_RGB, TJPF_BGR, TJPF_GRAY, etc.
    int subsamp = Data[3] % 5;     // TJSAMP_444, TJSAMP_422, TJSAMP_420, etc.
    
    // Limit dimensions to prevent OOM
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 64; height = 64;
    }

    // Calculate source buffer size
    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize < 1) pixelSize = 1;
    size_t srcSize = (size_t)width * height * pixelSize;
    if (srcSize > 100 * 1024 * 1024) return 0; // Cap at 100MB

    unsigned char *srcBuf = allocate_buffer(srcSize);
    if (!srcBuf) return 0;

    // Fill source buffer with fuzz data
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
        if (copyLen < srcSize) {
            memset(srcBuf + copyLen, 0, srcSize - copyLen);
        }
    }

    // Calculate YUV plane sizes
    int planeSizes[3] = {0};
    int strides[3] = {0};
    int numPlanes = (subsamp == TJSAMP_GRAY) ? 1 : 3;

    // Allocate destination planes
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    for (int i = 0; i < numPlanes; i++) {
        // tj3YUVPlaneSize signature: (componentID, width, stride, height, subsamp)
        // stride 0 means auto-calculate based on width
        planeSizes[i] = tj3YUVPlaneSize(i, width, 0, height, subsamp);
        if (planeSizes[i] <= 0) {
            planeSizes[i] = width * height; // Fallback
        }
        dstPlanes[i] = allocate_buffer(planeSizes[i]);
        if (!dstPlanes[i]) {
            // Cleanup on allocation failure
            for (int j = 0; j < i; j++) free_buffer(dstPlanes[j]);
            free_buffer(srcBuf);
            return 0;
        }
    }

    // Set strides (0 means auto-calculate)
    for (int i = 0; i < numPlanes; i++) {
        strides[i] = 0;
    }

    // --- Test tjEncodeYUV2 ---
    // Create a dummy destination buffer for tjEncodeYUV2 (planar YUV)
    // tjBufSizeYUV2 signature: (width, align, height, subsamp)
    int yuvSize = tjBufSizeYUV2(width, 4, height, subsamp);
    unsigned char *dstBuf = allocate_buffer(yuvSize);
    
    tjhandle handle = tjInitCompress();
    if (handle) {
        tjEncodeYUV2(handle, srcBuf, width, width * pixelSize, height, pixelFormat, dstBuf, subsamp, 0);
        tjDestroy(handle);
    }
    free_buffer(dstBuf);

    // --- Test tj3EncodeYUVPlanes8 ---
    handle = tjInitCompress();
    if (handle) {
        tj3EncodeYUVPlanes8(handle, srcBuf, width, width * pixelSize, height, pixelFormat, dstPlanes, strides);
        tjDestroy(handle);
    }

    // --- Test tjDecodeYUVPlanes ---
    // Re-initialize planes for decode test (using srcBuf as destination for RGB)
    // We need a valid YUV source. Let's use the dstPlanes we just filled (if successful) or dummy data.
    // For fuzzing, we just pass the pointers.
    handle = tjInitDecompress();
    if (handle) {
        tjDecodeYUVPlanes(handle, (const unsigned char **)dstPlanes, strides, subsamp, srcBuf, width, width * pixelSize, height, pixelFormat, 0);
        tjDestroy(handle);
    }

    // --- Test tj3DecompressToYUVPlanes8 ---
    // Create a dummy JPEG buffer (could be garbage)
    size_t jpegSize = (Size > 100) ? Size - 100 : Size;
    const unsigned char *jpegBuf = (Size > 100) ? Data + 100 : Data;
    
    handle = tjInitDecompress();
    if (handle) {
        tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, dstPlanes, strides);
        
        // --- Test Error Codes ---
        int errorCode = tjGetErrorCode(handle);
        int errorSeverity = tj3GetErrorCode(handle);
        (void)errorCode;
        (void)errorSeverity;

        tjDestroy(handle);
    }

    // Cleanup
    for (int i = 0; i < numPlanes; i++) {
        free_buffer(dstPlanes[i]);
    }
    free_buffer(srcBuf);

    return 0;
}