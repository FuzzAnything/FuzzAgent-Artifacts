// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
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
    // Do nothing, just return success
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Split input data for different purposes
    size_t jpeg_size = Size > 100 ? 100 : Size;
    size_t icc_size = Size > jpeg_size ? Size - jpeg_size : 0;
    
    const unsigned char *jpeg_data = Data;
    unsigned char *icc_data = NULL;
    
    if (icc_size > 0) {
        icc_data = (unsigned char *)Data + jpeg_size;
    }
    
    // Test tj3Alloc
    size_t alloc_size = Size % 1024 + 1; // Ensure non-zero
    void *allocated_buffer = tj3Alloc(alloc_size);
    if (allocated_buffer) {
        // Use the buffer minimally
        memset(allocated_buffer, 0, 1);
        tj3Free(allocated_buffer);
    }
    
    // Create handles for different operations
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle trans_handle = tj3Init(TJINIT_TRANSFORM);
    
    if (decomp_handle) {
        // Set parameters for decompression
        tj3Set(decomp_handle, TJPARAM_SAVEMARKERS, 2); // Enable ICC extraction
        
        // Test tj3DecompressHeader
        if (jpeg_size > 0) {
            tj3DecompressHeader(decomp_handle, jpeg_data, jpeg_size);
        }
        
        // Test tj3GetICCProfile
        unsigned char *retrieved_icc = NULL;
        size_t retrieved_icc_size = 0;
        tj3GetICCProfile(decomp_handle, &retrieved_icc, &retrieved_icc_size);
        
        if (retrieved_icc) {
            tj3Free(retrieved_icc);
        }
        
        tj3Destroy(decomp_handle);
    }
    
    if (comp_handle) {
        // Test tj3SetICCProfile
        if (icc_size > 0 && icc_data) {
            tj3SetICCProfile(comp_handle, icc_data, icc_size);
        } else {
            // Test removing ICC profile
            tj3SetICCProfile(comp_handle, NULL, 0);
        }
        
        tj3Destroy(comp_handle);
    }
    
    if (trans_handle && jpeg_size > 0) {
        // First read header to get image info
        tjhandle temp_handle = tj3Init(TJINIT_DECOMPRESS);
        if (temp_handle) {
            tj3Set(temp_handle, TJPARAM_SAVEMARKERS, 2);
            
            if (tj3DecompressHeader(temp_handle, jpeg_data, jpeg_size) == 0) {
                // Get width and height using tj3Get
                int width = tj3Get(temp_handle, TJPARAM_JPEGWIDTH);
                int height = tj3Get(temp_handle, TJPARAM_JPEGHEIGHT);
                
                // Test tj3TransformBufSize
                tjtransform transform;
                memset(&transform, 0, sizeof(transform));
                
                // Set some transform parameters based on input data
                transform.op = (Size % 8); // Various transform operations
                transform.options = (Size % 16);
                
                // Set a valid cropping region if image is large enough
                if (width > 10 && height > 10) {
                    transform.r.x = (Size % (width - 5));
                    transform.r.y = (Size % (height - 5));
                    transform.r.w = 5;
                    transform.r.h = 5;
                } else {
                    transform.r.w = width;
                    transform.r.h = height;
                }
                
                // Set custom filter callback
                transform.customFilter = dummy_custom_filter;
                
                size_t buf_size = tj3TransformBufSize(trans_handle, &transform);
                
                // Test tj3Transform if we have buffer size
                if (buf_size > 0) {
                    // Test with single transform
                    unsigned char *dst_bufs[1];
                    size_t dst_sizes[1];
                    tjtransform transforms[1];
                    
                    dst_bufs[0] = NULL; // Let library allocate
                    dst_sizes[0] = 0;
                    transforms[0] = transform;
                    
                    tj3Transform(trans_handle, jpeg_data, jpeg_size, 1,
                                 dst_bufs, dst_sizes, transforms);
                    
                    // Free allocated buffer if any
                    if (dst_bufs[0]) {
                        tj3Free(dst_bufs[0]);
                    }
                    
                    // Test with pre-allocated buffer
                    if (buf_size < 1024 * 1024) { // Limit allocation size
                        dst_bufs[0] = tj3Alloc(buf_size);
                        if (dst_bufs[0]) {
                            dst_sizes[0] = buf_size;
                            tj3Transform(trans_handle, jpeg_data, jpeg_size, 1,
                                         dst_bufs, dst_sizes, transforms);
                            tj3Free(dst_bufs[0]);
                        }
                    }
                }
            }
            tj3Destroy(temp_handle);
        }
        
        tj3Destroy(trans_handle);
    }
    
    return 0;
}