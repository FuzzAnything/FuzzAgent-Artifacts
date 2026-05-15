// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Prepare output variables for header functions
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    
    // Try tjDecompressHeader
    if (Size > 0) {
        unsigned long jpegSize = (unsigned long)Size;
        tjDecompressHeader(handle, (unsigned char *)Data, jpegSize, &width, &height);
    }
    
    // Try tjDecompressHeader2
    if (Size > 0) {
        unsigned long jpegSize = (unsigned long)Size;
        tjDecompressHeader2(handle, (unsigned char *)Data, jpegSize, &width, &height, &jpegSubsamp);
    }
    
    // Try tjDecompressHeader3
    if (Size > 0) {
        unsigned long jpegSize = (unsigned long)Size;
        tjDecompressHeader3(handle, Data, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    }
    
    // Try tjDecompressToYUV - need to allocate destination buffer
    if (Size > 0 && width > 0 && height > 0) {
        unsigned long jpegSize = (unsigned long)Size;
        unsigned long yuvSize = tjBufSizeYUV(width, height, jpegSubsamp);
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *dstBuf = (unsigned char *)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV(handle, (unsigned char *)Data, jpegSize, dstBuf, 0);
                free(dstBuf);
            }
        }
    }
    
    // Try tjDecompress2 - need to allocate destination buffer
    if (Size > 0 && width > 0 && height > 0) {
        unsigned long jpegSize = (unsigned long)Size;
        int pitch = width * tjPixelSize[TJPF_RGB];
        unsigned long rgbSize = pitch * height;
        if (rgbSize > 0 && rgbSize < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *dstBuf = (unsigned char *)malloc(rgbSize);
            if (dstBuf) {
                tjDecompress2(handle, Data, jpegSize, dstBuf, width, pitch, height, TJPF_RGB, 0);
                free(dstBuf);
            }
        }
    }
    
    // Try tjDecompressToYUV2 - need to allocate destination buffer
    if (Size > 0 && width > 0 && height > 0) {
        unsigned long jpegSize = (unsigned long)Size;
        unsigned long yuvSize = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *dstBuf = (unsigned char *)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV2(handle, Data, jpegSize, dstBuf, width, 4, height, 0);
                free(dstBuf);
            }
        }
    }
    
    // Cleanup
    tjDestroy(handle);
    
    return 0;
}