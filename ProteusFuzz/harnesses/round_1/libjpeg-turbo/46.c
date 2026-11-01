// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_buffers(unsigned char **dstBufs, int n) {
    if (dstBufs) {
        for (int i = 0; i < n; i++) {
            if (dstBufs[i]) {
                free(dstBufs[i]);
                dstBufs[i] = NULL;
            }
        }
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Parse simple control from input to diversify test cases
    int mode = Data[0] % 4; 
    int num_transforms = (Data[1] % 5) + 1; // 1 to 5 transforms

    tjhandle handle = NULL;
    tjhandle transform_handle = NULL;
    unsigned char **dstBufs = NULL;
    size_t *dstSizes = NULL;
    tjtransform *transforms = NULL;
    int ret = 0;

    // Allocate arrays
    dstBufs = (unsigned char **)malloc(num_transforms * sizeof(unsigned char *));
    dstSizes = (size_t *)malloc(num_transforms * sizeof(size_t));
    transforms = (tjtransform *)malloc(num_transforms * sizeof(tjtransform));

    if (!dstBufs || !dstSizes || !transforms) {
        free(dstBufs);
        free(dstSizes);
        free(transforms);
        return 0;
    }

    memset(dstBufs, 0, num_transforms * sizeof(unsigned char *));
    memset(dstSizes, 0, num_transforms * sizeof(size_t));
    memset(transforms, 0, num_transforms * sizeof(tjtransform));

    // Initialize transforms with random valid-ish parameters
    // Ensure we do not read past the buffer
    for (int i = 0; i < num_transforms; i++) {
        // Calculate index safely using modulo Size
        // Ensure indices are within [0, Size-1]
        size_t idx_op = 2 + (i * 10) % (Size - 2);
        size_t idx_opt = 3 + (i * 11) % (Size - 3);
        size_t idx_w = 4 + (i * 12) % (Size - 4);
        size_t idx_h = 5 + (i * 13) % (Size - 5);
        
        transforms[i].op = (Data[idx_op] % 8); // Random transform op
        
        transforms[i].options = (Data[idx_opt] % 4); // Random options
        
        transforms[i].r.w = (Data[idx_w] % 100) + 1;
        transforms[i].r.h = (Data[idx_h] % 100) + 1;
        
        transforms[i].r.x = 0;
        transforms[i].r.y = 0;
        transforms[i].customFilter = NULL;
        transforms[i].data = NULL;
    }

    // Mode 0: Test tj3Transform (requires tj3Init and tj3DecompressHeader)
    if (mode == 0) {
        handle = tj3Init(TJINIT_DECOMPRESS);
        if (!handle) return 0;

        // Try to read header (might fail on invalid JPEG, which is expected)
        ret = tj3DecompressHeader(handle, (const unsigned char *)Data, Size);
        
        if (ret == 0) {
            // If header read succeeded, try to calculate buffer size for one transform
            if (num_transforms > 0) {
                size_t bufSize = tj3TransformBufSize(handle, &transforms[0]);
                if (bufSize > 0) {
                    dstBufs[0] = (unsigned char *)malloc(bufSize);
                    if (dstBufs[0]) {
                        dstSizes[0] = bufSize;
                        // Perform transform
                        ret = tj3Transform(handle, (const unsigned char *)Data, Size, 
                                          num_transforms, dstBufs, dstSizes, transforms);
                        // Cleanup specific to this path
                        cleanup_buffers(dstBufs, num_transforms);
                    }
                }
            } else {
                // Fallback if num_transforms logic failed (unlikely)
                ret = tj3Transform(handle, (const unsigned char *)Data, Size, 1, 
                                  dstBufs, dstSizes, transforms);
            }
        } else {
            // Even if header fails, we can try passing invalid data to tj3Transform to test error handling
            ret = tj3Transform(handle, (const unsigned char *)Data, Size, 
                              num_transforms, dstBufs, dstSizes, transforms);
        }
        
        tj3Destroy(handle);
    }

    // Mode 1: Test tjTransform (Legacy API, requires tjInitTransform)
    else if (mode == 1) {
        transform_handle = tjInitTransform();
        if (!transform_handle) return 0;

        // tjTransform doesn't strictly require header read before call in all cases, 
        // but it's good practice for valid transforms. We skip explicit header read 
        // to test error paths on invalid JPEGs too.
        
        // Pre-allocate a dummy buffer for destination to test reallocation logic
        size_t dummySize = Size * 2; 
        if (dummySize > 0) {
            dstBufs[0] = (unsigned char *)malloc(dummySize);
            if (dstBufs[0]) {
                dstSizes[0] = dummySize;
                ret = tjTransform(transform_handle, (const unsigned char *)Data, (unsigned long)Size,
                                  num_transforms, dstBufs, dstSizes, transforms, 0);
                cleanup_buffers(dstBufs, num_transforms);
            }
        } else {
            // Test with NULL buffers (auto-allocation)
            ret = tjTransform(transform_handle, (const unsigned char *)Data, (unsigned long)Size,
                              num_transforms, dstBufs, dstSizes, transforms, 0);
            cleanup_buffers(dstBufs, num_transforms);
        }

        tjDestroy(transform_handle);
    }

    // Mode 2: Test tj3Init and tj3DecompressHeader only
    else if (mode == 2) {
        handle = tj3Init(TJINIT_DECOMPRESS);
        if (handle) {
            ret = tj3DecompressHeader(handle, (const unsigned char *)Data, Size);
            tj3Destroy(handle);
        }
    }

    // Mode 3: Test tj3TransformBufSize directly after init
    else if (mode == 3) {
        handle = tj3Init(TJINIT_DECOMPRESS);
        if (handle) {
            // Try to read header first
            ret = tj3DecompressHeader(handle, (const unsigned char *)Data, Size);
            if (ret == 0 && num_transforms > 0) {
                size_t size = tj3TransformBufSize(handle, &transforms[0]);
                // Do nothing with size, just ensure no crash
            }
            tj3Destroy(handle);
        }
    }

    // Cleanup
    free(dstBufs);
    free(dstSizes);
    free(transforms);

    return 0;
}