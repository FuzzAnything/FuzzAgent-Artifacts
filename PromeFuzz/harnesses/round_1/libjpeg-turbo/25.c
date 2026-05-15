// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TEST_WIDTH 16
#define TEST_HEIGHT 16
#define TEST_PIXEL_SIZE 3
#define TEST_PIXEL_FORMAT TJPF_RGB
#define TEST_SUBSAMP TJSAMP_420
#define TEST_FLAGS 0
#define TEST_ALIGN 4

static tjhandle init_compress_handle() {
    return tjInitCompress();
}

static tjhandle init_decompress_handle() {
    return tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static size_t calculate_yuv_buffer_size(int width, int height, int subsamp) {
    return tjBufSizeYUV(width, height, subsamp);
}

static size_t calculate_yuv_plane_size(int componentID, int width, int height, int subsamp) {
    return tjPlaneSizeYUV(componentID, width, 0, height, subsamp);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Initialize handles
    tjhandle compress_handle = init_compress_handle();
    tjhandle decompress_handle = init_decompress_handle();
    
    if (!compress_handle || !decompress_handle) {
        cleanup_handle(compress_handle);
        cleanup_handle(decompress_handle);
        return 0;
    }
    
    // Set up test dimensions
    int width = TEST_WIDTH;
    int height = TEST_HEIGHT;
    int pitch = width * TEST_PIXEL_SIZE;
    int pixelSize = TEST_PIXEL_SIZE;
    int pixelFormat = TEST_PIXEL_FORMAT;
    int subsamp = TEST_SUBSAMP;
    int flags = TEST_FLAGS;
    int align = TEST_ALIGN;
    
    // Calculate required buffer sizes
    size_t src_buf_size = pitch * height;
    size_t yuv_buf_size = calculate_yuv_buffer_size(width, height, subsamp);
    
    // Check if we have enough data
    if (Size < src_buf_size) {
        cleanup_handle(compress_handle);
        cleanup_handle(decompress_handle);
        return 0;
    }
    
    // Allocate and set up source buffer
    unsigned char *src_buf = (unsigned char *)malloc(src_buf_size);
    if (!src_buf) {
        cleanup_handle(compress_handle);
        cleanup_handle(decompress_handle);
        return 0;
    }
    memcpy(src_buf, Data, src_buf_size);
    
    // Test tjEncodeYUV
    {
        unsigned char *dst_buf = (unsigned char *)malloc(yuv_buf_size);
        if (dst_buf) {
            tjEncodeYUV(compress_handle, src_buf, width, pitch, height, 
                       pixelSize, dst_buf, subsamp, flags);
            free(dst_buf);
        }
    }
    
    // Test tj3EncodeYUVPlanes8
    {
        // Calculate plane sizes
        size_t y_plane_size = calculate_yuv_plane_size(0, width, height, subsamp);
        size_t u_plane_size = calculate_yuv_plane_size(1, width, height, subsamp);
        size_t v_plane_size = calculate_yuv_plane_size(2, width, height, subsamp);
        
        unsigned char *y_plane = (unsigned char *)malloc(y_plane_size);
        unsigned char *u_plane = (unsigned char *)malloc(u_plane_size);
        unsigned char *v_plane = (unsigned char *)malloc(v_plane_size);
        
        if (y_plane && u_plane && v_plane) {
            unsigned char *dst_planes[3] = {y_plane, u_plane, v_plane};
            int strides[3] = {0, 0, 0};
            
            tj3EncodeYUVPlanes8(compress_handle, src_buf, width, pitch, height,
                               pixelFormat, dst_planes, strides);
        }
        
        free(y_plane);
        free(u_plane);
        free(v_plane);
    }
    
    // Test tjEncodeYUVPlanes
    {
        size_t y_plane_size = calculate_yuv_plane_size(0, width, height, subsamp);
        size_t u_plane_size = calculate_yuv_plane_size(1, width, height, subsamp);
        size_t v_plane_size = calculate_yuv_plane_size(2, width, height, subsamp);
        
        unsigned char *y_plane = (unsigned char *)malloc(y_plane_size);
        unsigned char *u_plane = (unsigned char *)malloc(u_plane_size);
        unsigned char *v_plane = (unsigned char *)malloc(v_plane_size);
        
        if (y_plane && u_plane && v_plane) {
            unsigned char *dst_planes[3] = {y_plane, u_plane, v_plane};
            int strides[3] = {0, 0, 0};
            
            tjEncodeYUVPlanes(compress_handle, src_buf, width, pitch, height,
                             pixelFormat, dst_planes, strides, subsamp, flags);
        }
        
        free(y_plane);
        free(u_plane);
        free(v_plane);
    }
    
    // Test tjEncodeYUV3
    {
        unsigned char *dst_buf = (unsigned char *)malloc(yuv_buf_size);
        if (dst_buf) {
            tjEncodeYUV3(compress_handle, src_buf, width, pitch, height,
                        pixelFormat, dst_buf, align, subsamp, flags);
            free(dst_buf);
        }
    }
    
    // Test tjEncodeYUV2
    {
        unsigned char *dst_buf = (unsigned char *)malloc(yuv_buf_size);
        if (dst_buf) {
            tjEncodeYUV2(compress_handle, src_buf, width, pitch, height,
                        pixelFormat, dst_buf, subsamp, flags);
            free(dst_buf);
        }
    }
    
    // Test tjDecodeYUV - first encode some YUV data
    {
        unsigned char *yuv_buf = (unsigned char *)malloc(yuv_buf_size);
        if (yuv_buf) {
            // Encode to get YUV data
            if (tjEncodeYUV2(compress_handle, src_buf, width, pitch, height,
                            pixelFormat, yuv_buf, subsamp, flags) == 0) {
                // Now decode it back
                unsigned char *decoded_buf = (unsigned char *)malloc(src_buf_size);
                if (decoded_buf) {
                    tjDecodeYUV(decompress_handle, yuv_buf, align, subsamp,
                               decoded_buf, width, pitch, height, pixelFormat, flags);
                    free(decoded_buf);
                }
            }
            free(yuv_buf);
        }
    }
    
    // Clean up
    free(src_buf);
    cleanup_handle(compress_handle);
    cleanup_handle(decompress_handle);
    
    return 0;
}