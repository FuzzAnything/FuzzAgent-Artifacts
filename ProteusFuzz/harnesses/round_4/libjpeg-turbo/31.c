// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize TurboJPEG instance for transformation
    tjhandle handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) return 0;

    // Prepare a tjtransform structure
    tjtransform transform = {0};
    transform.r = (tjregion){0, 0, 0, 0};
    transform.op = 0;
    transform.options = 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // Use the input data as a potential JPEG buffer
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // Try to read the JPEG header to initialize decompression state
    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) == 0) {
        // Calculate required buffer size for transformation
        size_t bufSize = tj3TransformBufSize(handle, &transform);
        if (bufSize > 0) {
            unsigned char *dstBuf = NULL;
            size_t dstSize = 0;

            // Attempt transformation
            int n = 1;
            unsigned char *dstBufs[1] = {NULL};
            size_t dstSizes[1] = {0};
            tjtransform transforms[1] = {transform};

            int result = tj3Transform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms);
            if (result == 0 && dstBufs[0] != NULL) {
                // Free allocated buffer if successful
                tj3Free(dstBufs[0]);
            }
        }
    }

    // Also test the legacy tjTransform API if possible
    unsigned long legacyJpegSize = (unsigned long)Size;
    unsigned char *legacyDstBuf = NULL;
    unsigned long legacyDstSize = 0;
    tjtransform legacyTransform = transform;
    unsigned char *legacyDstBufs[1] = {NULL};
    unsigned long legacyDstSizes[1] = {0};
    tjtransform legacyTransforms[1] = {legacyTransform};

    int legacyResult = tjTransform(handle, jpegBuf, legacyJpegSize, 1, legacyDstBufs, legacyDstSizes, legacyTransforms, 0);
    if (legacyResult == 0 && legacyDstBufs[0] != NULL) {
        tj3Free(legacyDstBufs[0]);
    }

    // Cleanup
    tj3Destroy(handle);

    return 0;
}