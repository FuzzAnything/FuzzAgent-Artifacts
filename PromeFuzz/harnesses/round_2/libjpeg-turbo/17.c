// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned long jpegSize = Size;
    
    // Test tjDecompressHeader
    tjDecompressHeader(handle, (unsigned char *)Data, jpegSize, &width, &height);
    
    // Test tjDecompressHeader2
    tjDecompressHeader2(handle, (unsigned char *)Data, jpegSize, &width, &height, &jpegSubsamp);
    
    // Test tjDecompressHeader3
    tjDecompressHeader3(handle, Data, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    
    // If dimensions are valid, test decompression functions
    if (width > 0 && height > 0 && width <= 16384 && height <= 16384) {
        // Test tjDecompressToYUV2
        int align = 4;
        unsigned long yuvSize = tjBufSizeYUV2(width, align, height, jpegSubsamp);
        if (yuvSize > 0 && yuvSize < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *dstBuf = (unsigned char *)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV2(handle, Data, jpegSize, dstBuf, width, align, height, 0);
                free(dstBuf);
            }
        }
        
        // Test tjDecompress
        int pitch = width * 3; // RGB
        unsigned long rgbSize = pitch * height;
        if (rgbSize > 0 && rgbSize < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *dstBuf = (unsigned char *)malloc(rgbSize);
            if (dstBuf) {
                tjDecompress(handle, (unsigned char *)Data, jpegSize, dstBuf, width, pitch, height, 3, 0);
                free(dstBuf);
            }
        }
    }
    
    // Test tjDestroy
    tjDestroy(handle);
    
    return 0;
}

#ifdef __cplusplus
}
#endif