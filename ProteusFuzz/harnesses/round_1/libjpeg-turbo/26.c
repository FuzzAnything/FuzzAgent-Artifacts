// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Extract parameters from fuzz input
    int width = (Size > 0) ? Data[0] : 1;
    int height = (Size > 1) ? Data[1] : 1;
    int align = (Size > 2) ? Data[2] : 1;
    int subsamp = (Size > 3) ? Data[3] : TJSAMP_444;
    int quality = (Size > 4) ? Data[4] : 75;
    int flags = (Size > 5) ? Data[5] : 0;

    // Ensure valid ranges for safety
    if (width <= 0 || height <= 0) {
        width = 1;
        height = 1;
    }
    // Ensure align is power of 2 and > 0
    if (align <= 0) align = 1;
    align = 1 << (align % 16); // Force power of 2

    // Test tj3YUVBufSize
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        // Invalid args, try with minimal valid values
        yuvSize = tj3YUVBufSize(1, 1, 1, TJSAMP_444);
    }

    // Test tjBufSizeYUV (legacy)
    unsigned long legacyYuvSize = tjBufSizeYUV(width, height, subsamp);

    // Allocate YUV buffer
    unsigned char *yuvBuf = NULL;
    if (yuvSize > 0 && yuvSize < (size_t)1024 * 1024 * 10) { // Limit allocation
        yuvBuf = (unsigned char *)malloc(yuvSize);
        if (yuvBuf) {
            // Fill with fuzz data or pattern
            if (Size > 0) {
                size_t copySize = (Size < yuvSize) ? Size : yuvSize;
                memcpy(yuvBuf, Data, copySize);
                if (copySize < yuvSize) {
                    memset(yuvBuf + copySize, 0, yuvSize - copySize);
                }
            } else {
                memset(yuvBuf, 0, yuvSize);
            }
        }
    }

    // Initialize TurboJPEG instance for compression
    tjhandle cHandle = tjInitCompress();
    if (!cHandle) {
        free(yuvBuf);
        return 0;
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    // Test tjCompressFromYUV
    if (yuvBuf) {
        int ret = tjCompressFromYUV(cHandle, yuvBuf, width, align, height, subsamp,
                                    &jpegBuf, &jpegSize, quality, flags);
        // Ignore return value for fuzzing, just ensure no crash
    }

    // Test tj3CompressFromYUVPlanes8
    // Only run if yuvBuf is still valid (not freed or overwritten)
    if (yuvBuf) {
        const unsigned char *planes[3];
        int strides[3];
        // Simple planar layout based on subsampling
        if (subsamp == TJSAMP_GRAY) {
            planes[0] = yuvBuf;
            strides[0] = width;
        } else if (subsamp == TJSAMP_444) {
            planes[0] = yuvBuf;
            planes[1] = yuvBuf + width * height;
            planes[2] = yuvBuf + 2 * width * height;
            strides[0] = width; strides[1] = width; strides[2] = width;
        } else if (subsamp == TJSAMP_422) {
            planes[0] = yuvBuf;
            planes[1] = yuvBuf + width * height;
            planes[2] = yuvBuf + width * height + (width / 2) * height;
            strides[0] = width; strides[1] = width / 2; strides[2] = width / 2;
        } else if (subsamp == TJSAMP_420) {
            planes[0] = yuvBuf;
            planes[1] = yuvBuf + width * height;
            planes[2] = yuvBuf + width * height + (width / 2) * (height / 2);
            strides[0] = width; strides[1] = width / 2; strides[2] = width / 2;
        } else {
            planes[0] = yuvBuf; strides[0] = width;
        }

        unsigned char *jpegBuf2 = NULL;
        size_t jpegSize2 = 0;
        tj3CompressFromYUVPlanes8(cHandle, planes, width, strides, height,
                                  &jpegBuf2, &jpegSize2);
        if (jpegBuf2 && jpegBuf2 != jpegBuf) {
            free(jpegBuf2);
        }
    }

    // Destroy compression handle before freeing yuvBuf to ensure no internal references remain
    tjDestroy(cHandle);
    free(yuvBuf);
    yuvBuf = NULL;

    // If we have a JPEG buffer, test decompression
    if (jpegBuf && jpegSize > 0) {
        tjhandle dHandle = tjInitDecompress();
        if (!dHandle) {
            free(jpegBuf);
            return 0;
        }

        // Test tjDecompressToYUV2
        unsigned char *dstBuf = NULL;
        size_t dstSize = tj3YUVBufSize(width, align, height, subsamp);
        if (dstSize > 0 && dstSize < (size_t)1024 * 1024 * 10) {
            dstBuf = (unsigned char *)malloc(dstSize);
            if (dstBuf) {
                tjDecompressToYUV2(dHandle, jpegBuf, jpegSize, dstBuf,
                                   width, align, height, flags);
            }
        }

        // Test tj3DecompressToYUV8
        if (dstBuf) {
            tj3DecompressToYUV8(dHandle, jpegBuf, jpegSize, dstBuf, align);
        }

        tjDestroy(dHandle);
        free(dstBuf);
    }

    // Cleanup
    free(jpegBuf);

    return 0;
}