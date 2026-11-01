// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
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

static void cleanup(tjhandle *handle) {
    if (*handle) {
        tjDestroy(*handle);
        *handle = NULL;
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int ret;

    // Test tjDecompressHeader
    ret = tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);
    (void)ret;

    // Test tjDecompressHeader2
    ret = tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp);
    (void)ret;

    // Test tjDecompressHeader3
    ret = tjDecompressHeader3(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);
    (void)ret;

    // Ensure dimensions are valid before allocating output buffer
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        cleanup(&handle);
        return 0;
    }

    // Calculate pitch and pixel size for various formats
    int pitch = width * 3; // RGB
    int pixelSize = 3;
    int pixelFormat = TJPF_RGB;
    
    // Allocate destination buffer with some safety margin
    size_t dstSize = (size_t)height * pitch + 1024;
    unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
    if (!dstBuf) {
        cleanup(&handle);
        return 0;
    }

    // Test tjDecompress
    ret = tjDecompress(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, pixelSize, 0);
    (void)ret;

    // Test tjDecompress2 with different flags and formats
    int flags = 0;
    if (Size > 10 && Data[Size - 1] % 2 == 0) flags = TJFLAG_FASTDCT;
    if (Size > 20 && Data[Size - 2] % 2 == 0) flags |= TJFLAG_FASTUPSAMPLE;
    
    ret = tjDecompress2(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, pixelFormat, flags);
    (void)ret;

    // Test tjDecompress2 with scaling (0 for original)
    ret = tjDecompress2(handle, (unsigned char *)Data, Size, dstBuf, 0, 0, 0, pixelFormat, flags);
    (void)ret;

    // Test tjDecompressToYUV2
    int align = 16;
    ret = tjDecompressToYUV2(handle, (unsigned char *)Data, Size, dstBuf, width, align, height, flags);
    (void)ret;

    // Test tjDecompressToYUV2 with 0 dimensions
    ret = tjDecompressToYUV2(handle, (unsigned char *)Data, Size, dstBuf, 0, align, 0, flags);
    (void)ret;

    free(dstBuf);
    cleanup(&handle);
    return 0;
}