// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "src/turbojpeg.h"

static const int MAX_DIMENSION = 1024;
static const int MAX_ALIGN = 32;

static int get_valid_dimension(uint8_t value) {
    if (value == 0) return 1;
    return (value % MAX_DIMENSION) + 1;
}

static int get_valid_align(uint8_t value) {
    int align = (value % MAX_ALIGN) + 1;
    while ((align & (align - 1)) != 0) {
        align++;
    }
    return align > MAX_ALIGN ? 1 : align;
}

static int get_valid_subsamp(uint8_t value) {
    int subsamp = value % 6;
    switch (subsamp) {
        case 0: return TJSAMP_444;
        case 1: return TJSAMP_422;
        case 2: return TJSAMP_420;
        case 3: return TJSAMP_GRAY;
        case 4: return TJSAMP_440;
        case 5: return TJSAMP_411;
        default: return TJSAMP_420;
    }
}

static int get_valid_pixel_format(uint8_t value) {
    int fmt = value % 12;
    switch (fmt) {
        case 0: return TJPF_RGB;
        case 1: return TJPF_BGR;
        case 2: return TJPF_RGBX;
        case 3: return TJPF_BGRX;
        case 4: return TJPF_XBGR;
        case 5: return TJPF_XRGB;
        case 6: return TJPF_GRAY;
        case 7: return TJPF_RGBA;
        case 8: return TJPF_BGRA;
        case 9: return TJPF_ABGR;
        case 10: return TJPF_ARGB;
        case 11: return TJPF_CMYK;
        default: return TJPF_RGB;
    }
}

static int get_valid_param(uint8_t value) {
    int param = value % 20;
    switch (param) {
        case 0: return TJPARAM_STOPONWARNING;
        case 1: return TJPARAM_BOTTOMUP;
        case 2: return TJPARAM_FASTUPSAMPLE;
        case 3: return TJPARAM_FASTDCT;
        case 4: return TJPARAM_OPTIMIZE;
        case 5: return TJPARAM_PROGRESSIVE;
        case 6: return TJPARAM_SCANLIMIT;
        case 7: return TJPARAM_ARITHMETIC;
        case 8: return TJPARAM_LOSSLESS;
        case 9: return TJPARAM_LOSSLESSPSV;
        case 10: return TJPARAM_LOSSLESSPT;
        case 11: return TJPARAM_RESTARTROWS;
        case 12: return TJPARAM_RESTARTBLOCKS;
        case 13: return TJPARAM_SUBSAMP;
        case 14: return TJPARAM_JPEGWIDTH;
        case 15: return TJPARAM_JPEGHEIGHT;
        case 16: return TJPARAM_PRECISION;
        case 17: return TJPARAM_COLORSPACE;
        case 18: return TJPARAM_QUALITY;
        case 19: return TJPARAM_NOREALLOC;
        default: return TJPARAM_QUALITY;
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    uint8_t param_selector = Data[0];
    int width = get_valid_dimension(Data[1]);
    int height = get_valid_dimension(Data[2]);
    int align = get_valid_align(Data[3]);
    int subsamp = get_valid_subsamp(Data[4]);
    int pixelFormat = get_valid_pixel_format(Data[5]);
    int param = get_valid_param(Data[6]);
    int value = Data[7] % 256;
    
    tjhandle comp_handle = NULL;
    tjhandle decomp_handle = NULL;
    
    comp_handle = tj3Init(TJINIT_COMPRESS);
    decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        return 0;
    }
    
    size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);
    
    if (yuv_buf_size > 0 && yuv_buf_size < 1024 * 1024 * 10 && yuv_buf_size <= Size) {
        unsigned char *yuv_buf = (unsigned char *)malloc(yuv_buf_size);
        if (yuv_buf) {
            memcpy(yuv_buf, Data, yuv_buf_size);
            
            int pitch = width * tjPixelSize[pixelFormat];
            if (pitch <= 0) pitch = width * 4;
            
            unsigned char *dst_buf = (unsigned char *)malloc(pitch * height);
            if (dst_buf) {
                tj3Set(decomp_handle, TJPARAM_SUBSAMP, subsamp);
                tj3DecodeYUV8(decomp_handle, yuv_buf, align, dst_buf, width, pitch, height, pixelFormat);
                
                unsigned char *encoded_yuv = (unsigned char *)malloc(yuv_buf_size);
                if (encoded_yuv) {
                    tj3Set(comp_handle, TJPARAM_SUBSAMP, subsamp);
                    tj3EncodeYUV8(comp_handle, dst_buf, width, pitch, height, pixelFormat, encoded_yuv, align);
                    free(encoded_yuv);
                }
                
                free(dst_buf);
            }
            
            size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
            if (jpeg_buf_size > 0 && jpeg_buf_size < 1024 * 1024 * 10) {
                unsigned char *jpeg_buf = NULL;
                size_t jpeg_size = jpeg_buf_size;
                
                tj3Set(comp_handle, TJPARAM_SUBSAMP, subsamp);
                tj3Set(comp_handle, param, value);
                
                tj3CompressFromYUV8(comp_handle, yuv_buf, width, align, height, &jpeg_buf, &jpeg_size);
                
                if (jpeg_buf) {
                    free(jpeg_buf);
                }
            }
            
            free(yuv_buf);
        }
    }
    
    tj3Set(comp_handle, param, value);
    tj3Set(decomp_handle, param, value);
    
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    
    return 0;
}