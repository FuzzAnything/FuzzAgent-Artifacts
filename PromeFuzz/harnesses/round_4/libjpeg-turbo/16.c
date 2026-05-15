// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle create_decompress_handle(void) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return NULL;
    
    // Set some reasonable default parameters
    tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_420);
    tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);
    tj3Set(handle, TJPARAM_FASTDCT, 1);
    
    return handle;
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static int get_valid_pixel_format(uint8_t value) {
    const int formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XRGB, TJPF_XBGR, TJPF_GRAY, TJPF_RGBA,
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB
    };
    return formats[value % (sizeof(formats)/sizeof(formats[0]))];
}

static int get_valid_subsamp(uint8_t value) {
    const int subsamps[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420,
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    return subsamps[value % (sizeof(subsamps)/sizeof(subsamps[0]))];
}

static int get_valid_align(uint8_t value) {
    // Alignment must be power of 2, 1-256
    int align = 1 << (value % 9); // 2^0 to 2^8
    if (align > 256) align = 1;
    return align;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Extract parameters from fuzz data
    uint8_t func_selector = Data[0] % 6;
    uint8_t width_val = Data[1];
    uint8_t height_val = Data[2];
    uint8_t param1 = Data[3];
    uint8_t param2 = Data[4];
    
    // Ensure minimum dimensions
    int width = (width_val % 64) + 1;
    int height = (height_val % 64) + 1;
    int pitch = width * 4; // Assume RGBA for simplicity
    int align = get_valid_align(param1);
    int pixelFormat = get_valid_pixel_format(param2);
    int subsamp = get_valid_subsamp(param1);
    
    // Skip parameter bytes
    const uint8_t *buffer_data = Data + 5;
    size_t buffer_size = Size - 5;
    
    // Create decompression handle
    tjhandle handle = create_decompress_handle();
    if (!handle) return 0;
    
    // Allocate buffers
    unsigned char *src_buf = NULL;
    unsigned char *dst_buf = NULL;
    unsigned char *yuv_planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    // Calculate buffer sizes
    size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);
    size_t rgb_buf_size = width * height * tjPixelSize[pixelFormat];
    
    if (yuv_buf_size > 0 && yuv_buf_size <= buffer_size) {
        src_buf = (unsigned char*)malloc(yuv_buf_size);
        if (src_buf) {
            memcpy(src_buf, buffer_data, yuv_buf_size < buffer_size ? yuv_buf_size : buffer_size);
        }
    }
    
    if (rgb_buf_size > 0) {
        dst_buf = (unsigned char*)malloc(rgb_buf_size);
    }
    
    // Allocate YUV planes
    for (int i = 0; i < 3; i++) {
        size_t plane_size = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
        if (plane_size > 0 && plane_size <= buffer_size / 3) {
            yuv_planes[i] = (unsigned char*)malloc(plane_size);
            if (yuv_planes[i]) {
                size_t offset = i * (buffer_size / 3);
                if (offset + plane_size <= buffer_size) {
                    memcpy(yuv_planes[i], buffer_data + offset, plane_size);
                }
            }
        }
    }
    
    // Execute selected function
    switch (func_selector) {
        case 0: // tj3DecodeYUV8
            if (src_buf && dst_buf) {
                tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
                tj3DecodeYUV8(handle, src_buf, align, dst_buf, width, pitch, height, pixelFormat);
            }
            break;
            
        case 1: // tjDecodeYUV
            if (src_buf && dst_buf) {
                tjDecodeYUV(handle, src_buf, align, subsamp, dst_buf, width, pitch, height, pixelFormat, 0);
            }
            break;
            
        case 2: // tj3DecompressToYUVPlanes8
            if (buffer_size > 0 && yuv_planes[0]) {
                // Use buffer_data as JPEG data
                tj3DecompressToYUVPlanes8(handle, buffer_data, buffer_size, yuv_planes, strides);
            }
            break;
            
        case 3: // tjDestroy
            // Test destroying and recreating handle
            tjDestroy(handle);
            handle = create_decompress_handle();
            break;
            
        case 4: // tj3DecodeYUVPlanes8
            if (yuv_planes[0] && dst_buf) {
                tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
                tj3DecodeYUVPlanes8(handle, (const unsigned char* const*)yuv_planes, strides, dst_buf, width, pitch, height, pixelFormat);
            }
            break;
            
        case 5: // tjDecompressToYUVPlanes
            if (buffer_size > 0 && yuv_planes[0]) {
                tjDecompressToYUVPlanes(handle, buffer_data, (unsigned long)buffer_size, yuv_planes, width, strides, height, 0);
            }
            break;
    }
    
    // Cleanup
    cleanup_handle(handle);
    
    if (src_buf) free(src_buf);
    if (dst_buf) free(dst_buf);
    for (int i = 0; i < 3; i++) {
        if (yuv_planes[i]) free(yuv_planes[i]);
    }
    
    return 0;
}