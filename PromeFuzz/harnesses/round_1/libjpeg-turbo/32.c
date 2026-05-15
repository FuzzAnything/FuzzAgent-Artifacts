// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_compressor(tjhandle *comp) {
    *comp = tjInitCompress();
}

static void init_decompressor(tjhandle *decomp) {
    *decomp = tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static int get_valid_subsamp(int subsamp) {
    if (subsamp < TJSAMP_444 || subsamp > TJSAMP_GRAY) {
        return TJSAMP_420;
    }
    return subsamp;
}

static int get_valid_pixel_format(int pixel_format) {
    if (pixel_format < TJPF_RGB || pixel_format > TJPF_CMYK) {
        return TJPF_RGB;
    }
    return pixel_format;
}

static int get_valid_alignment(int align) {
    if (align <= 0) {
        return 1;
    }
    int valid_align = 1;
    while (valid_align < align && valid_align < 1024) {
        valid_align <<= 1;
    }
    if (valid_align > 1024) {
        valid_align = 1;
    }
    return valid_align;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    tjhandle comp_handle = NULL;
    tjhandle decomp_handle = NULL;
    
    init_compressor(&comp_handle);
    init_decompressor(&decomp_handle);
    
    if (!comp_handle || !decomp_handle) {
        cleanup_handle(comp_handle);
        cleanup_handle(decomp_handle);
        return 0;
    }

    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int subsamp = Data[4] % 5;
    int pixel_format = Data[5] % 13;
    int align = Data[6] % 32;
    int flags = Data[7] & 0x0F;
    
    width = (width % 1024) + 1;
    height = (height % 1024) + 1;
    subsamp = get_valid_subsamp(subsamp);
    pixel_format = get_valid_pixel_format(pixel_format);
    align = get_valid_alignment(align);
    
    size_t rgb_size = width * height * tjPixelSize[pixel_format];
    if (rgb_size == 0 || rgb_size > 100 * 1024 * 1024) {
        cleanup_handle(comp_handle);
        cleanup_handle(decomp_handle);
        return 0;
    }

    unsigned char *src_buf = (unsigned char *)malloc(rgb_size);
    if (!src_buf) {
        cleanup_handle(comp_handle);
        cleanup_handle(decomp_handle);
        return 0;
    }

    size_t copy_size = Size - 8;
    if (copy_size > rgb_size) {
        copy_size = rgb_size;
    }
    memcpy(src_buf, Data + 8, copy_size);
    
    size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);
    if (yuv_buf_size == 0 || yuv_buf_size > 100 * 1024 * 1024) {
        free(src_buf);
        cleanup_handle(comp_handle);
        cleanup_handle(decomp_handle);
        return 0;
    }

    unsigned char *yuv_buf = (unsigned char *)malloc(yuv_buf_size);
    if (!yuv_buf) {
        free(src_buf);
        cleanup_handle(comp_handle);
        cleanup_handle(decomp_handle);
        return 0;
    }

    unsigned char *dst_buf = (unsigned char *)malloc(rgb_size);
    if (!dst_buf) {
        free(src_buf);
        free(yuv_buf);
        cleanup_handle(comp_handle);
        cleanup_handle(decomp_handle);
        return 0;
    }

    int pitch = width * tjPixelSize[pixel_format];
    
    unsigned char *planes[3];
    int strides[3];
    
    int plane_sizes[3];
    plane_sizes[0] = width * height;
    plane_sizes[1] = (subsamp == TJSAMP_GRAY) ? 0 : (width * height) / (subsamp == TJSAMP_420 || subsamp == TJSAMP_422 ? 2 : 4);
    plane_sizes[2] = plane_sizes[1];
    
    for (int i = 0; i < 3; i++) {
        if (plane_sizes[i] > 0) {
            planes[i] = (unsigned char *)malloc(plane_sizes[i]);
            strides[i] = width;
        } else {
            planes[i] = NULL;
            strides[i] = 0;
        }
    }

    unsigned char *const src_planes[3] = {planes[0], planes[1], planes[2]};
    
    tj3Set(decomp_handle, TJPARAM_SUBSAMP, subsamp);

    int test_case = Data[0] % 6;
    
    switch (test_case) {
        case 0:
            tjEncodeYUVPlanes(comp_handle, src_buf, width, pitch, height,
                             pixel_format, planes, strides, subsamp, flags);
            break;
        case 1:
            tj3DecodeYUV8(decomp_handle, yuv_buf, align, dst_buf, width,
                         pitch, height, pixel_format);
            break;
        case 2:
            tjDecodeYUV(decomp_handle, yuv_buf, align, subsamp, dst_buf,
                       width, pitch, height, pixel_format, flags);
            break;
        case 3:
            tj3YUVBufSize(width, align, height, subsamp);
            break;
        case 4:
            tjDecodeYUVPlanes(decomp_handle, (const unsigned char **)src_planes,
                             strides, subsamp, dst_buf, width, pitch,
                             height, pixel_format, flags);
            break;
        case 5:
            tj3DecodeYUVPlanes8(decomp_handle, src_planes, strides, dst_buf,
                               width, pitch, height, pixel_format);
            break;
    }

    free(src_buf);
    free(yuv_buf);
    free(dst_buf);
    
    for (int i = 0; i < 3; i++) {
        if (planes[i]) {
            free(planes[i]);
        }
    }
    
    cleanup_handle(comp_handle);
    cleanup_handle(decomp_handle);
    
    return 0;
}