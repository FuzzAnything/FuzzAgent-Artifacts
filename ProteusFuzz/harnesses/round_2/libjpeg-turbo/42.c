// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
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

static void free_transforms(tjtransform *transforms, int n) {
    if (transforms) {
        for (int i = 0; i < n; i++) {
            // If custom data was allocated, it would be freed here.
            // In this fuzzer, we don't allocate custom data, so just free the struct array.
        }
        free(transforms);
    }
}

static void free_dst_bufs(unsigned char **dstBufs, int n) {
    if (dstBufs) {
        for (int i = 0; i < n; i++) {
            if (dstBufs[i]) {
                tj3Free(dstBufs[i]);
                dstBufs[i] = NULL;
            }
        }
        free(dstBufs);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Parse input to determine operation mode
    int mode = Data[0] % 4;
    size_t jpeg_start = 10;
    size_t jpeg_size = Size - jpeg_start;

    if (jpeg_size == 0) return 0;

    tjhandle handle = NULL;
    tjhandle transform_handle = NULL;
    tjtransform *transforms = NULL;
    unsigned char **dstBufs = NULL;
    size_t *dstSizes = NULL;
    int n = 1;
    int ret = 0;

    // Ensure we have enough data for at least one transform if needed
    if (mode == 1 || mode == 2 || mode == 3) {
        n = (Size > 11) ? (Data[1] % 5) + 1 : 1;
        if (n > 10) n = 10; // Cap number of transforms
    }

    // Initialize handles based on mode
    if (mode == 0) {
        // Test tj3DecompressHeader
        handle = tj3Init(TJINIT_DECOMPRESS);
        if (handle) {
            ret = tj3DecompressHeader(handle, (const unsigned char *)Data + jpeg_start, jpeg_size);
            (void)ret; // Ignore return for fuzzing, just check for crashes
            tj3Destroy(handle);
        }
        return 0;
    } else if (mode == 1) {
        // Test tjInitTransform and tj3TransformBufSize
        transform_handle = tjInitTransform();
        if (!transform_handle) return 0;

        // Prepare transform structure
        transforms = (tjtransform *)calloc(n, sizeof(tjtransform));
        if (!transforms) {
            tjDestroy(transform_handle);
            return 0;
        }

        // Initialize transform parameters from fuzz data
        for (int i = 0; i < n; i++) {
            transforms[i].op = (Data[2 + i] % 8); // TJXOP_ROT90, etc.
            transforms[i].options = (Data[3 + i] % 16);
            transforms[i].r.w = (Data[4 + i] % 100) + 1;
            transforms[i].r.h = (Data[5 + i] % 100) + 1;
            transforms[i].r.x = 0;
            transforms[i].r.y = 0;
            transforms[i].customFilter = NULL;
            transforms[i].data = NULL;
        }

        // First, try to read header to prime the transform handle
        // Note: tjInitTransform creates a handle, but we need to feed it a header first
        // to get valid dimensions for tj3TransformBufSize.
        // We can use tj3DecompressHeader on the transform handle? 
        // Actually, tjInitTransform returns a handle that can be used with tj3DecompressHeader.
        ret = tj3DecompressHeader(transform_handle, (const unsigned char *)Data + jpeg_start, jpeg_size);
        
        if (ret == 0) {
            // Try to get buffer size
            for (int i = 0; i < n; i++) {
                size_t buf_size = tj3TransformBufSize(transform_handle, &transforms[i]);
                (void)buf_size;
            }
        }

        free_transforms(transforms, n);
        tjDestroy(transform_handle);
        return 0;
    } else if (mode == 2) {
        // Test tj3Transform
        transform_handle = tjInitTransform();
        if (!transform_handle) return 0;

        // Read header first
        ret = tj3DecompressHeader(transform_handle, (const unsigned char *)Data + jpeg_start, jpeg_size);
        if (ret != 0) {
            tjDestroy(transform_handle);
            return 0;
        }

        // Allocate transforms
        transforms = (tjtransform *)calloc(n, sizeof(tjtransform));
        if (!transforms) {
            tjDestroy(transform_handle);
            return 0;
        }

        for (int i = 0; i < n; i++) {
            transforms[i].op = (Data[2 + i] % 8);
            transforms[i].options = (Data[3 + i] % 16);
            // Crop region logic: ensure it's valid relative to image if possible, 
            // but fuzzing often sends invalid crops to test robustness.
            transforms[i].r.w = (Data[4 + i] % 200) + 1;
            transforms[i].r.h = (Data[5 + i] % 200) + 1;
            transforms[i].r.x = 0;
            transforms[i].r.y = 0;
            transforms[i].customFilter = NULL;
            transforms[i].data = NULL;
        }

        // Allocate destination buffers pointers
        dstBufs = (unsigned char **)calloc(n, sizeof(unsigned char *));
        dstSizes = (size_t *)calloc(n, sizeof(size_t));
        
        if (!dstBufs || !dstSizes) {
            free_transforms(transforms, n);
            free(dstBufs);
            free(dstSizes);
            tjDestroy(transform_handle);
            return 0;
        }

        // Set dstBufs to NULL to let TurboJPEG allocate
        for (int i = 0; i < n; i++) {
            dstBufs[i] = NULL;
            dstSizes[i] = 0;
        }

        int flags = (Data[6] % 4); // TJXOPT_CROP, etc.
        
        ret = tj3Transform(transform_handle, 
                           (const unsigned char *)Data + jpeg_start, 
                           jpeg_size, 
                           n, 
                           dstBufs, 
                           dstSizes, 
                           transforms);
        (void)ret;

        // Cleanup
        free_dst_bufs(dstBufs, n);
        free(dstBufs);
        free(dstSizes);
        free_transforms(transforms, n);
        tjDestroy(transform_handle);
        return 0;
    } else {
        // Mode 3: Test legacy tjTransform if available, or just reuse tj3Transform logic
        // Since tjTransform is legacy, we'll stick to tj3Transform but with different flags
        transform_handle = tjInitTransform();
        if (!transform_handle) return 0;

        ret = tj3DecompressHeader(transform_handle, (const unsigned char *)Data + jpeg_start, jpeg_size);
        if (ret != 0) {
            tjDestroy(transform_handle);
            return 0;
        }

        transforms = (tjtransform *)calloc(n, sizeof(tjtransform));
        if (!transforms) {
            tjDestroy(transform_handle);
            return 0;
        }

        for (int i = 0; i < n; i++) {
            transforms[i].op = (Data[2 + i] % 8);
            transforms[i].options = (Data[3 + i] % 16);
            transforms[i].r.w = (Data[4 + i] % 200) + 1;
            transforms[i].r.h = (Data[5 + i] % 200) + 1;
            transforms[i].r.x = 0;
            transforms[i].r.y = 0;
            transforms[i].customFilter = NULL;
            transforms[i].data = NULL;
        }

        dstBufs = (unsigned char **)calloc(n, sizeof(unsigned char *));
        dstSizes = (size_t *)calloc(n, sizeof(size_t));
        
        if (!dstBufs || !dstSizes) {
            free_transforms(transforms, n);
            free(dstBufs);
            free(dstSizes);
            tjDestroy(transform_handle);
            return 0;
        }

        for (int i = 0; i < n; i++) {
            dstBufs[i] = NULL;
            dstSizes[i] = 0;
        }

        // Use TJPARAM_NOREALLOC or other flags
        int flags = (Data[6] % 4) | TJXOPT_CROP; 
        
        ret = tj3Transform(transform_handle, 
                           (const unsigned char *)Data + jpeg_start, 
                           jpeg_size, 
                           n, 
                           dstBufs, 
                           dstSizes, 
                           transforms);
        (void)ret;

        free_dst_bufs(dstBufs, n);
        free(dstBufs);
        free(dstSizes);
        free_transforms(transforms, n);
        tjDestroy(transform_handle);
        return 0;
    }
}