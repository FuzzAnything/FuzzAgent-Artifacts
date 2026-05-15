// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
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

#define MAX_DIM 4096
#define MAX_BUFFER_SIZE (MAX_DIM * MAX_DIM * 4)

static int custom_filter(short *coeffs, tjregion arrayRegion,
                         tjregion planeRegion, int componentID,
                         int transformID, struct tjtransform *transform) {
    // Simple custom filter that does nothing
    (void)coeffs;
    (void)arrayRegion;
    (void)planeRegion;
    (void)componentID;
    (void)transformID;
    (void)transform;
    return 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Use the fuzzer input as JPEG data
    unsigned char *jpegBuf = (unsigned char *)Data;
    unsigned long jpegSize = (unsigned long)Size;

    // Variables for header extraction
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    
    // Try all header extraction functions
    tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
    tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);

    // Skip if dimensions are unreasonable
    if (width <= 0 || height <= 0 || width > MAX_DIM || height > MAX_DIM) {
        tjDestroy(handle);
        return 0;
    }

    // Try decompression to YUV
    unsigned long yuvSize = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
    if (yuvSize > 0 && yuvSize <= MAX_BUFFER_SIZE) {
        unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
        if (yuvBuf) {
            tjDecompressToYUV(handle, jpegBuf, jpegSize, yuvBuf, 0);
            free(yuvBuf);
        }
    }

    // Try decompression to RGB
    int pitch = width * tjPixelSize[TJPF_RGB];
    unsigned long rgbSize = (unsigned long)pitch * height;
    if (rgbSize > 0 && rgbSize <= MAX_BUFFER_SIZE) {
        unsigned char *rgbBuf = (unsigned char *)malloc(rgbSize);
        if (rgbBuf) {
            tjDecompress2(handle, jpegBuf, jpegSize, rgbBuf, width, pitch, height, TJPF_RGB, 0);
            free(rgbBuf);
        }
    }

    // Try transformation
    if (Size > 100) {  // Need enough data for transformation
        tjtransform transform;
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        transform.op = TJXOP_NONE;
        transform.options = TJXOPT_PERFECT;
        transform.data = NULL;
        transform.customFilter = custom_filter;

        unsigned char *dstBuf = NULL;
        unsigned long dstSize = 0;
        
        tjTransform(handle, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, &transform, 0);
        
        if (dstBuf) {
            tjFree(dstBuf);
        }
    }

    tjDestroy(handle);
    return 0;
}