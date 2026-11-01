// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int run_fuzzing(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    int ret = 0;

    // Test tjDecompressHeader
    ret = tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);
    if (ret != 0) {
        // Invalid header, but we continue to test other paths
    }

    // Test tjDecompressHeader2
    ret = tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp);
    if (ret != 0) {
        // Invalid header
    }

    // Test tjDecompressHeader3
    ret = tjDecompressHeader3(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);
    if (ret != 0) {
        // Invalid header
    }

    // If dimensions were successfully parsed, proceed to decompression tests
    if (width > 0 && height > 0) {
        // Calculate buffer sizes
        int pixelSize = (width * height * 3); // RGB
        int yuvSize = (width * height * 3 / 2); // YUV 4:2:0 approx
        
        unsigned char *rgbBuf = malloc(pixelSize);
        unsigned char *yuvBuf = malloc(yuvSize);
        
        if (rgbBuf && yuvBuf) {
            // Test tjDecompress (RGB)
            ret = tjDecompress(handle, (unsigned char *)Data, Size, rgbBuf, 
                               width, width * 3, height, pixelSize, 0);
            
            // Test tjDecompress (YUV)
            ret = tjDecompress(handle, (unsigned char *)Data, Size, rgbBuf, 
                               width, width * 3, height, pixelSize, TJ_YUV);

            // Test tjDecompressToYUV
            ret = tjDecompressToYUV(handle, (unsigned char *)Data, Size, yuvBuf, 0);

            // Test tjDecompressToYUV2 with various parameters
            int align = (Size % 4) + 1; // Vary alignment
            int w = (Size % width) + 1; // Vary width
            int h = (Size % height) + 1; // Vary height
            
            ret = tjDecompressToYUV2(handle, (unsigned char *)Data, Size, yuvBuf, w, align, h, 0);
            
            // Test with 0 dimensions (original size)
            ret = tjDecompressToYUV2(handle, (unsigned char *)Data, Size, yuvBuf, 0, 0, 0, 0);
        }
        
        free(rgbBuf);
        free(yuvBuf);
    }

    tjDestroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    return run_fuzzing(Data, Size);
}