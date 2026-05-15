// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_custom_filter(short *coeffs, tjregion arrayRegion,
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

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Ensure we have minimum data for basic operations
    if (Size < 100) {
        return 0;
    }

    // Initialize handles
    tjhandle transform_handle = tjInitTransform();
    if (!transform_handle) {
        return 0;
    }

    tjhandle decompress_handle = tjInitDecompress();
    if (!decompress_handle) {
        tjDestroy(transform_handle);
        return 0;
    }

    // Try to decompress header first
    int width = 0, height = 0;
    unsigned long jpeg_size = Size > 1024 * 1024 ? 1024 * 1024 : Size;
    unsigned char *jpeg_buf = (unsigned char *)Data;
    
    int header_result = tjDecompressHeader(decompress_handle, jpeg_buf, jpeg_size, &width, &height);
    
    // Prepare transform structures
    tjtransform transforms[2];
    memset(transforms, 0, sizeof(transforms));
    
    // Set up first transform with some parameters from fuzz data
    transforms[0].r.x = Data[0] % 256;
    transforms[0].r.y = Data[1] % 256;
    transforms[0].r.w = Data[2] % 256;
    transforms[0].r.h = Data[3] % 256;
    transforms[0].op = Data[4] % 8;  // TJXOP values 0-7
    transforms[0].options = TJXOPT_PERFECT | (Data[5] & 0x0F);
    transforms[0].customFilter = dummy_custom_filter;
    
    // Set up second transform
    transforms[1].r.x = Data[6] % 256;
    transforms[1].r.y = Data[7] % 256;
    transforms[1].r.w = Data[8] % 256;
    transforms[1].r.h = Data[9] % 256;
    transforms[1].op = Data[10] % 8;
    transforms[1].options = TJXOPT_PERFECT | (Data[11] & 0x0F);
    transforms[1].customFilter = NULL;

    // Test tj3TransformBufSize if header was successfully read
    if (header_result == 0 && width > 0 && height > 0) {
        size_t buf_size = tj3TransformBufSize(transform_handle, &transforms[0]);
        if (buf_size > 0) {
            // Allocate buffer using tj3Alloc
            void *allocated_buf = tj3Alloc(buf_size);
            if (allocated_buf) {
                tj3Free(allocated_buf);
            }
        }
    }

    // Test tj3Transform with various buffer allocation strategies
    const int num_transforms = 2;
    unsigned char *dst_bufs[2] = {NULL, NULL};
    size_t dst_sizes[2] = {0, 0};
    
    // Strategy 1: Let library allocate buffers
    int result1 = tj3Transform(transform_handle, jpeg_buf, jpeg_size,
                               num_transforms, dst_bufs, dst_sizes, transforms);
    
    // Free buffers allocated by library
    for (int i = 0; i < num_transforms; i++) {
        if (dst_bufs[i]) {
            tj3Free(dst_bufs[i]);
            dst_bufs[i] = NULL;
        }
    }
    
    // Strategy 2: Pre-allocate buffers using tj3Alloc
    for (int i = 0; i < num_transforms; i++) {
        dst_sizes[i] = 1024 * 1024;  // 1MB initial buffer
        dst_bufs[i] = (unsigned char *)tj3Alloc(dst_sizes[i]);
    }
    
    int result2 = tj3Transform(transform_handle, jpeg_buf, jpeg_size,
                               num_transforms, dst_bufs, dst_sizes, transforms);
    
    // Free pre-allocated buffers
    for (int i = 0; i < num_transforms; i++) {
        if (dst_bufs[i]) {
            tj3Free(dst_bufs[i]);
            dst_bufs[i] = NULL;
        }
    }
    
    // Test old tjTransform API
    unsigned char *old_dst_bufs[2] = {NULL, NULL};
    unsigned long old_dst_sizes[2] = {0, 0};
    
    int result3 = tjTransform(transform_handle, jpeg_buf, jpeg_size,
                              num_transforms, old_dst_bufs, old_dst_sizes,
                              transforms, TJXOPT_PERFECT);
    
    // Free buffers allocated by old API
    for (int i = 0; i < num_transforms; i++) {
        if (old_dst_bufs[i]) {
            tjFree(old_dst_bufs[i]);
        }
    }

    // Clean up
    tjDestroy(transform_handle);
    tjDestroy(decompress_handle);
    
    return 0;
}