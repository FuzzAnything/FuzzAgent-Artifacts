// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void free_buffers(unsigned char **dstBufs, int n) {
    for (int i = 0; i < n; i++) {
        if (dstBufs[i]) {
            free(dstBufs[i]);
            dstBufs[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int op = Data[0] % 5;
    int n = (Data[1] % 3) + 1; // 1 to 3 transforms
    int useTj3 = (Data[0] % 2);

    tjhandle handle = NULL;
    unsigned char **dstBufs = NULL;
    size_t *dstSizes = NULL;
    tjtransform *transforms = NULL;

    // Allocate arrays for transforms and outputs
    if (n > 0) {
        dstBufs = (unsigned char **)malloc(n * sizeof(unsigned char *));
        dstSizes = (size_t *)malloc(n * sizeof(size_t));
        transforms = (tjtransform *)malloc(n * sizeof(tjtransform));
        if (!dstBufs || !dstSizes || !transforms) {
            free(dstBufs);
            free(dstSizes);
            free(transforms);
            return 0;
        }
        memset(dstBufs, 0, n * sizeof(unsigned char *));
        memset(dstSizes, 0, n * sizeof(size_t));
        memset(transforms, 0, n * sizeof(tjtransform));
    }

    // Initialize handle based on choice
    if (useTj3) {
        handle = tj3Init(TJINIT_DECOMPRESS | TJINIT_TRANSFORM);
    } else {
        handle = tjInitTransform();
    }

    if (!handle) {
        free(dstBufs);
        free(dstSizes);
        free(transforms);
        return 0;
    }

    // Use input data as potential JPEG buffer
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // Attempt to read header to populate internal state for transform size calc
    if (useTj3) {
        tj3DecompressHeader(handle, jpegBuf, jpegSize);
    } else {
        // For legacy API, we might need to decompress or just rely on transform
        // tjTransform internally reads headers if needed, but tj3DecompressHeader is safer for size calc
        // Since tjInitTransform doesn't have a direct header read equivalent in the legacy API 
        // without decompressing, we proceed to transform which handles it.
    }

    // Setup transforms
    for (int i = 0; i < n; i++) {
        transforms[i].op = op;
        // Set some random crop region if possible, keeping it safe
        if (Size > 4) {
            int w = Data[2] % 100;
            int h = Data[3] % 100;
            transforms[i].r.w = w > 0 ? w : 1;
            transforms[i].r.h = h > 0 ? h : 1;
            transforms[i].r.x = 0;
            transforms[i].r.y = 0;
        } else {
            transforms[i].r.w = 1;
            transforms[i].r.h = 1;
            transforms[i].r.x = 0;
            transforms[i].r.y = 0;
        }
        transforms[i].options = 0;
        transforms[i].customFilter = NULL;
        transforms[i].data = NULL;
    }

    // Calculate buffer size if using tj3
    if (useTj3 && n > 0) {
        size_t bufSize = tj3TransformBufSize(handle, &transforms[0]);
        if (bufSize > 0 && bufSize < (1024 * 1024 * 100)) { // Sanity check
            // Pre-allocate if we want, but letting API handle NULL is also fine
            // dstBufs[0] = (unsigned char *)malloc(bufSize);
            // dstSizes[0] = bufSize;
            // Leaving as NULL to let API allocate
        }
    }

    // Perform Transform
    if (useTj3) {
        // tj3Transform
        int ret = tj3Transform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms);
        (void)ret; // Ignore return for fuzzing, just want coverage
    } else {
        // tjTransform (legacy)
        // Cast sizes to unsigned long as required by legacy API
        unsigned long *legacyDstSizes = (unsigned long *)dstSizes;
        int ret = tjTransform(handle, jpegBuf, (unsigned long)jpegSize, n, dstBufs, legacyDstSizes, transforms, 0);
        (void)ret;
    }

    // Cleanup
    free_buffers(dstBufs, n);
    free(dstBufs);
    free(dstSizes);
    free(transforms);
    
    // Destroy handle
    if (handle) {
        if (useTj3) {
            tj3Destroy(handle);
        } else {
            tjDestroy(handle);
        }
    }

    return 0;
}