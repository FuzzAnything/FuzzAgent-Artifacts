// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_to_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file for any file-based operations
    write_to_dummy_file(Data, Size);
    
    // Initialize TurboJPEG decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }
    
    // Initialize variables for API calls
    int width = 0, height = 0, jpegSubsamp = 0;
    unsigned long jpegSize = Size;
    unsigned char *jpegBuf = (unsigned char *)Data;
    
    // Test tjDecompressHeader
    if (Size > 0) {
        tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
    }
    
    // Test tjDecompressHeader2
    if (Size > 0) {
        tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    }
    
    // Test tjDecompress if we got valid dimensions
    if (width > 0 && height > 0 && Size > 0) {
        // Calculate buffer size for RGB output (3 bytes per pixel)
        unsigned long dstBufSize = width * height * 3;
        unsigned char *dstBuf = (unsigned char *)malloc(dstBufSize);
        if (dstBuf) {
            // Try different pixel sizes and flags
            tjDecompress(handle, jpegBuf, jpegSize, dstBuf, width, 0, height, 3, 0);
            tjDecompress(handle, jpegBuf, jpegSize, dstBuf, width, width * 3, height, 3, TJFLAG_FASTDCT);
            tjDecompress(handle, jpegBuf, jpegSize, dstBuf, width, width * 3, height, 3, TJFLAG_ACCURATEDCT);
            free(dstBuf);
        }
    }
    
    // Test tjDecompressToYUV2 if we got valid dimensions
    if (width > 0 && height > 0 && Size > 0) {
        // Calculate buffer size for YUV output
        unsigned long yuvBufSize = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
        unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
        if (yuvBuf) {
            // Try different alignments and flags
            tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, width, 4, height, 0);
            tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, width, 1, height, TJFLAG_FASTDCT);
            tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, width, 8, height, TJFLAG_ACCURATEDCT);
            free(yuvBuf);
        }
    }
    
    // Test tj3SetScalingFactor with different scaling factors
    // Note: tj3SetScalingFactor requires a tjscalingfactor struct
    // We'll test with a few common scaling factors
    tjscalingfactor scalingFactors[] = {
        {1, 8},  // 1/8 scale
        {1, 4},  // 1/4 scale
        {1, 2},  // 1/2 scale
        {3, 4},  // 3/4 scale
        {1, 1},  // No scale
        {2, 1}   // 2x scale (if supported)
    };
    
    for (int i = 0; i < 6; i++) {
        tj3SetScalingFactor(handle, scalingFactors[i]);
    }
    
    // Test with TJUNSCALED
    tjscalingfactor unscaled = TJUNSCALED;
    tj3SetScalingFactor(handle, unscaled);
    
    // Clean up
    tjDestroy(handle);
    
    return 0;
}