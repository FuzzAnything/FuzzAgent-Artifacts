// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MIN_WIDTH 1
#define MAX_WIDTH 256
#define MIN_HEIGHT 1
#define MAX_HEIGHT 256

static int get_valid_pixel_format(uint8_t value) {
    static const int formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XRGB, TJPF_XBGR, TJPF_GRAY, TJPF_RGBA,
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB
    };
    return formats[value % (sizeof(formats) / sizeof(formats[0]))];
}

static int get_valid_subsamp(uint8_t value) {
    static const int subsamps[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420,
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    return subsamps[value % (sizeof(subsamps) / sizeof(subsamps[0]))];
}

static int get_valid_alignment(uint8_t value) {
    static const int aligns[] = {1, 2, 4, 8, 16, 32};
    return aligns[value % (sizeof(aligns) / sizeof(aligns[0]))];
}

static int get_uv_width(int subsamp, int width) {
    switch (subsamp) {
        case TJSAMP_444: return width;
        case TJSAMP_422: return (width + 1) / 2;
        case TJSAMP_420: return (width + 1) / 2;
        case TJSAMP_GRAY: return 0;
        case TJSAMP_440: return width;
        case TJSAMP_411: return (width + 3) / 4;
        default: return (width + 1) / 2;
    }
}

static int get_uv_height(int subsamp, int height) {
    switch (subsamp) {
        case TJSAMP_444: return height;
        case TJSAMP_422: return height;
        case TJSAMP_420: return (height + 1) / 2;
        case TJSAMP_GRAY: return 0;
        case TJSAMP_440: return (height + 1) / 2;
        case TJSAMP_411: return height;
        default: return height;
    }
}

static void init_random_params(const uint8_t *data, size_t size,
                               int *width, int *height, int *pixel_format,
                               int *subsamp, int *align, int *pitch) {
    if (size < 8) {
        *width = 16;
        *height = 16;
        *pixel_format = TJPF_RGB;
        *subsamp = TJSAMP_420;
        *align = 4;
        *pitch = *width * tjPixelSize[TJPF_RGB];
        return;
    }
    
    *width = MIN_WIDTH + (data[0] % (MAX_WIDTH - MIN_WIDTH + 1));
    *height = MIN_HEIGHT + (data[1] % (MAX_HEIGHT - MIN_HEIGHT + 1));
    *pixel_format = get_valid_pixel_format(data[2]);
    *subsamp = get_valid_subsamp(data[3]);
    *align = get_valid_alignment(data[4]);
    *pitch = *width * tjPixelSize[*pixel_format];
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle handle = NULL;
    int width, height, pixel_format, subsamp, align, pitch;
    unsigned char *src_buf = NULL;
    unsigned char *dst_buf = NULL;
    unsigned char *yuv_buf = NULL;
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    size_t src_size, dst_size, yuv_size;
    int ret = 0;
    
    init_random_params(Data, Size, &width, &height, &pixel_format, 
                       &subsamp, &align, &pitch);
    
    if (width <= 0 || height <= 0 || pitch <= 0) {
        return 0;
    }
    
    src_size = (size_t)pitch * (size_t)height;
    if (src_size == 0 || src_size > 1024 * 1024) {
        return 0;
    }
    
    src_buf = (unsigned char*)malloc(src_size);
    if (!src_buf) return 0;
    
    size_t copy_size = Size > src_size ? src_size : Size;
    memcpy(src_buf, Data, copy_size);
    if (copy_size < src_size) {
        memset(src_buf + copy_size, 0, src_size - copy_size);
    }
    
    uint8_t func_selector = Data[0] % 6;
    
    switch (func_selector) {
        case 0: {
            handle = tjInitCompress();
            if (!handle) goto cleanup;
            
            yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuv_size == 0) goto cleanup;
            
            yuv_buf = (unsigned char*)malloc(yuv_size);
            if (!yuv_buf) goto cleanup;
            
            ret = tjEncodeYUV3(handle, src_buf, width, pitch, height,
                              pixel_format, yuv_buf, align, subsamp, 0);
            break;
        }
        
        case 1: {
            handle = tjInitCompress();
            if (!handle) goto cleanup;
            
            int y_width = width;
            int y_height = height;
            int uv_width = get_uv_width(subsamp, width);
            int uv_height = get_uv_height(subsamp, height);
            
            size_t y_plane_size = (size_t)y_width * (size_t)y_height;
            size_t u_plane_size = (size_t)uv_width * (size_t)uv_height;
            size_t v_plane_size = (size_t)uv_width * (size_t)uv_height;
            
            if (y_plane_size == 0) goto cleanup;
            
            planes[0] = (unsigned char*)malloc(y_plane_size);
            if (!planes[0]) goto cleanup;
            
            if (subsamp != TJSAMP_GRAY) {
                if (u_plane_size == 0 || v_plane_size == 0) goto cleanup;
                planes[1] = (unsigned char*)malloc(u_plane_size);
                planes[2] = (unsigned char*)malloc(v_plane_size);
                if (!planes[1] || !planes[2]) goto cleanup;
            }
            
            strides[0] = y_width;
            if (subsamp != TJSAMP_GRAY) {
                strides[1] = uv_width;
                strides[2] = uv_width;
            }
            
            ret = tj3EncodeYUVPlanes8(handle, src_buf, width, pitch, height,
                                     pixel_format, planes, strides);
            break;
        }
        
        case 2: {
            handle = tjInitDecompress();
            if (!handle) goto cleanup;
            
            yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuv_size == 0) goto cleanup;
            
            yuv_buf = (unsigned char*)malloc(yuv_size);
            if (!yuv_buf) goto cleanup;
            
            size_t yuv_copy_size = Size > yuv_size ? yuv_size : Size;
            memcpy(yuv_buf, Data, yuv_copy_size);
            if (yuv_copy_size < yuv_size) {
                memset(yuv_buf + yuv_copy_size, 0, yuv_size - yuv_copy_size);
            }
            
            dst_size = (size_t)pitch * (size_t)height;
            if (dst_size == 0) goto cleanup;
            
            dst_buf = (unsigned char*)malloc(dst_size);
            if (!dst_buf) goto cleanup;
            
            ret = tjDecodeYUV(handle, yuv_buf, align, subsamp, dst_buf,
                             width, pitch, height, pixel_format, 0);
            break;
        }
        
        case 3: {
            handle = tjInitCompress();
            if (!handle) goto cleanup;
            
            int y_width = width;
            int y_height = height;
            int uv_width = get_uv_width(subsamp, width);
            int uv_height = get_uv_height(subsamp, height);
            
            size_t y_plane_size = tjPlaneSizeYUV(0, width, 0, height, subsamp);
            size_t u_plane_size = tjPlaneSizeYUV(1, width, 0, height, subsamp);
            size_t v_plane_size = tjPlaneSizeYUV(2, width, 0, height, subsamp);
            
            if (y_plane_size == 0) goto cleanup;
            
            planes[0] = (unsigned char*)malloc(y_plane_size);
            if (!planes[0]) goto cleanup;
            
            if (subsamp != TJSAMP_GRAY) {
                if (u_plane_size == 0 || v_plane_size == 0) goto cleanup;
                planes[1] = (unsigned char*)malloc(u_plane_size);
                planes[2] = (unsigned char*)malloc(v_plane_size);
                if (!planes[1] || !planes[2]) goto cleanup;
            }
            
            strides[0] = y_width;
            if (subsamp != TJSAMP_GRAY) {
                strides[1] = uv_width;
                strides[2] = uv_width;
            }
            
            ret = tjEncodeYUVPlanes(handle, src_buf, width, pitch, height,
                                   pixel_format, planes, strides, subsamp, 0);
            break;
        }
        
        case 4: {
            handle = tjInitCompress();
            if (!handle) goto cleanup;
            
            yuv_size = tjBufSizeYUV(width, subsamp, height);
            if (yuv_size == 0) goto cleanup;
            
            yuv_buf = (unsigned char*)malloc(yuv_size);
            if (!yuv_buf) goto cleanup;
            
            int pixel_size = tjPixelSize[pixel_format];
            ret = tjEncodeYUV(handle, src_buf, width, pitch, height,
                             pixel_size, yuv_buf, subsamp, 0);
            break;
        }
        
        case 5: {
            handle = tjInitCompress();
            if (!handle) goto cleanup;
            
            yuv_size = tjBufSizeYUV2(width, 4, height, subsamp);
            if (yuv_size == 0) goto cleanup;
            
            yuv_buf = (unsigned char*)malloc(yuv_size);
            if (!yuv_buf) goto cleanup;
            
            ret = tjEncodeYUV2(handle, src_buf, width, pitch, height,
                              pixel_format, yuv_buf, subsamp, 0);
            break;
        }
    }
    
cleanup:
    if (handle) tjDestroy(handle);
    if (src_buf) free(src_buf);
    if (dst_buf) free(dst_buf);
    if (yuv_buf) free(yuv_buf);
    if (planes[0]) free(planes[0]);
    if (planes[1]) free(planes[1]);
    if (planes[2]) free(planes[2]);
    
    return 0;
}