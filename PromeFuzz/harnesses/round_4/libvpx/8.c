// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"

static vpx_img_fmt_t get_random_format(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return VPX_IMG_FMT_I420;
    
    vpx_img_fmt_t formats[] = {
        VPX_IMG_FMT_YV12,
        VPX_IMG_FMT_I420,
        VPX_IMG_FMT_I422,
        VPX_IMG_FMT_I444,
        VPX_IMG_FMT_I440,
        VPX_IMG_FMT_NV12,
        VPX_IMG_FMT_I42016,
        VPX_IMG_FMT_I42216,
        VPX_IMG_FMT_I44416,
        VPX_IMG_FMT_I44016
    };
    
    uint8_t idx = data[*offset] % (sizeof(formats) / sizeof(formats[0]));
    (*offset)++;
    return formats[idx];
}

static unsigned int get_random_dimension(const uint8_t *data, size_t *offset, size_t size, unsigned int max) {
    if (*offset + 2 > size) return 16;
    
    unsigned int val = (data[*offset] << 8) | data[*offset + 1];
    (*offset) += 2;
    return (val % max) + 1;
}

static unsigned int get_random_alignment(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return 16;
    
    unsigned int align = 1 << (data[*offset] % 6); // 1, 2, 4, 8, 16, 32
    (*offset)++;
    return align;
}

static int initialize_codec(vpx_codec_ctx_t *ctx) {
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    
    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test vpx_img_alloc
    vpx_img_fmt_t fmt = get_random_format(Data, &offset, Size);
    unsigned int width = get_random_dimension(Data, &offset, Size, 256);
    unsigned int height = get_random_dimension(Data, &offset, Size, 256);
    unsigned int align = get_random_alignment(Data, &offset, Size);
    
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) return 0;
    
    // Test vpx_img_flip
    vpx_img_flip(img);
    
    // Test vpx_img_set_rect
    if (offset + 8 <= Size) {
        unsigned int x = Data[offset] % (width / 2 + 1);
        unsigned int y = Data[offset + 1] % (height / 2 + 1);
        unsigned int w = Data[offset + 2] % (width - x) + 1;
        unsigned int h = Data[offset + 3] % (height - y) + 1;
        offset += 4;
        
        vpx_img_set_rect(img, x, y, w, h);
    }
    
    // Test vpx_img_wrap
    if (offset + 4 <= Size) {
        unsigned int wrap_width = get_random_dimension(Data, &offset, Size, 128);
        unsigned int wrap_height = get_random_dimension(Data, &offset, Size, 128);
        unsigned int wrap_align = get_random_alignment(Data, &offset, Size);
        
        // Calculate required buffer size for wrapping
        size_t buffer_size = wrap_width * wrap_height * 3; // Conservative estimate
        unsigned char *buffer = (unsigned char *)malloc(buffer_size);
        if (buffer) {
            vpx_image_t *wrapped_img = vpx_img_wrap(NULL, fmt, wrap_width, wrap_height, wrap_align, buffer);
            if (wrapped_img) {
                vpx_img_free(wrapped_img);
            }
            free(buffer);
        }
    }
    
    // Test vpx_codec_encode
    vpx_codec_ctx_t ctx;
    static vpx_codec_pts_t pts = 0;
    
    if (initialize_codec(&ctx)) {
        vpx_codec_encode(&ctx, img, pts, 1, 0, 0);
        pts++;
        
        // Flush encoder
        vpx_codec_encode(&ctx, NULL, pts, 0, 0, 0);
        pts++;
        
        vpx_codec_destroy(&ctx);
    }
    
    // Flip back for cleanup
    vpx_img_flip(img);
    
    // Test vpx_img_free
    vpx_img_free(img);
    
    return 0;
}