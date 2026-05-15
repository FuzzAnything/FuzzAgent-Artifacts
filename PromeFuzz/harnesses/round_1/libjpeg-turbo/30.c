// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static int dummy_custom_filter(short *coeffs, tjregion arrayRegion,
                               tjregion planeRegion, int componentID,
                               int transformID, struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Test tj3Alloc and tj3Free
    if (Size > 0) {
        size_t alloc_size = Size % 1024;
        void *allocated_buffer = tj3Alloc(alloc_size);
        if (allocated_buffer) {
            memset(allocated_buffer, 0, alloc_size);
            tj3Free(allocated_buffer);
        }
    }

    // Test tjInitTransform
    tjhandle transform_handle = tjInitTransform();
    if (!transform_handle) {
        return 0;
    }

    // Write input data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Test tj3TransformBufSize and tj3Transform if we have enough data
    if (Size >= sizeof(tjtransform) + 100) {
        tjtransform transform;
        memcpy(&transform, Data, sizeof(tjtransform) > Size ? Size : sizeof(tjtransform));
        
        transform.op = transform.op % 8;
        transform.options = transform.options & (TJXOPT_PERFECT | TJXOPT_TRIM | TJXOPT_CROP | TJXOPT_GRAY | TJXOPT_NOOUTPUT);
        
        if (Size > sizeof(tjtransform) + 50) {
            transform.customFilter = dummy_custom_filter;
        } else {
            transform.customFilter = NULL;
        }
        
        size_t buf_size = tj3TransformBufSize(transform_handle, &transform);
        
        if (buf_size > 0 && buf_size < 10 * 1024 * 1024) {
            void *transform_buffer = tj3Alloc(buf_size);
            if (transform_buffer) {
                size_t dst_size = buf_size;
                unsigned char *dst_buf = (unsigned char *)transform_buffer;
                
                int result = tj3Transform(transform_handle, Data, 
                                         Size > buf_size ? buf_size : Size,
                                         1, &dst_buf, &dst_size, &transform);
                
                tj3Free(transform_buffer);
            }
        }
    }

    // Test tjTransform (legacy API) if we have enough data
    if (Size >= sizeof(tjtransform) + 200) {
        tjtransform transforms[1];
        memcpy(&transforms[0], Data, sizeof(tjtransform) > Size ? Size : sizeof(tjtransform));
        
        transforms[0].op = transforms[0].op % 8;
        transforms[0].options = transforms[0].options & (TJXOPT_PERFECT | TJXOPT_TRIM | TJXOPT_CROP | TJXOPT_GRAY | TJXOPT_NOOUTPUT);
        
        if (Size > sizeof(tjtransform) + 100) {
            transforms[0].customFilter = dummy_custom_filter;
        } else {
            transforms[0].customFilter = NULL;
        }
        
        unsigned char *dst_bufs[1] = {NULL};
        unsigned long dst_sizes[1] = {0};
        
        int legacy_result = tjTransform(transform_handle, Data,
                                       Size > 65535 ? 65535 : (unsigned long)Size,
                                       1, dst_bufs, dst_sizes, transforms,
                                       TJXOPT_PERFECT);
        
        if (dst_bufs[0]) {
            tj3Free(dst_bufs[0]);
        }
    }

    // Clean up dummy file
    remove("./dummy_file");
    
    // Destroy the transform handle to fix memory leak
    // tjDestroy is the proper function to free the handle created by tjInitTransform
    // This function is available in the turbojpeg.h header
    tjDestroy(transform_handle);
    
    return 0;
}