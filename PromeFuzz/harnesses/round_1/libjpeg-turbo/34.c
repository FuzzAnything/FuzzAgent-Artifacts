// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static int customFilter(short *coeffs, tjregion arrayRegion,
                       tjregion planeRegion, int componentID, int transformID,
                       struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Test tj3Alloc
    size_t alloc_size = (Data[0] * 256 + Data[1]) % 4096 + 1;
    void *allocated = tj3Alloc(alloc_size);
    if (allocated) {
        size_t copy_size = alloc_size < Size ? alloc_size : Size;
        memcpy(allocated, Data, copy_size);
        tj3Free(allocated);
    }

    // Initialize handles
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle transform_handle = tj3Init(TJINIT_TRANSFORM);

    if (!compress_handle || !decompress_handle || !transform_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        if (transform_handle) tj3Destroy(transform_handle);
        return 0;
    }

    // Test tj3SetICCProfile
    size_t icc_size = Size % 1024;
    unsigned char *icc_buf = icc_size > 0 ? (unsigned char *)Data : NULL;
    tj3SetICCProfile(compress_handle, icc_buf, icc_size);
    tj3SetICCProfile(compress_handle, NULL, 0);

    // Test tj3DecompressHeader
    tj3Set(decompress_handle, TJPARAM_SAVEMARKERS, 2);
    int header_result = tj3DecompressHeader(decompress_handle, Data, Size);

    if (header_result == 0) {
        // Test tj3GetICCProfile
        unsigned char *retrieved_icc = NULL;
        size_t retrieved_size = 0;
        tj3GetICCProfile(decompress_handle, &retrieved_icc, &retrieved_size);
        if (retrieved_icc) {
            tj3Free(retrieved_icc);
        }

        // Get image parameters
        int width = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
        int height = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
        int subsamp = tj3Get(decompress_handle, TJPARAM_SUBSAMP);
        int colorspace = tj3Get(decompress_handle, TJPARAM_COLORSPACE);

        if (width > 0 && height > 0) {
            // Prepare transform structure
            tjtransform transform;
            memset(&transform, 0, sizeof(transform));
            
            transform.op = Data[0] % 8;
            transform.options = (Data[1] & 1) ? TJXOPT_TRIM : 0;
            transform.customFilter = customFilter;
            
            // Set cropping region if we have enough data
            if (Size >= 8) {
                transform.r.x = (Data[2] << 8) | Data[3];
                transform.r.y = (Data[4] << 8) | Data[5];
                transform.r.w = (Data[6] << 8) | Data[7];
                transform.r.h = (Data[0] << 8) | Data[1];
                
                // Clamp to image dimensions
                if (transform.r.x >= width) transform.r.x = width - 1;
                if (transform.r.y >= height) transform.r.y = height - 1;
                if (transform.r.w > width - transform.r.x) transform.r.w = width - transform.r.x;
                if (transform.r.h > height - transform.r.y) transform.r.h = height - transform.r.y;
            }

            // Test tj3TransformBufSize
            size_t buf_size = tj3TransformBufSize(transform_handle, &transform);
            
            if (buf_size > 0) {
                // Test tj3Transform with pre-allocated buffer
                unsigned char *dst_buf = tj3Alloc(buf_size);
                size_t dst_size = buf_size;
                
                if (dst_buf) {
                    tj3Transform(transform_handle, Data, Size, 1, 
                                &dst_buf, &dst_size, &transform);
                    tj3Free(dst_buf);
                }

                // Test tj3Transform with auto-allocation
                if (Size > 16) {
                    tjtransform transforms[2];
                    unsigned char *dst_bufs[2] = {NULL, NULL};
                    size_t dst_sizes[2] = {0, 0};
                    
                    for (int i = 0; i < 2; i++) {
                        transforms[i] = transform;
                        transforms[i].op = (Data[i] % 8);
                    }
                    
                    tj3Transform(transform_handle, Data, Size, 2, 
                                dst_bufs, dst_sizes, transforms);
                    
                    for (int i = 0; i < 2; i++) {
                        if (dst_bufs[i]) {
                            tj3Free(dst_bufs[i]);
                        }
                    }
                }
            }
        }
    }

    // Clean up
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    tj3Destroy(transform_handle);

    return 0;
}