// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/turbojpeg.h"

#define MIN_DIMENSION 1
#define MAX_DIMENSION 256
#define MAX_PLANES 3

static void init_handle_and_buffers(tjhandle *comp_handle, tjhandle *decomp_handle,
                                   unsigned char **src_buf, unsigned char **dst_buf,
                                   unsigned char **jpeg_buf, unsigned long *jpeg_size,
                                   const uint8_t *data, size_t size, int width, int height,
                                   int pixel_format) {
    *comp_handle = tjInitCompress();
    *decomp_handle = tjInitDecompress();
    
    int pixel_size = 0;
    switch(pixel_format) {
        case TJPF_GRAY: pixel_size = 1; break;
        case TJPF_RGB: pixel_size = 3; break;
        case TJPF_BGR: pixel_size = 3; break;
        case TJPF_RGBX: pixel_size = 4; break;
        case TJPF_BGRX: pixel_size = 4; break;
        case TJPF_XRGB: pixel_size = 4; break;
        case TJPF_XBGR: pixel_size = 4; break;
        default: pixel_size = 3; break;
    }
    
    size_t src_size = width * height * pixel_size;
    *src_buf = (unsigned char*)malloc(src_size);
    if (*src_buf && size > 0) {
        size_t copy_size = size < src_size ? size : src_size;
        memcpy(*src_buf, data, copy_size);
        if (copy_size < src_size) {
            memset(*src_buf + copy_size, 0, src_size - copy_size);
        }
    }
    
    *dst_buf = (unsigned char*)malloc(src_size);
    *jpeg_buf = NULL;
    *jpeg_size = 0;
}

static void cleanup_buffers(unsigned char *src_buf, unsigned char *dst_buf,
                           unsigned char *jpeg_buf) {
    if (src_buf) free(src_buf);
    if (dst_buf) free(dst_buf);
    if (jpeg_buf) tjFree(jpeg_buf);
}

static void cleanup_handles(tjhandle comp_handle, tjhandle decomp_handle) {
    if (comp_handle) tjDestroy(comp_handle);
    if (decomp_handle) tjDestroy(decomp_handle);
}

static int get_valid_dimension(const uint8_t *data, size_t offset, size_t size) {
    if (offset < size) {
        int dim = data[offset] % (MAX_DIMENSION - MIN_DIMENSION + 1) + MIN_DIMENSION;
        return dim;
    }
    return 16;
}

static int get_pixel_format(const uint8_t *data, size_t offset, size_t size) {
    static const int formats[] = {
        TJPF_GRAY, TJPF_RGB, TJPF_BGR, TJPF_RGBX,
        TJPF_BGRX, TJPF_XRGB, TJPF_XBGR
    };
    if (offset < size) {
        return formats[data[offset] % (sizeof(formats)/sizeof(formats[0]))];
    }
    return TJPF_RGB;
}

static int get_subsamp(const uint8_t *data, size_t offset, size_t size) {
    static const int subsamps[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420,
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    if (offset < size) {
        return subsamps[data[offset] % (sizeof(subsamps)/sizeof(subsamps[0]))];
    }
    return TJSAMP_420;
}

static int get_quality(const uint8_t *data, size_t offset, size_t size) {
    if (offset < size) {
        return data[offset] % 101;
    }
    return 75;
}

static int get_flags(const uint8_t *data, size_t offset, size_t size) {
    int flags = 0;
    if (offset < size) {
        if (data[offset] & 0x01) flags |= TJFLAG_BOTTOMUP;
        if (data[offset] & 0x02) flags |= TJFLAG_FASTUPSAMPLE;
        if (data[offset] & 0x04) flags |= TJFLAG_FASTDCT;
        if (data[offset] & 0x08) flags |= TJFLAG_ACCURATEDCT;
        if (data[offset] & 0x10) flags |= TJFLAG_NOREALLOC;
    }
    return flags;
}

static void test_tjDecodeYUVPlanes(tjhandle decomp_handle, const uint8_t *data, size_t size,
                                  int width, int height, int subsamp, int pixel_format, int flags) {
    const unsigned char *src_planes[MAX_PLANES] = {0};
    int strides[MAX_PLANES] = {0};
    unsigned char *dst_buf = NULL;
    
    int y_width = width;
    int y_height = height;
    int uv_width = tjPlaneWidth(0, width, subsamp);
    int uv_height = tjPlaneHeight(0, height, subsamp);
    
    size_t y_size = y_width * y_height;
    size_t uv_size = uv_width * uv_height;
    
    unsigned char *y_plane = (unsigned char*)malloc(y_size);
    unsigned char *u_plane = (unsigned char*)malloc(uv_size);
    unsigned char *v_plane = (unsigned char*)malloc(uv_size);
    
    if (!y_plane || !u_plane || !v_plane) {
        free(y_plane);
        free(u_plane);
        free(v_plane);
        return;
    }
    
    size_t offset = 0;
    size_t copy_size;
    
    copy_size = size - offset < y_size ? size - offset : y_size;
    if (copy_size > 0) memcpy(y_plane, data + offset, copy_size);
    offset += copy_size;
    
    copy_size = size - offset < uv_size ? size - offset : uv_size;
    if (copy_size > 0) memcpy(u_plane, data + offset, copy_size);
    offset += copy_size;
    
    copy_size = size - offset < uv_size ? size - offset : uv_size;
    if (copy_size > 0) memcpy(v_plane, data + offset, copy_size);
    
    src_planes[0] = y_plane;
    src_planes[1] = u_plane;
    src_planes[2] = v_plane;
    
    strides[0] = y_width;
    strides[1] = uv_width;
    strides[2] = uv_width;
    
    int pixel_size = 0;
    switch(pixel_format) {
        case TJPF_GRAY: pixel_size = 1; break;
        case TJPF_RGB: pixel_size = 3; break;
        case TJPF_BGR: pixel_size = 3; break;
        case TJPF_RGBX: pixel_size = 4; break;
        case TJPF_BGRX: pixel_size = 4; break;
        case TJPF_XRGB: pixel_size = 4; break;
        case TJPF_XBGR: pixel_size = 4; break;
        default: pixel_size = 3; break;
    }
    
    size_t dst_size = width * height * pixel_size;
    dst_buf = (unsigned char*)malloc(dst_size);
    if (!dst_buf) {
        free(y_plane);
        free(u_plane);
        free(v_plane);
        return;
    }
    
    int pitch = width * pixel_size;
    
    tjDecodeYUVPlanes(decomp_handle, src_planes, strides, subsamp,
                     dst_buf, width, pitch, height, pixel_format, flags);
    
    tjGetErrorCode(decomp_handle);
    
    free(y_plane);
    free(u_plane);
    free(v_plane);
    free(dst_buf);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    tjhandle comp_handle = NULL;
    tjhandle decomp_handle = NULL;
    unsigned char *src_buf = NULL;
    unsigned char *dst_buf = NULL;
    unsigned char *jpeg_buf = NULL;
    unsigned long jpeg_size = 0;
    
    int width = get_valid_dimension(Data, 0, Size);
    int height = get_valid_dimension(Data, 1, Size);
    int pixel_format = get_pixel_format(Data, 2, Size);
    int subsamp = get_subsamp(Data, 3, Size);
    int quality = get_quality(Data, 4, Size);
    int flags = get_flags(Data, 5, Size);
    
    init_handle_and_buffers(&comp_handle, &decomp_handle, &src_buf, &dst_buf,
                           &jpeg_buf, &jpeg_size, Data, Size, width, height, pixel_format);
    
    if (!comp_handle || !decomp_handle || !src_buf || !dst_buf) {
        cleanup_buffers(src_buf, dst_buf, jpeg_buf);
        cleanup_handles(comp_handle, decomp_handle);
        return 0;
    }
    
    int pitch = width * (pixel_format == TJPF_GRAY ? 1 : 
                        (pixel_format == TJPF_RGB || pixel_format == TJPF_BGR ? 3 : 4));
    
    int align = 1;
    if (Size > 6) {
        align = (Data[6] % 4) + 1;
    }
    
    int test_case = 0;
    if (Size > 7) {
        test_case = Data[7] % 4;
    }
    
    switch(test_case) {
        case 0:
            tjCompress2(comp_handle, src_buf, width, pitch, height, pixel_format,
                       &jpeg_buf, &jpeg_size, subsamp, quality, flags);
            tjGetErrorCode(comp_handle);
            break;
            
        case 1:
            tjCompressFromYUV(comp_handle, src_buf, width, align, height, subsamp,
                             &jpeg_buf, &jpeg_size, quality, flags);
            tjGetErrorCode(comp_handle);
            break;
            
        case 2:
            tjCompress(comp_handle, src_buf, width, pitch, height,
                      (pixel_format == TJPF_GRAY ? 1 : 
                       (pixel_format == TJPF_RGB || pixel_format == TJPF_BGR ? 3 : 4)),
                      dst_buf, &jpeg_size, subsamp, quality, flags);
            tjGetErrorCode(comp_handle);
            break;
            
        case 3:
            test_tjDecodeYUVPlanes(decomp_handle, Data, Size, width, height,
                                  subsamp, pixel_format, flags);
            break;
    }
    
    tj3GetErrorCode(comp_handle);
    tj3GetErrorCode(decomp_handle);
    
    cleanup_buffers(src_buf, dst_buf, jpeg_buf);
    cleanup_handles(comp_handle, decomp_handle);
    
    return 0;
}