// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize handle for transformation
    tjhandle handle = tjInitTransform();
    if (!handle) return 0;

    // Attempt to read header (primes decompressor)
    size_t jpegSize = Size;
    tj3DecompressHeader(handle, Data, jpegSize);

    // Prepare transform structure
    tjtransform transform = {0};
    // Randomize operation based on input data to explore states
    // Ensure we have enough data before accessing indices
    transform.op = (int)(Data[0] % 10); 
    transform.options = (int)(Data[1] % 100);
    
    // Initialize cropping region
    transform.r.w = (int)(Data[2] * 100);
    transform.r.h = (int)(Data[3] * 100);
    transform.r.x = 0;
    transform.r.y = 0;

    // Test tj3TransformBufSize
    size_t bufSize = tj3TransformBufSize(handle, &transform);
    
    // Allocate destination buffer if size is valid and non-zero
    unsigned char *dstBuf = NULL;
    size_t dstSize = 0;
    
    if (bufSize > 0 && bufSize < (1024 * 1024 * 100)) { // Limit allocation
        dstBuf = (unsigned char *)malloc(bufSize);
        if (dstBuf) {
            dstSize = bufSize;
        }
    }

    // Test tj3Transform
    unsigned char *dstBufs[] = {dstBuf};
    size_t dstSizes[] = {dstSize};
    
    if (dstBuf) {
        // tj3Transform signature: (handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms)
        tj3Transform(handle, Data, jpegSize, 1, dstBufs, dstSizes, &transform);
    }

    // Test legacy tjTransform (if available in header, usually wrapper)
    // Note: tjTransform takes unsigned long for size, casting carefully
    if (Size <= 0x7FFFFFFF) {
        unsigned long ulSize = (unsigned long)jpegSize;
        unsigned long ulDstSizes[] = {dstSize};
        // tjTransform signature: (handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms, flags)
        tjTransform(handle, Data, ulSize, 1, dstBufs, ulDstSizes, &transform, 0);
    }

    // Cleanup
    if (dstBuf) {
        free(dstBuf);
    }
    tjDestroy(handle);

    return 0;
}