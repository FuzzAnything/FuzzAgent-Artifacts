// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle != NULL) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tjhandle handle = tjInitDecompress();
    if (handle == NULL) {
        return 0;
    }

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int ret;

    // Fuzz tjDecompressHeader
    ret = tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);
    (void)ret;

    // Fuzz tjDecompressHeader2
    ret = tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp);
    (void)ret;

    // Fuzz tjDecompressHeader3
    ret = tjDecompressHeader3(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);
    (void)ret;

    // If header parsing succeeded, attempt decompression
    if (width > 0 && height > 0) {
        // Allocate destination buffer for RGB output
        int pitch = width * 3; // RGB
        size_t dstSize = (size_t)height * pitch;
        unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
        if (dstBuf != NULL) {
            // Fuzz tjDecompress
            ret = tjDecompress(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, 3, 0);
            (void)ret;

            // Fuzz tjDecompress2 with various flags
            ret = tjDecompress2(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, TJPF_RGB, 0);
            (void)ret;
            ret = tjDecompress2(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, TJPF_RGB, TJFLAG_FASTDCT);
            (void)ret;
            ret = tjDecompress2(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, TJPF_GRAY, 0);
            (void)ret;

            // Fuzz tjDecompressToYUV2
            // Allocate YUV buffer (approximate size: width * height * 1.5)
            size_t yuvSize = (size_t)width * height * 3 / 2;
            unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
            if (yuvBuf != NULL) {
                ret = tjDecompressToYUV2(handle, (unsigned char *)Data, Size, yuvBuf, width, 1, height, 0);
                (void)ret;
                ret = tjDecompressToYUV2(handle, (unsigned char *)Data, Size, yuvBuf, width, 16, height, TJFLAG_FASTDCT);
                (void)ret;
                free(yuvBuf);
            }

            free(dstBuf);
        }
    }

    cleanup_handle(handle);
    return 0;
}