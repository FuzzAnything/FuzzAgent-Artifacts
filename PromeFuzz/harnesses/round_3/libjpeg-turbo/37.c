// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
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
    // Simple filter that does nothing
    (void)coeffs;
    (void)arrayRegion;
    (void)planeRegion;
    (void)componentID;
    (void)transformID;
    (void)transform;
    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    // Split input into parts for different parameters
    size_t jpeg_size = Size * 3 / 4;
    if (jpeg_size > Size) jpeg_size = Size;
    
    const unsigned char *jpeg_buf = Data;
    unsigned long jpeg_size_long = (unsigned long)jpeg_size;
    
    // Create transform handle
    tjhandle transform_handle = tjInitTransform();
    if (!transform_handle) {
        return 0;
    }

    // Test tjDecompressHeader
    int width = 0, height = 0;
    int header_result = tjDecompressHeader(transform_handle, 
                                           (unsigned char*)jpeg_buf, 
                                           jpeg_size_long, 
                                           &width, &height);
    
    // Only proceed with transforms if we have valid header
    if (header_result == 0 && width > 0 && height > 0) {
        // Create transform parameters using remaining input data
        size_t param_offset = jpeg_size;
        size_t remaining = Size - jpeg_size;
        
        // Set up transform structure
        tjtransform transform;
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = width / 2;
        transform.r.h = height / 2;
        
        // Use input bytes to determine operation and options
        if (remaining > 0) {
            transform.op = Data[param_offset] % 8;  // TJXOP values are 0-7
            transform.options = Data[param_offset] | TJXOPT_PERFECT;
        } else {
            transform.op = 0;  // TJXOP_NONE
            transform.options = TJXOPT_PERFECT;
        }
        
        // Randomly decide whether to use custom filter
        if (remaining > 1 && (Data[param_offset + 1] % 2)) {
            transform.customFilter = dummy_custom_filter;
        } else {
            transform.customFilter = NULL;
        }
        transform.data = NULL;

        // Test tj3TransformBufSize
        size_t buf_size = tj3TransformBufSize(transform_handle, &transform);
        
        if (buf_size > 0) {
            // Test tj3Alloc
            unsigned char *dst_buf = (unsigned char*)tj3Alloc(buf_size);
            if (dst_buf) {
                size_t dst_size = buf_size;
                
                // Test tj3Transform
                int n = 1;
                unsigned char *dst_bufs[1] = {dst_buf};
                size_t dst_sizes[1] = {dst_size};
                tjtransform transforms[1] = {transform};
                
                int transform_result = tj3Transform(transform_handle,
                                                    jpeg_buf,
                                                    jpeg_size,
                                                    n,
                                                    dst_bufs,
                                                    dst_sizes,
                                                    transforms);
                
                // Clean up allocated buffer
                if (dst_bufs[0] && dst_bufs[0] != dst_buf) {
                    // Buffer was reallocated by tj3Transform
                    free(dst_bufs[0]);
                } else {
                    free(dst_buf);
                }
                (void)transform_result;
            }
        }

        // Test old tjTransform API
        unsigned char *old_dst_buf = NULL;
        unsigned long old_dst_size = 0;
        int n = 1;
        
        int old_transform_result = tjTransform(transform_handle,
                                               jpeg_buf,
                                               jpeg_size_long,
                                               n,
                                               &old_dst_buf,
                                               &old_dst_size,
                                               &transform,
                                               TJXOPT_PERFECT);
        
        if (old_dst_buf) {
            free(old_dst_buf);
        }
        (void)old_transform_result;
    }

    // Clean up
    tjDestroy(transform_handle);
    
    return 0;
}