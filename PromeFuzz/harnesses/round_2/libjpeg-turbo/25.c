// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
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
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
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
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Split input for JPEG and ICC data
    size_t split = Data[0] % (Size + 1);
    const uint8_t *jpeg_data = Data;
    size_t jpeg_size = split;
    const uint8_t *icc_data = Data + split;
    size_t icc_size = Size - split;

    // Initialize handles
    tjhandle decompressor = tj3Init(TJINIT_DECOMPRESS);
    tjhandle compressor = tj3Init(TJINIT_COMPRESS);
    tjhandle transformer = tj3Init(TJINIT_TRANSFORM);

    if (!decompressor || !compressor || !transformer) {
        if (decompressor) tj3Destroy(decompressor);
        if (compressor) tj3Destroy(compressor);
        if (transformer) tj3Destroy(transformer);
        return 0;
    }

    // Test tj3Alloc
    size_t alloc_size = Size % 1024;
    void *allocated_buffer = tj3Alloc(alloc_size);
    if (allocated_buffer) {
        tj3Free(allocated_buffer);
    }

    // Test tj3DecompressHeader
    int header_result = tj3DecompressHeader(decompressor, jpeg_data, jpeg_size);
    
    // Set save markers to extract ICC if header was successful
    if (header_result == 0) {
        tj3Set(decompressor, TJPARAM_SAVEMARKERS, 2);
        // Try again with the same data to extract ICC
        tj3DecompressHeader(decompressor, jpeg_data, jpeg_size);
    }

    // Test tj3SetICCProfile
    unsigned char *icc_copy = NULL;
    if (icc_size > 0) {
        icc_copy = (unsigned char *)malloc(icc_size);
        if (icc_copy) {
            memcpy(icc_copy, icc_data, icc_size);
            tj3SetICCProfile(compressor, icc_copy, icc_size);
            free(icc_copy);
        }
    } else {
        tj3SetICCProfile(compressor, NULL, 0);
    }

    // Test tj3GetICCProfile
    unsigned char *retrieved_icc = NULL;
    size_t retrieved_size = 0;
    tj3GetICCProfile(decompressor, &retrieved_icc, &retrieved_size);
    if (retrieved_icc) {
        tj3Free(retrieved_icc);
    }

    // Test tj3TransformBufSize and tj3Transform if we have valid JPEG data
    if (header_result == 0 && jpeg_size > 0) {
        // Read header with transformer handle
        tj3DecompressHeader(transformer, jpeg_data, jpeg_size);
        
        // Create transform structure
        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        
        // Use fuzz data to set transform parameters
        if (jpeg_size >= sizeof(transform)) {
            memcpy(&transform, jpeg_data, sizeof(transform) < jpeg_size ? sizeof(transform) : jpeg_size);
        }
        
        // Ensure reasonable values
        transform.op = transform.op % 8;
        transform.options = transform.options % 32;
        transform.customFilter = dummy_custom_filter;
        
        // Get buffer size
        size_t buf_size = tj3TransformBufSize(transformer, &transform);
        
        if (buf_size > 0 && buf_size < 100 * 1024 * 1024) { // Limit to 100MB
            // Test with pre-allocated buffer
            unsigned char *dst_buf = tj3Alloc(buf_size);
            size_t dst_size = buf_size;
            
            if (dst_buf) {
                int n = 1;
                unsigned char *dst_bufs[1] = {dst_buf};
                size_t dst_sizes[1] = {dst_size};
                tjtransform transforms[1] = {transform};
                
                tj3Transform(transformer, jpeg_data, jpeg_size, n, dst_bufs, dst_sizes, transforms);
                
                tj3Free(dst_buf);
            }
            
            // Test with NULL buffer (auto-allocation)
            unsigned char *auto_buf = NULL;
            size_t auto_size = 0;
            unsigned char *auto_bufs[1] = {auto_buf};
            size_t auto_sizes[1] = {auto_size};
            
            tj3Transform(transformer, jpeg_data, jpeg_size, 1, auto_bufs, auto_sizes, &transform);
            
            if (auto_bufs[0]) {
                tj3Free(auto_bufs[0]);
            }
        }
    }

    // Cleanup
    tj3Destroy(decompressor);
    tj3Destroy(compressor);
    tj3Destroy(transformer);

    return 0;
}