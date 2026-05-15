// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static tjhandle init_compressor() {
    return tj3Init(TJINIT_COMPRESS);
}

static tjhandle init_decompressor() {
    return tj3Init(TJINIT_DECOMPRESS);
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static int get_valid_subsamp(int subsamp) {
    if (subsamp < 0 || subsamp > 4) {
        return TJSAMP_444;
    }
    return subsamp;
}

static int get_valid_pixel_format(int pixelFormat) {
    if (pixelFormat < 0 || pixelFormat >= TJ_NUMPF) {
        return TJPF_RGB;
    }
    return pixelFormat;
}

static int get_valid_align(int align) {
    if (align <= 0 || (align & (align - 1)) != 0) {
        return 4;
    }
    return align;
}

static int get_valid_width(int width) {
    if (width <= 0 || width > 4096) {
        return 64;
    }
    return width;
}

static int get_valid_height(int height) {
    if (height <= 0 || height > 4096) {
        return 64;
    }
    return height;
}

static int get_valid_pitch(int width, int pixelFormat) {
    int min_pitch = width * tjPixelSize[pixelFormat];
    if (min_pitch <= 0) {
        return 64;
    }
    return min_pitch;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    // Parse parameters from fuzz data
    const uint8_t *data_ptr = Data;
    size_t remaining = Size;
    
    int width = get_valid_width(*(int*)data_ptr % 4096);
    data_ptr += sizeof(int);
    remaining -= sizeof(int);
    
    int height = get_valid_height(*(int*)data_ptr % 4096);
    data_ptr += sizeof(int);
    remaining -= sizeof(int);
    
    int pixelFormat = get_valid_pixel_format(*(int*)data_ptr % TJ_NUMPF);
    data_ptr += sizeof(int);
    remaining -= sizeof(int);
    
    int pitch = get_valid_pitch(width, pixelFormat);
    if (remaining > sizeof(int)) {
        int fuzz_pitch = *(int*)data_ptr % 8192;
        data_ptr += sizeof(int);
        remaining -= sizeof(int);
        if (fuzz_pitch >= pitch) {
            pitch = fuzz_pitch;
        }
    }
    
    int subsamp = get_valid_subsamp(*(int*)data_ptr % 5);
    data_ptr += sizeof(int);
    remaining -= sizeof(int);
    
    int align = get_valid_align(*(int*)data_ptr % 32 + 1);
    data_ptr += sizeof(int);
    remaining -= sizeof(int);
    
    int flags = *(int*)data_ptr;
    data_ptr += sizeof(int);
    remaining -= sizeof(int);
    
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Prepare source buffer
    size_t src_size = (size_t)pitch * (size_t)height;
    if (src_size == 0 || src_size > remaining) {
        src_size = remaining > 0 ? remaining : 1;
    }
    
    unsigned char *src_buf = (unsigned char*)malloc(src_size);
    if (!src_buf) {
        return 0;
    }
    memcpy(src_buf, data_ptr, src_size);
    
    // Test tj3EncodeYUVPlanes8
    {
        tjhandle handle = init_compressor();
        if (handle) {
            tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
            
            // Calculate plane sizes with safe parameters
            int plane_width = width;
            int plane_height = height;
            if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422) {
                plane_height = (height + 1) / 2;
            }
            if (subsamp == TJSAMP_420 || subsamp == TJSAMP_440) {
                plane_width = (width + 1) / 2;
            }
            
            size_t y_size = (size_t)width * (size_t)height;
            size_t u_size = (size_t)plane_width * (size_t)plane_height;
            size_t v_size = (size_t)plane_width * (size_t)plane_height;
            
            unsigned char *y_plane = (unsigned char*)calloc(1, y_size);
            unsigned char *u_plane = (unsigned char*)calloc(1, u_size);
            unsigned char *v_plane = (unsigned char*)calloc(1, v_size);
            
            if (y_plane && u_plane && v_plane) {
                unsigned char *planes[3] = {y_plane, u_plane, v_plane};
                int strides[3] = {0, 0, 0};
                
                tj3EncodeYUVPlanes8(handle, src_buf, width, pitch, height, 
                                   pixelFormat, planes, strides);
            }
            
            free(y_plane);
            free(u_plane);
            free(v_plane);
            cleanup_handle(handle);
        }
    }
    
    // Test tjEncodeYUV
    {
        tjhandle handle = tjInitCompress();
        if (handle) {
            size_t dst_size = tjBufSizeYUV(width, height, subsamp);
            unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
            
            if (dst_buf) {
                tjEncodeYUV(handle, src_buf, width, pitch, height, pixelSize,
                           dst_buf, subsamp, flags);
                free(dst_buf);
            }
            tjDestroy(handle);
        }
    }
    
    // Test tjEncodeYUV3
    {
        tjhandle handle = tjInitCompress();
        if (handle) {
            size_t dst_size = tjBufSizeYUV2(width, align, height, subsamp);
            unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
            
            if (dst_buf) {
                tjEncodeYUV3(handle, src_buf, width, pitch, height, pixelFormat,
                            dst_buf, align, subsamp, flags);
                free(dst_buf);
            }
            tjDestroy(handle);
        }
    }
    
    // Test tj3EncodeYUV8
    {
        tjhandle handle = init_compressor();
        if (handle) {
            tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
            
            size_t dst_size = tj3YUVBufSize(width, align, height, subsamp);
            unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
            
            if (dst_buf) {
                tj3EncodeYUV8(handle, src_buf, width, pitch, height, pixelFormat,
                             dst_buf, align);
                free(dst_buf);
            }
            cleanup_handle(handle);
        }
    }
    
    // Test tjDecodeYUV
    {
        tjhandle handle = init_decompressor();
        if (handle) {
            size_t yuv_size = tjBufSizeYUV(width, height, subsamp);
            unsigned char *yuv_buf = (unsigned char*)malloc(yuv_size);
            
            if (yuv_buf) {
                size_t copy_size = yuv_size < remaining ? yuv_size : remaining;
                if (copy_size > 0) {
                    memcpy(yuv_buf, data_ptr, copy_size);
                }
                
                size_t dst_size = (size_t)pitch * (size_t)height;
                unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
                
                if (dst_buf) {
                    tjDecodeYUV(handle, yuv_buf, align, subsamp, dst_buf,
                               width, pitch, height, pixelFormat, flags);
                    free(dst_buf);
                }
            }
            
            free(yuv_buf);
            cleanup_handle(handle);
        }
    }
    
    // Test tjEncodeYUV2
    {
        tjhandle handle = tjInitCompress();
        if (handle) {
            size_t dst_size = tjBufSizeYUV2(width, align, height, subsamp);
            unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
            
            if (dst_buf) {
                tjEncodeYUV2(handle, src_buf, width, pitch, height, pixelFormat,
                            dst_buf, subsamp, flags);
                free(dst_buf);
            }
            tjDestroy(handle);
        }
    }
    
    free(src_buf);
    return 0;
}