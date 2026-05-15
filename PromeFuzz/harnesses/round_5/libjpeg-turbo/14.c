// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle init_decompressor() {
    return tjInitDecompress();
}

static void cleanup_decompressor(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    tjhandle handle = init_decompressor();
    if (!handle) {
        return 0;
    }

    // Prepare output variables
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    
    // Test tjDecompressHeader
    tjDecompressHeader(handle, (unsigned char *)Data, (unsigned long)Size, &width, &height);
    
    // Test tjDecompressHeader2
    tjDecompressHeader2(handle, (unsigned char *)Data, (unsigned long)Size, 
                       &width, &height, &jpegSubsamp);
    
    // Test tjDecompressHeader3
    tjDecompressHeader3(handle, Data, (unsigned long)Size, 
                       &width, &height, &jpegSubsamp, &jpegColorspace);
    
    // Only proceed with decompression if we have valid dimensions
    if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
        // Test tjDecompressToYUV
        unsigned long yuvSize = tjBufSizeYUV(width, height, jpegSubsamp);
        if (yuvSize > 0 && yuvSize <= 100 * 1024 * 1024) { // Limit to 100MB
            unsigned char *dstBuf = (unsigned char *)malloc(yuvSize);
            if (dstBuf) {
                tjDecompressToYUV(handle, (unsigned char *)Data, (unsigned long)Size, 
                                 dstBuf, 0);
                free(dstBuf);
            }
        }
        
        // Test tjDecompressToYUV2
        int align = 1;
        unsigned long yuvSize2 = tjBufSizeYUV2(width, align, height, jpegSubsamp);
        if (yuvSize2 > 0 && yuvSize2 <= 100 * 1024 * 1024) {
            unsigned char *dstBuf2 = (unsigned char *)malloc(yuvSize2);
            if (dstBuf2) {
                tjDecompressToYUV2(handle, Data, (unsigned long)Size, 
                                  dstBuf2, width, align, height, 0);
                free(dstBuf2);
            }
        }
        
        // Test tjDecompress2
        int pixelFormat = TJPF_RGB;
        int pitch = width * tjPixelSize[pixelFormat];
        unsigned long rgbSize = pitch * height;
        if (rgbSize > 0 && rgbSize <= 100 * 1024 * 1024) {
            unsigned char *dstBuf3 = (unsigned char *)malloc(rgbSize);
            if (dstBuf3) {
                tjDecompress2(handle, Data, (unsigned long)Size, 
                             dstBuf3, width, pitch, height, pixelFormat, 0);
                free(dstBuf3);
            }
        }
    }
    
    cleanup_decompressor(handle);
    return 0;
}