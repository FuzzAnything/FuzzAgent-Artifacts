// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"

#define MIN_DIMENSION 1
#define MAX_DIMENSION 256
#define MAX_PLANES 3

static unsigned char* create_dummy_buffer(size_t size) {
    unsigned char* buf = (unsigned char*)malloc(size);
    if (buf && size > 0) {
        for (size_t i = 0; i < size; i++) {
            buf[i] = i & 0xFF;
        }
    }
    return buf;
}

static void free_planes(unsigned char** planes, int count) {
    if (planes) {
        for (int i = 0; i < count; i++) {
            free(planes[i]);
        }
    }
}

static int get_valid_pixel_format(uint8_t value) {
    const int formats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, 
                          TJPF_XRGB, TJPF_XBGR, TJPF_GRAY, TJPF_RGBA, 
                          TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK};
    return formats[value % 12];
}

static int get_valid_subsamp(uint8_t value) {
    const int subsamps[] = {TJSAMP_444, TJSAMP_422, TJSAMP_420, 
                           TJSAMP_GRAY, TJSAMP_440, TJSAMP_411};
    return subsamps[value % 6];
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Initialize handles
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tjDestroy(compress_handle);
        if (decompress_handle) tjDestroy(decompress_handle);
        return 0;
    }
    
    // Parse parameters from fuzz input
    size_t offset = 0;
    uint8_t func_selector = Data[offset++] % 6;
    uint8_t width_val = Data[offset++];
    uint8_t height_val = Data[offset++];
    uint8_t pitch_val = Data[offset++];
    uint8_t pixel_fmt_val = Data[offset++];
    uint8_t subsamp_val = Data[offset++];
    uint8_t align_val = Data[offset++];
    uint8_t flags_val = Data[offset++];
    
    // Ensure we have enough data for image buffers
    if (Size - offset < 64) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // Set dimensions with reasonable bounds
    int width = MIN_DIMENSION + (width_val % (MAX_DIMENSION - MIN_DIMENSION + 1));
    int height = MIN_DIMENSION + (height_val % (MAX_DIMENSION - MIN_DIMENSION + 1));
    int pitch = pitch_val ? (pitch_val % (width * 4 + 64)) : 0;
    int pixelFormat = get_valid_pixel_format(pixel_fmt_val);
    int subsamp = get_valid_subsamp(subsamp_val);
    int align = 1 << (align_val % 5); // 1, 2, 4, 8, 16
    int flags = flags_val & 0x0F;
    
    // Calculate pixel size for older functions
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Create source buffer
    size_t src_size = (pitch ? pitch : width * pixelSize) * height;
    if (src_size == 0) src_size = width * height * 4;
    unsigned char* src_buf = create_dummy_buffer(src_size);
    if (!src_buf) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // Copy fuzz data into source buffer
    size_t copy_size = src_size < (Size - offset) ? src_size : (Size - offset);
    memcpy(src_buf, Data + offset, copy_size);
    
    switch (func_selector) {
        case 0: { // tj3EncodeYUVPlanes8
            // Set up destination planes
            unsigned char* dst_planes[MAX_PLANES] = {NULL};
            int strides[MAX_PLANES] = {0};
            
            // Calculate plane sizes and allocate
            for (int i = 0; i < MAX_PLANES; i++) {
                // Get stride for this plane from fuzz data
                if (Size - offset > i) {
                    strides[i] = Data[offset + i] % 32;
                }
                
                // Calculate plane size with the stride
                size_t plane_size = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
                if (plane_size > 0 && plane_size < 1024 * 1024) { // Limit size
                    dst_planes[i] = (unsigned char*)malloc(plane_size);
                    if (dst_planes[i]) {
                        memset(dst_planes[i], 0, plane_size);
                    }
                }
            }
            
            // Call function
            tj3EncodeYUVPlanes8(compress_handle, src_buf, width, pitch, height,
                               pixelFormat, dst_planes, strides);
            
            free_planes(dst_planes, MAX_PLANES);
            break;
        }
        
        case 1: { // tjEncodeYUV
            // Calculate destination buffer size
            int yuv_size = tjBufSizeYUV(width, height, subsamp);
            if (yuv_size > 0 && yuv_size < 1024 * 1024) {
                unsigned char* dst_buf = (unsigned char*)malloc(yuv_size);
                if (dst_buf) {
                    tjEncodeYUV(compress_handle, src_buf, width, pitch, height,
                               pixelSize, dst_buf, subsamp, flags);
                    free(dst_buf);
                }
            }
            break;
        }
        
        case 2: { // tjEncodeYUV3
            // Calculate destination buffer size
            int yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuv_size > 0 && yuv_size < 1024 * 1024) {
                unsigned char* dst_buf = (unsigned char*)malloc(yuv_size);
                if (dst_buf) {
                    tjEncodeYUV3(compress_handle, src_buf, width, pitch, height,
                                pixelFormat, dst_buf, align, subsamp, flags);
                    free(dst_buf);
                }
            }
            break;
        }
        
        case 3: { // tjDecodeYUV
            // Create YUV source buffer
            int yuv_src_size = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuv_src_size > 0 && yuv_src_size < 1024 * 1024) {
                unsigned char* yuv_src = create_dummy_buffer(yuv_src_size);
                if (yuv_src) {
                    // Create destination buffer
                    int dst_size = (pitch ? pitch : width * tjPixelSize[pixelFormat]) * height;
                    if (dst_size > 0 && dst_size < 1024 * 1024) {
                        unsigned char* dst_buf = (unsigned char*)malloc(dst_size);
                        if (dst_buf) {
                            tjDecodeYUV(decompress_handle, yuv_src, align, subsamp,
                                       dst_buf, width, pitch, height, pixelFormat, flags);
                            free(dst_buf);
                        }
                    }
                    free(yuv_src);
                }
            }
            break;
        }
        
        case 4: { // tjEncodeYUVPlanes
            // Set up destination planes
            unsigned char* dst_planes[MAX_PLANES] = {NULL};
            int strides[MAX_PLANES] = {0};
            
            // Calculate plane sizes and allocate
            for (int i = 0; i < MAX_PLANES; i++) {
                // Get stride for this plane from fuzz data
                if (Size - offset > i) {
                    strides[i] = Data[offset + i] % 32;
                }
                
                // Calculate plane size with the stride
                unsigned long plane_size = tjPlaneSizeYUV(i, width, strides[i], height, subsamp);
                if (plane_size > 0 && plane_size < 1024 * 1024) {
                    dst_planes[i] = (unsigned char*)malloc(plane_size);
                    if (dst_planes[i]) {
                        memset(dst_planes[i], 0, plane_size);
                    }
                }
            }
            
            // Call function
            tjEncodeYUVPlanes(compress_handle, src_buf, width, pitch, height,
                             pixelFormat, dst_planes, strides, subsamp, flags);
            
            free_planes(dst_planes, MAX_PLANES);
            break;
        }
        
        case 5: { // tjEncodeYUV2
            // Calculate destination buffer size
            int yuv_size = tjBufSizeYUV2(width, 4, height, subsamp);
            if (yuv_size > 0 && yuv_size < 1024 * 1024) {
                unsigned char* dst_buf = (unsigned char*)malloc(yuv_size);
                if (dst_buf) {
                    tjEncodeYUV2(compress_handle, src_buf, width, pitch, height,
                                pixelFormat, dst_buf, subsamp, flags);
                    free(dst_buf);
                }
            }
            break;
        }
    }
    
    // Cleanup
    free(src_buf);
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    
    return 0;
}