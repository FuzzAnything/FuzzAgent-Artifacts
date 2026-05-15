// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Create a TurboJPEG decompression handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Variables for dimensions and subsampling
    int width = 0, height = 0, jpegSubsamp = 0;
    
    // Test tjDecompressHeader2
    if (Size > 0) {
        tjDecompressHeader2(handle, (unsigned char*)Data, Size, &width, &height, &jpegSubsamp);
    }

    // Test tjDecompressHeader
    if (Size > 0) {
        tjDecompressHeader(handle, (unsigned char*)Data, Size, &width, &height);
    }

    // Test tj3SetScalingFactor with a supported scaling factor
    tjscalingfactor scalingFactor;
    scalingFactor.num = 1;
    scalingFactor.denom = 2;
    tj3SetScalingFactor(handle, scalingFactor);

    // If dimensions were obtained, test decompression functions
    if (width > 0 && height > 0) {
        // Calculate scaled dimensions for YUV decompression
        int scaledWidth = TJSCALED(width, scalingFactor);
        int scaledHeight = TJSCALED(height, scalingFactor);
        
        // Allocate buffer for YUV decompression
        unsigned long yuvBufSize = scaledWidth * scaledHeight * 3;
        unsigned char *yuvBuf = (unsigned char*)malloc(yuvBufSize);
        if (yuvBuf) {
            // Test tjDecompressToYUV2
            if (Size > 0) {
                tjDecompressToYUV2(handle, Data, Size, yuvBuf, scaledWidth, 1, scaledHeight, 0);
            }
            free(yuvBuf);
        }

        // Allocate buffer for RGB decompression
        int pixelSize = 3; // RGB
        int pitch = width * pixelSize;
        unsigned long rgbBufSize = pitch * height;
        unsigned char *rgbBuf = (unsigned char*)malloc(rgbBufSize);
        if (rgbBuf) {
            // Test tjDecompress
            if (Size > 0) {
                tjDecompress(handle, (unsigned char*)Data, Size, rgbBuf, width, pitch, height, pixelSize, 0);
            }
            free(rgbBuf);
        }
    }

    // Test tjDestroy
    tjDestroy(handle);

    return 0;
}