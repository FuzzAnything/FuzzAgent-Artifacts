// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned long jpegSize = Size;
    int ret = 0;
    unsigned char *dstBuf = NULL;
    unsigned long dstBufSize = 0;
    int pitch = 0;
    int flags = 0;
    int align = 1;
    int pixelFormat = TJPF_RGB;

    // Initialize decompressor handle
    handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Test tjDecompressHeader
    if (Size > 0) {
        ret = tjDecompressHeader(handle, (unsigned char *)Data, jpegSize, &width, &height);
        // We don't care about the return value for fuzzing
    }

    // Test tjDecompressHeader2
    if (Size > 0) {
        width = height = jpegSubsamp = 0;
        ret = tjDecompressHeader2(handle, (unsigned char *)Data, jpegSize, &width, &height, &jpegSubsamp);
    }

    // Test tjDecompressHeader3
    if (Size > 0) {
        width = height = jpegSubsamp = jpegColorspace = 0;
        ret = tjDecompressHeader3(handle, Data, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    }

    // If we got valid dimensions, try decompression functions
    if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
        // Test tjDecompressToYUV
        if (Size > 0) {
            dstBufSize = tjBufSizeYUV2(width, align, height, jpegSubsamp);
            if (dstBufSize > 0 && dstBufSize <= 100 * 1024 * 1024) { // Limit to 100MB
                dstBuf = (unsigned char *)malloc(dstBufSize);
                if (dstBuf) {
                    ret = tjDecompressToYUV(handle, (unsigned char *)Data, jpegSize, dstBuf, flags);
                    free(dstBuf);
                    dstBuf = NULL;
                }
            }
        }

        // Test tjDecompressToYUV2
        if (Size > 0) {
            dstBufSize = tjBufSizeYUV2(width, align, height, jpegSubsamp);
            if (dstBufSize > 0 && dstBufSize <= 100 * 1024 * 1024) {
                dstBuf = (unsigned char *)malloc(dstBufSize);
                if (dstBuf) {
                    ret = tjDecompressToYUV2(handle, Data, jpegSize, dstBuf, width, align, height, flags);
                    free(dstBuf);
                    dstBuf = NULL;
                }
            }
        }

        // Test tjDecompress2
        if (Size > 0) {
            pitch = width * tjPixelSize[pixelFormat];
            dstBufSize = pitch * height;
            if (dstBufSize > 0 && dstBufSize <= 100 * 1024 * 1024) {
                dstBuf = (unsigned char *)malloc(dstBufSize);
                if (dstBuf) {
                    ret = tjDecompress2(handle, Data, jpegSize, dstBuf, width, pitch, height, pixelFormat, flags);
                    free(dstBuf);
                    dstBuf = NULL;
                }
            }
        }
    }

    // Clean up
    if (handle) {
        tjDestroy(handle);
    }

    return 0;
}