// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "turbojpeg.h"

#define MAX_WIDTH 1024
#define MAX_HEIGHT 1024
#define MAX_PIXEL_SIZE 4

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static void cleanup_buffers(unsigned char** buffers, int count) {
    for (int i = 0; i < count; i++) {
        if (buffers[i]) {
            free(buffers[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Initialize handles
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    
    if (!compress_handle || !decompress_handle) {
        cleanup_handle(compress_handle);
        cleanup_handle(decompress_handle);
        return 0;
    }

    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixelFormat = Data[4] % 12;  // TJPF values 0-11
    int subsamp = Data[5] % 5;      // TJSAMP values 0-4
    int align = 1 << (Data[6] % 4); // 1, 2, 4, or 8
    int flags = Data[7];
    
    // Clamp dimensions to reasonable values
    if (width <= 0 || width > MAX_WIDTH) width = 64;
    if (height <= 0 || height > MAX_HEIGHT) height = 64;
    if (align <= 0) align = 1;
    
    int pitch = width * tjPixelSize[pixelFormat];
    if (pitch <= 0) pitch = width * 3; // Default to RGB
    
    // Calculate source buffer size needed
    size_t src_size = (size_t)pitch * height;
    if (src_size == 0 || src_size > 100 * 1024 * 1024) { // Limit to 100MB
        cleanup_handle(compress_handle);
        cleanup_handle(decompress_handle);
        return 0;
    }
    
    // Allocate and initialize source buffer
    unsigned char* src_buf = (unsigned char*)malloc(src_size);
    if (!src_buf) {
        cleanup_handle(compress_handle);
        cleanup_handle(decompress_handle);
        return 0;
    }
    
    // Fill source buffer with fuzzer data (repeating if needed)
    for (size_t i = 0; i < src_size; i++) {
        src_buf[i] = Data[i % Size];
    }
    
    // Test tjEncodeYUV3
    {
        // Calculate destination buffer size
        int yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
        if (yuv_size > 0) {
            unsigned char* dst_buf = (unsigned char*)malloc(yuv_size);
            if (dst_buf) {
                tjEncodeYUV3(compress_handle, src_buf, width, pitch, height,
                            pixelFormat, dst_buf, align, subsamp, flags);
                free(dst_buf);
            }
        }
    }
    
    // Test tj3EncodeYUVPlanes8
    {
        unsigned char* dst_planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        // Calculate plane sizes and allocate
        for (int i = 0; i < 3; i++) {
            int plane_size = tj3YUVPlaneSize(i, width, 0, height, subsamp);
            if (plane_size > 0) {
                dst_planes[i] = (unsigned char*)malloc(plane_size);
                if (dst_planes[i]) {
                    strides[i] = 0; // Use default stride
                }
            }
        }
        
        tj3EncodeYUVPlanes8(compress_handle, src_buf, width, pitch, height,
                           pixelFormat, dst_planes, strides);
        
        cleanup_buffers(dst_planes, 3);
    }
    
    // Test tjDecodeYUV
    {
        // First create some YUV data to decode
        int yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
        if (yuv_size > 0) {
            unsigned char* yuv_buf = (unsigned char*)malloc(yuv_size);
            if (yuv_buf) {
                // Fill with fuzzer data
                for (int i = 0; i < yuv_size; i++) {
                    yuv_buf[i] = Data[i % Size];
                }
                
                // Allocate destination buffer
                int dst_size = pitch * height;
                unsigned char* dst_buf = (unsigned char*)malloc(dst_size);
                if (dst_buf) {
                    tjDecodeYUV(decompress_handle, yuv_buf, align, subsamp,
                               dst_buf, width, pitch, height, pixelFormat, flags);
                    free(dst_buf);
                }
                free(yuv_buf);
            }
        }
    }
    
    // Test tjEncodeYUVPlanes
    {
        unsigned char* dst_planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        // Calculate plane sizes and allocate
        for (int i = 0; i < 3; i++) {
            int plane_size = tjPlaneSizeYUV(i, width, 0, height, subsamp);
            if (plane_size > 0) {
                dst_planes[i] = (unsigned char*)malloc(plane_size);
                if (dst_planes[i]) {
                    strides[i] = 0; // Use default stride
                }
            }
        }
        
        tjEncodeYUVPlanes(compress_handle, src_buf, width, pitch, height,
                         pixelFormat, dst_planes, strides, subsamp, flags);
        
        cleanup_buffers(dst_planes, 3);
    }
    
    // Test tjEncodeYUV
    {
        int pixelSize = tjPixelSize[pixelFormat];
        if (pixelSize > 0) {
            int yuv_size = tjBufSizeYUV(width, height, subsamp);
            if (yuv_size > 0) {
                unsigned char* dst_buf = (unsigned char*)malloc(yuv_size);
                if (dst_buf) {
                    tjEncodeYUV(compress_handle, src_buf, width, pitch, height,
                               pixelSize, dst_buf, subsamp, flags);
                    free(dst_buf);
                }
            }
        }
    }
    
    // Test tjEncodeYUV2
    {
        int yuv_size = tjBufSizeYUV2(width, 4, height, subsamp);
        if (yuv_size > 0) {
            unsigned char* dst_buf = (unsigned char*)malloc(yuv_size);
            if (dst_buf) {
                tjEncodeYUV2(compress_handle, src_buf, width, pitch, height,
                            pixelFormat, dst_buf, subsamp, flags);
                free(dst_buf);
            }
        }
    }
    
    // Cleanup
    free(src_buf);
    cleanup_handle(compress_handle);
    cleanup_handle(decompress_handle);
    
    return 0;
}