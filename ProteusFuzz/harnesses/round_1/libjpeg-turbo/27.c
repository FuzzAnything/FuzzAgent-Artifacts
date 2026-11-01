// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static void cleanup_buffer(unsigned char **buf) {
    if (buf && *buf) {
        tjFree(*buf);
        *buf = NULL;
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 7) return 0;

    // Initialize TurboJPEG handle for compression
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Use data to determine dimensions and parameters
    int width = (Data[0] % 256) + 1;
    int height = (Data[1] % 256) + 1;
    int quality = (Data[2] % 101); // 0-100
    int subsamp = (Data[3] % 4); // 0-3
    int flags = (Data[4] % 8);
    int pixelFormat = (Data[5] % 4); // 0-3
    int align = 1 << (Data[6] % 4); // 1, 2, 4, 8

    // Ensure dimensions are reasonable to avoid huge allocations
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Calculate sizes
    int pitch = width * 3; // RGB
    size_t srcSize = (size_t)height * pitch;
    size_t yuvSize = tj3YUVBufSize(width, height, align, subsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);

    // Allocate source buffers
    unsigned char *srcBuf = (unsigned char *)tjAlloc(srcSize);
    unsigned char *yuvBuf = (unsigned char *)tjAlloc(yuvSize);
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    size_t jpegSize_t = 0;

    if (!srcBuf || !yuvBuf) {
        cleanup_handle(handle);
        if (srcBuf) tjFree(srcBuf);
        if (yuvBuf) tjFree(yuvBuf);
        return 0;
    }

    // Fill source buffers with fuzz data
    memset(srcBuf, 0, srcSize);
    memset(yuvBuf, 0, yuvSize);
    
    // Safe copy for srcBuf
    size_t copySize = (Size > srcSize) ? srcSize : Size;
    memcpy(srcBuf, Data, copySize);
    
    // Safe copy for yuvBuf
    copySize = (Size > yuvSize) ? yuvSize : Size;
    memcpy(yuvBuf, Data, copySize);

    // Prepare ICC profile data (if available in fuzz input)
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    if (Size > 10) {
        iccSize = Size - 10;
        iccBuf = (unsigned char *)tjAlloc(iccSize);
        if (iccBuf) {
            memcpy(iccBuf, Data + 10, iccSize);
        }
    }

    // Test tj3SetICCProfile
    if (iccBuf && iccSize > 0) {
        tj3SetICCProfile(handle, iccBuf, iccSize);
    } else {
        // Try removing profile
        tj3SetICCProfile(handle, NULL, 0);
    }

    // Test tjCompress
    unsigned long compressedSize = 0;
    unsigned char *dstBuf = (unsigned char *)tjAlloc(jpegBufSize);
    if (dstBuf) {
        tjCompress(handle, srcBuf, width, pitch, height, 3, dstBuf, &compressedSize, subsamp, quality, flags);
        tjFree(dstBuf);
    }

    // Test tjCompress2
    unsigned char *jpegBuf2 = NULL;
    unsigned long jpegSize2 = 0;
    tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf2, &jpegSize2, subsamp, quality, flags);
    cleanup_buffer(&jpegBuf2);

    // Test tjCompressFromYUVPlanes
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0}; // 0 means use plane width
    
    // Calculate plane sizes based on subsampling
    size_t yPlaneSize = (size_t)width * height;
    size_t uvPlaneSize = 0;
    
    if (subsamp != 0) {
        uvPlaneSize = ((size_t)width / 2) * ((size_t)height / 2);
        // Check if yuvBuf is large enough to hold Y + U + V planes
        if (yuvSize >= yPlaneSize + uvPlaneSize * 2) {
            srcPlanes[0] = yuvBuf;
            srcPlanes[1] = yuvBuf + yPlaneSize;
            srcPlanes[2] = srcPlanes[1] + uvPlaneSize;
            strides[0] = width;
            strides[1] = width / 2;
            strides[2] = width / 2;
        }
    } else {
        // Grayscale or no subsampling, only Y plane needed
        if (yuvSize >= yPlaneSize) {
            srcPlanes[0] = yuvBuf;
            strides[0] = width;
        }
    }

    unsigned char *jpegBuf3 = NULL;
    unsigned long jpegSize3 = 0;
    if (srcPlanes[0]) {
        tjCompressFromYUVPlanes(handle, srcPlanes, width, strides, height, subsamp, &jpegBuf3, &jpegSize3, quality, flags);
        cleanup_buffer(&jpegBuf3);
    }

    // Test tj3CompressFromYUVPlanes8
    unsigned char *jpegBuf4 = NULL;
    size_t jpegSize4 = 0;
    if (srcPlanes[0]) {
        tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, &jpegBuf4, &jpegSize4);
        cleanup_buffer(&jpegBuf4);
    }

    // Test tj3CompressFromYUV8
    // Ensure yuvBuf is valid and large enough for the operation
    // The previous allocation used tj3YUVBufSize which should be correct,
    // but we must ensure the data passed is consistent with the parameters.
    unsigned char *jpegBuf5 = NULL;
    size_t jpegSize5 = 0;
    if (yuvBuf && yuvSize > 0) {
        tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf5, &jpegSize5);
        cleanup_buffer(&jpegBuf5);
    }

    // Cleanup
    if (iccBuf) tjFree(iccBuf);
    tjFree(srcBuf);
    tjFree(yuvBuf);
    cleanup_handle(handle);

    return 0;
}