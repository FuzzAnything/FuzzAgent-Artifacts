// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "turbojpeg.h"

static int dummy_custom_filter(short *coeffs, tjregion arrayRegion,
                               tjregion planeRegion, int componentID,
                               int transformID, struct tjtransform *transform) {
    // Do nothing - just a dummy callback
    return 0;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Initialize handles for different operations
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle trans_handle = tj3Init(TJINIT_TRANSFORM);
    
    if (!decomp_handle || !comp_handle || !trans_handle) {
        if (decomp_handle) tj3Destroy(decomp_handle);
        if (comp_handle) tj3Destroy(comp_handle);
        if (trans_handle) tj3Destroy(trans_handle);
        return 0;
    }
    
    // Test tj3Alloc with various sizes
    size_t alloc_size = Size > 0 ? (Data[0] % 1024) + 1 : 1;
    void *allocated_buffer = tj3Alloc(alloc_size);
    if (allocated_buffer) {
        // Try to use the buffer
        memset(allocated_buffer, 0, alloc_size);
        tj3Free(allocated_buffer);
    }
    
    // Test tj3DecompressHeader with the input data
    if (Size > 0) {
        int header_result = tj3DecompressHeader(decomp_handle, Data, Size);
        
        // If header decompression succeeded, try to get ICC profile
        if (header_result == 0) {
            unsigned char *icc_buf = NULL;
            size_t icc_size = 0;
            
            // First get just the size
            int get_icc_result1 = tj3GetICCProfile(decomp_handle, NULL, &icc_size);
            
            // Then try to get the actual profile
            if (icc_size > 0) {
                int get_icc_result2 = tj3GetICCProfile(decomp_handle, &icc_buf, &icc_size);
                if (icc_buf) {
                    tj3Free(icc_buf);
                }
            }
        }
    }
    
    // Test tj3SetICCProfile with compression handle
    if (Size > 10) {
        size_t icc_data_size = Size > 100 ? 100 : Size / 2;
        unsigned char *icc_data = (unsigned char *)Data;
        
        // Test setting ICC profile
        int set_icc_result = tj3SetICCProfile(comp_handle, icc_data, icc_data_size);
        
        // Test removing ICC profile
        if (set_icc_result == 0) {
            tj3SetICCProfile(comp_handle, NULL, 0);
        }
    }
    
    // Test tj3Transform and tj3TransformBufSize
    if (Size > 50) {
        // First read header for transformation
        tj3DecompressHeader(trans_handle, Data, Size);
        
        // Prepare transform structure
        tjtransform transform;
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = 100;
        transform.r.h = 100;
        transform.op = TJXOP_NONE;
        transform.options = 0;
        transform.data = NULL;
        transform.customFilter = dummy_custom_filter;
        
        // Calculate buffer size needed
        size_t buf_size = tj3TransformBufSize(trans_handle, &transform);
        
        if (buf_size > 0 && buf_size < 10 * 1024 * 1024) { // Sanity check
            // Allocate buffer for transformed image
            unsigned char *dst_buf = tj3Alloc(buf_size);
            size_t dst_size = buf_size;
            
            if (dst_buf) {
                unsigned char *dst_bufs[1] = {dst_buf};
                size_t dst_sizes[1] = {dst_size};
                
                // Try to transform
                int transform_result = tj3Transform(trans_handle, Data, Size, 1,
                                                   dst_bufs, dst_sizes, &transform);
                
                tj3Free(dst_buf);
            }
        }
        
        // Test with NULL destination buffer (let library allocate)
        if (Size > 100) {
            unsigned char *dst_bufs[1] = {NULL};
            size_t dst_sizes[1] = {0};
            
            int transform_result = tj3Transform(trans_handle, Data, Size, 1,
                                               dst_bufs, dst_sizes, &transform);
            
            if (dst_bufs[0]) {
                tj3Free(dst_bufs[0]);
            }
        }
    }
    
    // Clean up
    tj3Destroy(decomp_handle);
    tj3Destroy(comp_handle);
    tj3Destroy(trans_handle);
    
    return 0;
}