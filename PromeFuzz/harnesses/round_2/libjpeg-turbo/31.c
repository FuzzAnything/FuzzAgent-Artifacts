// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle create_handle(int is_compressor) {
    if (is_compressor) {
        return tjInitCompress();
    } else {
        return tjInitDecompress();
    }
}

static void destroy_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static int get_valid_pixel_format(uint8_t value) {
    const int formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XRGB, TJPF_XBGR, TJPF_GRAY, TJPF_RGBA,
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK
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

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // Extract parameters from fuzz data
    uint8_t func_selector = Data[0] % 6;
    int width = (Data[1] << 8) | Data[2];
    int height = (Data[3] << 8) | Data[4];
    int pitch = (Data[5] << 8) | Data[6];
    int pixel_format = get_valid_pixel_format(Data[7]);
    int subsamp = get_valid_subsamp(Data[8]);
    int align = 1 << (Data[9] % 4); // 1, 2, 4, or 8
    int flags = Data[10];
    int pixel_size = Data[11] % 5 + 1; // 1-5 bytes per pixel

    // Ensure reasonable dimensions to avoid excessive memory usage
    if (width <= 0 || width > 1024) width = 64;
    if (height <= 0 || height > 1024) height = 64;
    
    // Calculate pixel size for the format
    int pixel_size_bytes = tjPixelSize[pixel_format];
    if (pixel_size_bytes <= 0) pixel_size_bytes = 3;
    
    if (pitch <= 0 || pitch > width * pixel_size_bytes * 2) {
        pitch = width * pixel_size_bytes;
    }
    
    // Ensure pitch is at least width * pixel_size_bytes to avoid buffer overflow
    if (pitch < width * pixel_size_bytes) {
        pitch = width * pixel_size_bytes;
    }

    // Calculate source buffer size
    size_t src_buf_size = (size_t)pitch * (size_t)height;
    if (src_buf_size == 0 || src_buf_size > 10 * 1024 * 1024) {
        src_buf_size = 64 * 64 * 4;
    }

    // Allocate and fill source buffer
    unsigned char *src_buf = (unsigned char *)malloc(src_buf_size);
    if (!src_buf) return 0;
    
    size_t copy_size = Size - 12;
    if (copy_size > src_buf_size) copy_size = src_buf_size;
    memcpy(src_buf, Data + 12, copy_size);
    if (copy_size < src_buf_size) {
        memset(src_buf + copy_size, 0, src_buf_size - copy_size);
    }

    // Calculate destination buffer sizes
    size_t dst_buf_size = tjBufSizeYUV2(width, align, height, subsamp);
    if (dst_buf_size == 0 || dst_buf_size > 10 * 1024 * 1024) {
        dst_buf_size = 1024 * 1024;
    }
    
    // For decode functions, we need a pixel buffer, not YUV buffer
    size_t pixel_dst_buf_size = (size_t)pitch * (size_t)height;
    if (pixel_dst_buf_size == 0 || pixel_dst_buf_size > 10 * 1024 * 1024) {
        pixel_dst_buf_size = 1024 * 1024;
    }
    
    unsigned char *dst_buf = (unsigned char *)malloc(dst_buf_size);
    unsigned char *pixel_dst_buf = (unsigned char *)malloc(pixel_dst_buf_size);
    if (!dst_buf || !pixel_dst_buf) {
        free(src_buf);
        free(dst_buf);
        free(pixel_dst_buf);
        return 0;
    }
    memset(dst_buf, 0, dst_buf_size);
    memset(pixel_dst_buf, 0, pixel_dst_buf_size);

    // Create handles
    tjhandle comp_handle = create_handle(1);
    tjhandle decomp_handle = create_handle(0);

    switch (func_selector) {
        case 0: { // tjEncodeYUV3
            if (comp_handle) {
                tjEncodeYUV3(comp_handle, src_buf, width, pitch, height,
                            pixel_format, dst_buf, align, subsamp, flags);
            }
            break;
        }
        
        case 1: { // tj3EncodeYUVPlanes8
            if (comp_handle) {
                unsigned char *planes[3] = {NULL, NULL, NULL};
                int strides[3] = {0, 0, 0};
                
                // Calculate plane sizes
                for (int i = 0; i < 3; i++) {
                    int plane_width = width;
                    int plane_height = height;
                    
                    // Adjust dimensions for chroma planes based on subsampling
                    if (i > 0 && subsamp != TJSAMP_GRAY && subsamp != TJSAMP_444) {
                        if (subsamp == TJSAMP_422 || subsamp == TJSAMP_420 || 
                            subsamp == TJSAMP_440 || subsamp == TJSAMP_411) {
                            plane_width = (width + 1) / 2;
                        }
                        if (subsamp == TJSAMP_420 || subsamp == TJSAMP_440) {
                            plane_height = (height + 1) / 2;
                        }
                    }
                    
                    size_t plane_size = (size_t)plane_width * (size_t)plane_height;
                    if (plane_size > 0 && plane_size < 10 * 1024 * 1024) {
                        planes[i] = (unsigned char *)malloc(plane_size);
                        if (planes[i]) {
                            memset(planes[i], 0, plane_size);
                        }
                    }
                }
                
                if (planes[0] || planes[1] || planes[2]) {
                    tj3EncodeYUVPlanes8(comp_handle, src_buf, width, pitch, height,
                                       pixel_format, planes, strides);
                }
                
                // Clean up planes
                for (int i = 0; i < 3; i++) {
                    free(planes[i]);
                }
            }
            break;
        }
        
        case 2: { // tjEncodeYUV2
            if (comp_handle) {
                tjEncodeYUV2(comp_handle, src_buf, width, pitch, height,
                            pixel_format, dst_buf, subsamp, flags);
            }
            break;
        }
        
        case 3: { // tjDecodeYUV
            if (decomp_handle) {
                // Use a properly sized YUV buffer for source
                size_t yuv_src_size = tjBufSizeYUV2(width, align, height, subsamp);
                if (yuv_src_size > 0 && yuv_src_size <= dst_buf_size) {
                    // Ensure pixel_dst_buf is large enough for the decoded image
                    size_t required_dst_size = (size_t)pitch * (size_t)height;
                    if (required_dst_size <= pixel_dst_buf_size) {
                        tjDecodeYUV(decomp_handle, dst_buf, align, subsamp, pixel_dst_buf,
                                   width, pitch, height, pixel_format, flags);
                    }
                }
            }
            break;
        }
        
        case 4: { // tjEncodeYUV
            if (comp_handle) {
                tjEncodeYUV(comp_handle, src_buf, width, pitch, height,
                           pixel_size, dst_buf, subsamp, flags);
            }
            break;
        }
        
        case 5: { // tjEncodeYUVPlanes
            if (comp_handle) {
                unsigned char *planes[3] = {NULL, NULL, NULL};
                int strides[3] = {0, 0, 0};
                
                // Calculate plane sizes using simplified calculation
                for (int i = 0; i < 3; i++) {
                    int plane_width = width;
                    int plane_height = height;
                    
                    // Adjust dimensions for chroma planes based on subsampling
                    if (i > 0 && subsamp != TJSAMP_GRAY && subsamp != TJSAMP_444) {
                        if (subsamp == TJSAMP_422 || subsamp == TJSAMP_420 || 
                            subsamp == TJSAMP_440 || subsamp == TJSAMP_411) {
                            plane_width = (width + 1) / 2;
                        }
                        if (subsamp == TJSAMP_420 || subsamp == TJSAMP_440) {
                            plane_height = (height + 1) / 2;
                        }
                    }
                    
                    size_t plane_size = (size_t)plane_width * (size_t)plane_height;
                    if (plane_size > 0 && plane_size < 10 * 1024 * 1024) {
                        planes[i] = (unsigned char *)malloc(plane_size);
                        if (planes[i]) {
                            memset(planes[i], 0, plane_size);
                        }
                    }
                }
                
                if (planes[0] || planes[1] || planes[2]) {
                    tjEncodeYUVPlanes(comp_handle, src_buf, width, pitch, height,
                                     pixel_format, planes, strides, subsamp, flags);
                }
                
                // Clean up planes
                for (int i = 0; i < 3; i++) {
                    free(planes[i]);
                }
            }
            break;
        }
    }

    // Cleanup
    destroy_handle(comp_handle);
    destroy_handle(decomp_handle);
    free(src_buf);
    free(dst_buf);
    free(pixel_dst_buf);

    return 0;
}