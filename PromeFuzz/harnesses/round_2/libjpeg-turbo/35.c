// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

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

static int get_pixel_format(uint8_t index) {
    static const int formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XBGR, TJPF_XRGB, TJPF_GRAY, TJPF_RGBA,
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK
    };
    return formats[index % (sizeof(formats) / sizeof(formats[0]))];
}

static int get_subsamp(uint8_t index) {
    static const int subsamps[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420,
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    return subsamps[index % (sizeof(subsamps) / sizeof(subsamps[0]))];
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    const int width = 16;
    const int height = 16;
    const int pitch = width * 4; // Using 4 bytes per pixel for RGBX format
    
    // Extract parameters from fuzz data
    uint8_t func_selector = Data[0] % 6;
    uint8_t pixel_format_idx = Data[1];
    uint8_t subsamp_idx = Data[2];
    uint8_t align_value = Data[3] % 16;
    if (align_value == 0) align_value = 1;
    
    int pixelFormat = get_pixel_format(pixel_format_idx);
    int subsamp = get_subsamp(subsamp_idx);
    int pixelSize = 3 + (pixel_format_idx % 2); // Vary between 3 and 4 bytes
    
    // Prepare source buffer
    size_t src_size = pitch * height;
    unsigned char *src_buf = (unsigned char*)malloc(src_size);
    if (!src_buf) return 0;
    
    size_t copy_size = (Size - 4 > src_size) ? src_size : (Size - 4);
    memcpy(src_buf, Data + 4, copy_size);
    
    // Test different functions based on selector
    switch (func_selector) {
        case 0: { // tj3EncodeYUVPlanes8
            tjhandle handle = init_compressor();
            if (!handle) {
                free(src_buf);
                return 0;
            }
            
            tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
            
            // Calculate plane sizes
            int strides[3] = {0, 0, 0};
            unsigned char *planes[3] = {NULL, NULL, NULL};
            
            for (int i = 0; i < 3; i++) {
                int plane_width = (i == 0) ? width : (width / (subsamp == TJSAMP_420 || subsamp == TJSAMP_422 ? 2 : 1));
                int plane_height = (i == 0) ? height : (height / (subsamp == TJSAMP_420 ? 2 : 1));
                size_t plane_size = plane_width * plane_height;
                planes[i] = (unsigned char*)malloc(plane_size);
                if (!planes[i]) {
                    for (int j = 0; j < i; j++) free(planes[j]);
                    cleanup_handle(handle);
                    free(src_buf);
                    return 0;
                }
            }
            
            tj3EncodeYUVPlanes8(handle, src_buf, width, pitch, height,
                               pixelFormat, planes, strides);
            
            for (int i = 0; i < 3; i++) free(planes[i]);
            cleanup_handle(handle);
            break;
        }
        
        case 1: { // tjEncodeYUV3
            tjhandle handle = init_compressor();
            if (!handle) {
                free(src_buf);
                return 0;
            }
            
            // Calculate destination buffer size
            int dst_size = tjBufSizeYUV2(width, align_value, height, subsamp);
            unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
            if (!dst_buf) {
                cleanup_handle(handle);
                free(src_buf);
                return 0;
            }
            
            tjEncodeYUV3(handle, src_buf, width, pitch, height,
                        pixelFormat, dst_buf, align_value, subsamp, 0);
            
            free(dst_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 2: { // tjEncodeYUV2
            tjhandle handle = init_compressor();
            if (!handle) {
                free(src_buf);
                return 0;
            }
            
            int dst_size = tjBufSizeYUV2(width, 4, height, subsamp);
            unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
            if (!dst_buf) {
                cleanup_handle(handle);
                free(src_buf);
                return 0;
            }
            
            tjEncodeYUV2(handle, src_buf, width, pitch, height,
                        pixelFormat, dst_buf, subsamp, 0);
            
            free(dst_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 3: { // tjDecodeYUV
            tjhandle handle = init_decompressor();
            if (!handle) {
                free(src_buf);
                return 0;
            }
            
            // Prepare destination buffer
            unsigned char *dst_buf = (unsigned char*)malloc(src_size);
            if (!dst_buf) {
                cleanup_handle(handle);
                free(src_buf);
                return 0;
            }
            
            tjDecodeYUV(handle, src_buf, align_value, subsamp,
                       dst_buf, width, pitch, height, pixelFormat, 0);
            
            free(dst_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 4: { // tjEncodeYUV
            tjhandle handle = init_compressor();
            if (!handle) {
                free(src_buf);
                return 0;
            }
            
            int dst_size = tjBufSizeYUV(width, height, subsamp);
            unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
            if (!dst_buf) {
                cleanup_handle(handle);
                free(src_buf);
                return 0;
            }
            
            tjEncodeYUV(handle, src_buf, width, pitch, height,
                       pixelSize, dst_buf, subsamp, 0);
            
            free(dst_buf);
            cleanup_handle(handle);
            break;
        }
        
        case 5: { // tjEncodeYUVPlanes
            tjhandle handle = init_compressor();
            if (!handle) {
                free(src_buf);
                return 0;
            }
            
            int strides[3] = {0, 0, 0};
            unsigned char *planes[3] = {NULL, NULL, NULL};
            
            for (int i = 0; i < 3; i++) {
                int plane_size = tjPlaneSizeYUV(i, width, strides[i], height, subsamp);
                if (plane_size > 0) {
                    planes[i] = (unsigned char*)malloc(plane_size);
                    if (!planes[i]) {
                        for (int j = 0; j < i; j++) free(planes[j]);
                        cleanup_handle(handle);
                        free(src_buf);
                        return 0;
                    }
                }
            }
            
            tjEncodeYUVPlanes(handle, src_buf, width, pitch, height,
                             pixelFormat, planes, strides, subsamp, 0);
            
            for (int i = 0; i < 3; i++) free(planes[i]);
            cleanup_handle(handle);
            break;
        }
    }
    
    free(src_buf);
    return 0;
}