// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tjhandle handle = tjInitDecompress();
    if (handle == NULL) {
        return 0;
    }

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    
    // Test tjDecompressHeader
    if (Size > 0) {
        tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);
    }

    // Test tjDecompressHeader2
    if (Size > 0) {
        tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp);
    }

    // Test tjDecompressHeader3
    if (Size > 0) {
        tjDecompressHeader3(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);
    }

    // Allocate buffers for decompression tests
    // Ensure minimum size to avoid allocation failures or crashes
    size_t bufferSize = Size > 0 ? Size : 1;
    unsigned char *dstBuf = (unsigned char *)malloc(bufferSize);
    if (dstBuf == NULL) {
        cleanup_handle(handle);
        return 0;
    }

    // Test tjDecompress2
    // Use 0 for width/height to request original dimensions
    // pitch is set to 0 to let library calculate it
    // pixelFormat: TJPF_RGB is a common format
    // flags: 0 for default behavior
    if (Size > 0) {
        tjDecompress2(handle, Data, Size, dstBuf, 0, 0, 0, TJPF_RGB, 0);
    }

    // Test tjDecompressToYUV2
    // width/height 0 for original, align 16 is common
    if (Size > 0) {
        tjDecompressToYUV2(handle, Data, Size, dstBuf, 0, 16, 0, 0);
    }

    free(dstBuf);
    cleanup_handle(handle);
    return 0;
}