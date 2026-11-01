// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Use the first 4 bytes to determine the fuzzing path
    uint32_t seed = 0;
    if (Size >= 4) {
        memcpy(&seed, Data, sizeof(uint32_t));
    }

    int initType = (seed & 0x3) ? TJINIT_DECOMPRESS : TJINIT_COMPRESS;
    tjhandle handle = tj3Init(initType);
    if (!handle) {
        return 0;
    }

    // Prepare a minimal JPEG-like buffer for testing
    size_t jpegSize = (Size > 4) ? Size - 4 : 0;
    const uint8_t *jpegBuf = (Size > 4) ? Data + 4 : (const uint8_t *)"";
    
    // Ensure we have at least a minimal JPEG SOI marker if size is small
    if (jpegSize < 2) {
        static uint8_t miniJpeg[2] = {0xFF, 0xD8};
        jpegBuf = miniJpeg;
        jpegSize = 2;
    }

    // Test tj3Get to retrieve parameters
    int param = (seed & 0x1) ? TJPARAM_QUALITY : TJPARAM_SUBSAMP;
    int val = tj3Get(handle, param);
    (void)val; 

    // Test tj3DecompressHeader
    if (initType == TJINIT_DECOMPRESS || initType == TJINIT_TRANSFORM) {
        tj3DecompressHeader(handle, jpegBuf, jpegSize);
    }

    // Prepare transform structures
    tjtransform transforms[2];
    memset(transforms, 0, sizeof(transforms));

    // Fuzz transform parameters
    transforms[0].op = (seed & 0x7) ? TJXOP_ROT90 : TJXOP_NONE;
    transforms[0].options = (seed >> 3) & 0xF;
    transforms[0].r.w = (seed >> 7) & 0xFFFF;
    transforms[0].r.h = (seed >> 23) & 0xFFFF;
    
    transforms[1].op = (seed >> 10) & 0x7;
    transforms[1].options = (seed >> 17) & 0xF;

    // Test tj3TransformBufSize
    size_t bufSize = tj3TransformBufSize(handle, &transforms[0]);
    
    // Allocate destination buffers based on the calculated size or a fallback
    unsigned char *dstBufs[2] = {NULL, NULL};
    size_t dstSizes[2] = {0, 0};

    if (bufSize > 0 && bufSize < 100000000) { 
        dstBufs[0] = (unsigned char *)malloc(bufSize);
        if (dstBufs[0]) {
            dstSizes[0] = bufSize;
        }
    }
    
    if (bufSize > 0 && bufSize < 100000000) {
        dstBufs[1] = (unsigned char *)malloc(bufSize);
        if (dstBufs[1]) {
            dstSizes[1] = bufSize;
        }
    }

    // Test tj3Transform
    int n = (seed & 0x1) ? 1 : 2;
    if (n == 2 && (!dstBufs[0] || !dstBufs[1])) {
        n = 1;
    }
    
    if (n > 0 && jpegSize > 0) {
        tj3Transform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms);
    }

    // Test legacy tjTransform
    if (n > 0 && jpegSize > 0) {
        unsigned long jpegSizeUL = (unsigned long)jpegSize;
        unsigned long *dstSizesUL = (unsigned long *)malloc(n * sizeof(unsigned long));
        if (dstSizesUL) {
            for (int i = 0; i < n; i++) {
                dstSizesUL[i] = (unsigned long)dstSizes[i];
            }
            tjTransform(handle, jpegBuf, jpegSizeUL, n, dstBufs, dstSizesUL, transforms, 0);
            free(dstSizesUL);
        }
    }

    // Cleanup
    if (dstBufs[0]) free(dstBufs[0]);
    if (dstBufs[1]) free(dstBufs[1]);
    
    tjDestroy(handle);

    return 0;
}