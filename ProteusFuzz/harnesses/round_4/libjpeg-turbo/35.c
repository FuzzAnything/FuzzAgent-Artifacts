// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_resources(unsigned char *dstBuf, unsigned char *yuvBuf, tjhandle handle) {
    if (dstBuf) {
        free(dstBuf);
    }
    if (yuvBuf) {
        free(yuvBuf);
    }
    if (handle) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned char *dstBuf = NULL;
    unsigned char *yuvBuf = NULL;
    int pitch = 0;
    int pixelSize = 0;
    int flags = 0;

    // Allocate buffers based on potential max size to avoid immediate overflow
    // Using a safe upper bound for testing purposes
    size_t maxDim = 4096;
    size_t bufSize = maxDim * maxDim * 4; // Max 4 bytes per pixel (RGBA)
    dstBuf = (unsigned char *)malloc(bufSize);
    yuvBuf = (unsigned char *)malloc(bufSize);

    if (!dstBuf || !yuvBuf) {
        cleanup_resources(dstBuf, yuvBuf, handle);
        return 0;
    }

    // 1. Test tjDecompressHeader3
    if (tjDecompressHeader3(handle, Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace) == 0) {
        if (width > 0 && height > 0) {
            // Ensure buffers are large enough for the actual dimensions
            size_t requiredSize = (size_t)width * (size_t)height * 4;
            if (requiredSize > bufSize) {
                free(dstBuf);
                free(yuvBuf);
                dstBuf = (unsigned char *)malloc(requiredSize);
                yuvBuf = (unsigned char *)malloc(requiredSize);
                if (!dstBuf || !yuvBuf) {
                    cleanup_resources(dstBuf, yuvBuf, handle);
                    return 0;
                }
            }
            
            // Calculate pitch and pixelSize for tjDecompress
            // Assuming RGB output (3 bytes) or YUV (varies), let's try RGB first
            pixelSize = 3; 
            pitch = width * pixelSize;

            // 2. Test tjDecompress
            tjDecompress(handle, Data, Size, dstBuf, width, pitch, height, pixelSize, 0);

            // 3. Test tjDecompressToYUV
            // Re-allocate YUV buffer if needed based on actual size (YUV420 is 1.5x)
            size_t yuvSize = (size_t)width * (size_t)height * 3 / 2;
            if (yuvSize > bufSize) {
                free(yuvBuf);
                yuvBuf = (unsigned char *)malloc(yuvSize);
                if (!yuvBuf) {
                    cleanup_resources(dstBuf, yuvBuf, handle);
                    return 0;
                }
            }
            tjDecompressToYUV(handle, Data, Size, yuvBuf, 0);

            // 4. Test tjDecompressToYUV2 with custom dimensions (downscale)
            int newWidth = width > 1 ? width / 2 : 1;
            int newHeight = height > 1 ? height / 2 : 1;
            tjDecompressToYUV2(handle, Data, Size, yuvBuf, newWidth, 1, newHeight, 0);
        }
    }

    // 5. Test tjDecompressHeader2
    tjDecompressHeader2(handle, Data, Size, &width, &height, &jpegSubsamp);

    // 6. Test tjDecompressHeader
    tjDecompressHeader(handle, Data, Size, &width, &height);

    // Cleanup
    cleanup_resources(dstBuf, yuvBuf, handle);

    return 0;
}