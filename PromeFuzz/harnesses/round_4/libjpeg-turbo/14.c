// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIMENSION 1024
#define MAX_PLANES 3

static tjhandle init_compressor() {
    return tjInitCompress();
}

static tjhandle init_decompressor() {
    return tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static int get_safe_dimension(uint32_t value) {
    return (value % MAX_DIMENSION) + 1;
}

static int get_pixel_format(uint8_t value) {
    const int formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XBGR, TJPF_XRGB, TJPF_GRAY, TJPF_RGBA,
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB
    };
    return formats[value % (sizeof(formats) / sizeof(formats[0]))];
}

static int get_subsamp(uint8_t value) {
    const int subsamps[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420,
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    return subsamps[value % (sizeof(subsamps) / sizeof(subsamps[0]))];
}

static int get_align(uint8_t value) {
    const int aligns[] = {1, 2, 4, 8};
    return aligns[value % (sizeof(aligns) / sizeof(aligns[0]))];
}

int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size) {
    if (size < 16) return 0;

    uint32_t width = get_safe_dimension(*(uint32_t*)data);
    uint32_t height = get_safe_dimension(*(uint32_t*)(data + 4));
    uint8_t pixel_format_selector = data[8];
    uint8_t subsamp_selector = data[9];
    uint8_t align_selector = data[10];
    uint8_t function_selector = data[11] % 6;

    int pixelFormat = get_pixel_format(pixel_format_selector);
    int subsamp = get_subsamp(subsamp_selector);
    int align = get_align(align_selector);
    int pixelSize = tjPixelSize[pixelFormat];
    
    size_t src_pitch = width * pixelSize;
    size_t src_size = src_pitch * height;
    
    if (src_size == 0) {
        return 0;
    }

    const unsigned char *src_data = data + 12;
    size_t src_data_available = size - 12;
    
    switch (function_selector) {
        case 0: { // tj3EncodeYUVPlanes8
            tjhandle handle = init_compressor();
            if (!handle) return 0;
            
            tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
            
            unsigned char *src_buf = malloc(src_size);
            if (!src_buf) {
                cleanup_handle(handle);
                return 0;
            }
            
            size_t copy_size = src_data_available < src_size ? src_data_available : src_size;
            memcpy(src_buf, src_data, copy_size);
            if (copy_size < src_size) {
                memset(src_buf + copy_size, 0, src_size - copy_size);
            }
            
            size_t yuv_sizes[MAX_PLANES];
            unsigned char *yuv_planes[MAX_PLANES] = {NULL};
            int alloc_failed = 0;
            
            for (int i = 0; i < MAX_PLANES; i++) {
                yuv_sizes[i] = tj3YUVPlaneSize(i, width, 0, height, subsamp);
                if (yuv_sizes[i] > 0) {
                    yuv_planes[i] = malloc(yuv_sizes[i]);
                    if (!yuv_planes[i]) {
                        alloc_failed = 1;
                        break;
                    }
                    memset(yuv_planes[i], 0, yuv_sizes[i]);
                }
            }
            
            if (!alloc_failed) {
                int strides[3] = {0, 0, 0};
                tj3EncodeYUVPlanes8(handle, src_buf, width, src_pitch, height,
                                   pixelFormat, yuv_planes, strides);
            }
            
            for (int i = 0; i < MAX_PLANES; i++) {
                free(yuv_planes[i]);
            }
            free(src_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 1: { // tjDecodeYUV
            tjhandle handle = init_decompressor();
            if (!handle) return 0;
            
            size_t yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuv_size == 0 || yuv_size > src_data_available) {
                cleanup_handle(handle);
                return 0;
            }
            
            unsigned char *yuv_buf = malloc(yuv_size);
            if (!yuv_buf) {
                cleanup_handle(handle);
                return 0;
            }
            
            memcpy(yuv_buf, src_data, yuv_size);
            
            unsigned char *dst_buf = malloc(src_size);
            if (!dst_buf) {
                free(yuv_buf);
                cleanup_handle(handle);
                return 0;
            }
            
            tjDecodeYUV(handle, yuv_buf, align, subsamp, dst_buf,
                       width, src_pitch, height, pixelFormat, 0);
            
            free(dst_buf);
            free(yuv_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 2: { // tjEncodeYUV
            tjhandle handle = init_compressor();
            if (!handle) return 0;
            
            unsigned char *src_buf = malloc(src_size);
            if (!src_buf) {
                cleanup_handle(handle);
                return 0;
            }
            
            size_t copy_size = src_data_available < src_size ? src_data_available : src_size;
            memcpy(src_buf, src_data, copy_size);
            if (copy_size < src_size) {
                memset(src_buf + copy_size, 0, src_size - copy_size);
            }
            
            size_t yuv_size = tjBufSizeYUV2(width, 4, height, subsamp);
            if (yuv_size == 0) {
                free(src_buf);
                cleanup_handle(handle);
                return 0;
            }
            
            unsigned char *dst_buf = malloc(yuv_size);
            if (!dst_buf) {
                free(src_buf);
                cleanup_handle(handle);
                return 0;
            }
            
            tjEncodeYUV(handle, src_buf, width, src_pitch, height,
                       pixelSize, dst_buf, subsamp, 0);
            
            free(dst_buf);
            free(src_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 3: { // tjEncodeYUVPlanes
            tjhandle handle = init_compressor();
            if (!handle) return 0;
            
            unsigned char *src_buf = malloc(src_size);
            if (!src_buf) {
                cleanup_handle(handle);
                return 0;
            }
            
            size_t copy_size = src_data_available < src_size ? src_data_available : src_size;
            memcpy(src_buf, src_data, copy_size);
            if (copy_size < src_size) {
                memset(src_buf + copy_size, 0, src_size - copy_size);
            }
            
            unsigned char *yuv_planes[MAX_PLANES] = {NULL};
            int strides[3] = {0, 0, 0};
            int alloc_failed = 0;
            
            for (int i = 0; i < MAX_PLANES; i++) {
                size_t plane_size = tjPlaneSizeYUV(i, width, 0, height, subsamp);
                if (plane_size > 0) {
                    yuv_planes[i] = malloc(plane_size);
                    if (!yuv_planes[i]) {
                        alloc_failed = 1;
                        break;
                    }
                    memset(yuv_planes[i], 0, plane_size);
                }
            }
            
            if (!alloc_failed) {
                tjEncodeYUVPlanes(handle, src_buf, width, src_pitch, height,
                                 pixelFormat, yuv_planes, strides, subsamp, 0);
            }
            
            for (int i = 0; i < MAX_PLANES; i++) {
                free(yuv_planes[i]);
            }
            free(src_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 4: { // tjEncodeYUV3
            tjhandle handle = init_compressor();
            if (!handle) return 0;
            
            unsigned char *src_buf = malloc(src_size);
            if (!src_buf) {
                cleanup_handle(handle);
                return 0;
            }
            
            size_t copy_size = src_data_available < src_size ? src_data_available : src_size;
            memcpy(src_buf, src_data, copy_size);
            if (copy_size < src_size) {
                memset(src_buf + copy_size, 0, src_size - copy_size);
            }
            
            size_t yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuv_size == 0) {
                free(src_buf);
                cleanup_handle(handle);
                return 0;
            }
            
            unsigned char *dst_buf = malloc(yuv_size);
            if (!dst_buf) {
                free(src_buf);
                cleanup_handle(handle);
                return 0;
            }
            
            tjEncodeYUV3(handle, src_buf, width, src_pitch, height,
                        pixelFormat, dst_buf, align, subsamp, 0);
            
            free(dst_buf);
            free(src_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 5: { // tjEncodeYUV2
            tjhandle handle = init_compressor();
            if (!handle) return 0;
            
            unsigned char *src_buf = malloc(src_size);
            if (!src_buf) {
                cleanup_handle(handle);
                return 0;
            }
            
            size_t copy_size = src_data_available < src_size ? src_data_available : src_size;
            memcpy(src_buf, src_data, copy_size);
            if (copy_size < src_size) {
                memset(src_buf + copy_size, 0, src_size - copy_size);
            }
            
            size_t yuv_size = tjBufSizeYUV2(width, 4, height, subsamp);
            if (yuv_size == 0) {
                free(src_buf);
                cleanup_handle(handle);
                return 0;
            }
            
            unsigned char *dst_buf = malloc(yuv_size);
            if (!dst_buf) {
                free(src_buf);
                cleanup_handle(handle);
                return 0;
            }
            
            tjEncodeYUV2(handle, src_buf, width, src_pitch, height,
                        pixelFormat, dst_buf, subsamp, 0);
            
            free(dst_buf);
            free(src_buf);
            cleanup_handle(handle);
            break;
        }
    }
    
    return 0;
}