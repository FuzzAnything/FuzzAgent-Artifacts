// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "turbojpeg.h"

static const int FUZZ_WIDTH = 32;
static const int FUZZ_HEIGHT = 32;
static const int MAX_PLANES = 3;

static tjhandle init_compressor() {
    tjhandle handle = tjInitCompress();
    if (handle) {
        tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_444);
    }
    return handle;
}

static tjhandle init_decompressor() {
    tjhandle handle = tjInitDecompress();
    if (handle) {
        tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_444);
    }
    return handle;
}

static int get_random_value(const uint8_t *data, size_t size, size_t *offset, int min, int max) {
    if (*offset >= size) return min;
    int val = data[*offset] % (max - min + 1) + min;
    (*offset)++;
    return val;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    size_t offset = 0;
    
    // Initialize compressor and decompressor
    tjhandle comp_handle = init_compressor();
    tjhandle decomp_handle = init_decompressor();
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tjDestroy(comp_handle);
        if (decomp_handle) tjDestroy(decomp_handle);
        return 0;
    }
    
    // Get random parameters from fuzzer input
    int width = get_random_value(Data, Size, &offset, 1, FUZZ_WIDTH);
    int height = get_random_value(Data, Size, &offset, 1, FUZZ_HEIGHT);
    int align = 1 << get_random_value(Data, Size, &offset, 0, 4); // 1, 2, 4, 8, 16
    int subsamp = get_random_value(Data, Size, &offset, TJSAMP_444, TJSAMP_GRAY);
    int pixelFormat = get_random_value(Data, Size, &offset, TJPF_RGB, TJPF_CMYK);
    int flags = get_random_value(Data, Size, &offset, 0, TJFLAG_FASTDCT);
    
    // Calculate buffer sizes with proper bounds checking
    int pixel_size = tjPixelSize[pixelFormat];
    if (pixel_size <= 0) {
        tjDestroy(comp_handle);
        tjDestroy(decomp_handle);
        return 0;
    }
    
    int rgb_size = width * height * pixel_size;
    if (rgb_size <= 0 || rgb_size > 1024 * 1024 * 10) { // Limit to 10MB
        tjDestroy(comp_handle);
        tjDestroy(decomp_handle);
        return 0;
    }
    
    // Calculate YUV buffer sizes using appropriate functions
    int yuv_size = tjBufSizeYUV(width, height, subsamp);
    int yuv_size_new = tj3YUVBufSize(width, align, height, subsamp);
    
    // Use the larger of the two sizes to ensure we have enough buffer
    int yuv_buf_size = yuv_size > yuv_size_new ? yuv_size : yuv_size_new;
    
    if (yuv_buf_size <= 0 || yuv_buf_size > 1024 * 1024 * 10) { // Limit to 10MB
        tjDestroy(comp_handle);
        tjDestroy(decomp_handle);
        return 0;
    }
    
    // Allocate buffers
    unsigned char *rgb_buf = (unsigned char*)malloc(rgb_size);
    unsigned char *yuv_buf = (unsigned char*)malloc(yuv_buf_size);
    unsigned char *yuv_planes[MAX_PLANES];
    int strides[MAX_PLANES];
    
    if (!rgb_buf || !yuv_buf) {
        free(rgb_buf);
        free(yuv_buf);
        tjDestroy(comp_handle);
        tjDestroy(decomp_handle);
        return 0;
    }
    
    // Initialize YUV planes
    for (int i = 0; i < MAX_PLANES; i++) {
        int plane_size = tjPlaneSizeYUV(i, width, 0, height, subsamp);
        if (plane_size <= 0) {
            plane_size = 1024; // Default size if calculation fails
        }
        yuv_planes[i] = (unsigned char*)malloc(plane_size);
        strides[i] = width;
    }
    
    // Fill buffers with fuzzer data
    size_t data_to_copy = Size - offset;
    if (data_to_copy > 0) {
        // Copy to RGB buffer
        size_t rgb_copy = data_to_copy > (size_t)rgb_size ? rgb_size : data_to_copy;
        memcpy(rgb_buf, Data + offset, rgb_copy);
        
        // Copy to YUV buffer
        if (data_to_copy > rgb_copy) {
            size_t yuv_copy = data_to_copy - rgb_copy;
            yuv_copy = yuv_copy > (size_t)yuv_buf_size ? yuv_buf_size : yuv_copy;
            memcpy(yuv_buf, Data + offset + rgb_copy, yuv_copy);
        }
    }
    
    // Test tj3DecodeYUV8 with proper buffer size
    if (decomp_handle) {
        unsigned char *dst_buf = (unsigned char*)malloc(rgb_size);
        if (dst_buf) {
            // Ensure width and height are valid
            if (width > 0 && height > 0) {
                tj3DecodeYUV8(decomp_handle, yuv_buf, align, dst_buf, width, 0, height, pixelFormat);
            }
            free(dst_buf);
        }
    }
    
    // Test tj3EncodeYUVPlanes8
    if (comp_handle) {
        // Ensure width and height are valid
        if (width > 0 && height > 0) {
            tj3EncodeYUVPlanes8(comp_handle, rgb_buf, width, 0, height, pixelFormat, yuv_planes, strides);
        }
    }
    
    // Test tjDecodeYUVPlanes
    if (decomp_handle) {
        unsigned char *dst_buf = (unsigned char*)malloc(rgb_size);
        if (dst_buf) {
            const unsigned char *src_planes[MAX_PLANES];
            for (int i = 0; i < MAX_PLANES; i++) {
                src_planes[i] = yuv_planes[i];
            }
            // Ensure width and height are valid
            if (width > 0 && height > 0) {
                tjDecodeYUVPlanes(decomp_handle, src_planes, strides, subsamp, dst_buf, width, 0, height, pixelFormat, flags);
            }
            free(dst_buf);
        }
    }
    
    // Test tjEncodeYUV2
    if (comp_handle) {
        unsigned char *dst_buf = (unsigned char*)malloc(yuv_size);
        if (dst_buf) {
            // Ensure width and height are valid
            if (width > 0 && height > 0) {
                tjEncodeYUV2(comp_handle, rgb_buf, width, 0, height, pixelFormat, dst_buf, subsamp, flags);
            }
            free(dst_buf);
        }
    }
    
    // Test tjDecodeYUV
    if (decomp_handle) {
        unsigned char *dst_buf = (unsigned char*)malloc(rgb_size);
        if (dst_buf) {
            // Ensure width and height are valid
            if (width > 0 && height > 0) {
                tjDecodeYUV(decomp_handle, yuv_buf, align, subsamp, dst_buf, width, 0, height, pixelFormat, flags);
            }
            free(dst_buf);
        }
    }
    
    // Test tjEncodeYUVPlanes
    if (comp_handle) {
        // Ensure width and height are valid
        if (width > 0 && height > 0) {
            tjEncodeYUVPlanes(comp_handle, rgb_buf, width, 0, height, pixelFormat, yuv_planes, strides, subsamp, flags);
        }
    }
    
    // Cleanup
    free(rgb_buf);
    free(yuv_buf);
    for (int i = 0; i < MAX_PLANES; i++) {
        free(yuv_planes[i]);
    }
    
    if (comp_handle) tjDestroy(comp_handle);
    if (decomp_handle) tjDestroy(decomp_handle);
    
    return 0;
}