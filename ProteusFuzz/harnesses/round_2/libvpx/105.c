// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static void safe_free_image(vpx_image_t *img) {
    if (img) {
        vpx_img_free(img);
    }
}

static vpx_image_t *create_image_from_fuzz(const uint8_t *data, size_t size) {
    vpx_image_t *img = NULL;
    unsigned int w, h, align;
    vpx_img_fmt_t fmt;

    if (size < 16) {
        return NULL;
    }

    // Extract parameters from fuzz data safely
    w = *(uint32_t *)(data);
    h = *(uint32_t *)(data + 4);
    align = *(uint32_t *)(data + 8);
    fmt = (vpx_img_fmt_t)(*(uint32_t *)(data + 12));

    // Sanity checks to prevent crashes or invalid allocations
    if (w == 0 || h == 0 || w > 0x08000000 || h > 0x08000000) {
        return NULL;
    }
    if (align == 0 || align > 65536) {
        align = 16; // Default safe alignment
    }
    if (fmt >= VPX_IMG_FMT_I44016) {
        fmt = VPX_IMG_FMT_I420; // Default safe format
    }

    // Allocate image
    img = vpx_img_alloc(NULL, fmt, w, h, align);
    if (img) {
        // Fill image data with fuzz input to test encoding paths
        size_t total_size = 0;
        for (int i = 0; i < 4; i++) {
            if (img->planes[i]) {
                total_size += (size_t)img->stride[i] * img->h;
            }
        }
        if (total_size > 0 && size > 16) {
            size_t copy_len = (size - 16) < total_size ? (size - 16) : total_size;
            memcpy(img->planes[0], data + 16, copy_len);
        }
    }
    return img;
}

static vpx_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = NULL;
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    if (!iface) return NULL;

    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return NULL;

    // Fuzz configuration parameters
    if (size > 8) {
        cfg.g_w = *(uint32_t *)data;
        cfg.g_h = *(uint32_t *)(data + 4);
    }
    // Ensure valid dimensions
    if (cfg.g_w == 0 || cfg.g_h == 0 || cfg.g_w > 0x08000000 || cfg.g_h > 0x08000000) {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    err = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // 1. Test vpx_img_alloc and vpx_img_free
    vpx_image_t *img_alloc = create_image_from_fuzz(Data, Size);
    if (img_alloc) {
        vpx_img_free(img_alloc);
    }

    // 2. Test vpx_img_wrap
    vpx_image_t img_wrap_struct;
    memset(&img_wrap_struct, 0, sizeof(img_wrap_struct));
    
    unsigned int w = (Size > 4) ? *(uint32_t *)Data : 64;
    unsigned int h = (Size > 8) ? *(uint32_t *)(Data + 4) : 64;
    unsigned int align = (Size > 12) ? *(uint32_t *)(Data + 8) : 16;
    vpx_img_fmt_t fmt = (Size > 12) ? (vpx_img_fmt_t)(*(uint32_t *)(Data + 12)) : VPX_IMG_FMT_I420;

    if (w == 0 || h == 0 || w > 0x08000000 || h > 0x08000000) {
        w = 64; h = 64;
    }
    if (align == 0 || align > 65536) align = 16;
    if (fmt >= VPX_IMG_FMT_I44016) fmt = VPX_IMG_FMT_I420;

    // Calculate buffer size roughly (simplified for fuzzing)
    size_t buf_size = (size_t)w * h * 2; 
    unsigned char *buf = (unsigned char *)malloc(buf_size);
    if (buf) {
        memset(buf, 0, buf_size);
        vpx_image_t *img_wrap = vpx_img_wrap(&img_wrap_struct, fmt, w, h, align, buf);
        if (img_wrap) {
            // Test vpx_img_flip on wrapped image
            vpx_img_flip(img_wrap);
            
            // Test vpx_img_set_rect on wrapped image
            // Ensure we have enough data before reading rect parameters
            unsigned int rect_x = 0;
            unsigned int rect_y = 0;
            unsigned int rect_w = w;
            unsigned int rect_h = h;
            
            if (Size > 28) {
                rect_x = *(uint32_t *)(Data + 16);
                rect_y = *(uint32_t *)(Data + 20);
                rect_w = *(uint32_t *)(Data + 24);
                rect_h = *(uint32_t *)(Data + 28);
            }
            
            // Clamp rect to image bounds
            if (rect_x >= w) rect_x = 0;
            if (rect_y >= h) rect_y = 0;
            if (rect_w > w - rect_x) rect_w = w - rect_x;
            if (rect_h > h - rect_y) rect_h = h - rect_y;
            
            vpx_img_set_rect(img_wrap, rect_x, rect_y, rect_w, rect_h);
            vpx_img_free(img_wrap);
        }
        free(buf);
    }

    // 3. Test vpx_codec_encode
    vpx_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (ctx) {
        vpx_image_t *img_encode = create_image_from_fuzz(Data, Size);
        if (img_encode) {
            // Ensure image dimensions match encoder config if possible, or just pass it
            // The encoder might reject mismatched sizes, which is fine for fuzzing
            vpx_codec_pts_t pts = 0;
            unsigned long duration = 1;
            vpx_enc_frame_flags_t flags = 0;
            vpx_enc_deadline_t deadline = 0;

            if (Size > 48) {
                pts = *(int64_t *)(Data + 32);
                duration = *(unsigned long *)(Data + 40);
                flags = *(long *)(Data + 48);
                if (Size > 56) {
                    deadline = *(unsigned long *)(Data + 56);
                }
            }

            vpx_codec_encode(ctx, img_encode, pts, duration, flags, deadline);
            
            // Flush encoder
            vpx_codec_encode(ctx, NULL, pts + 1, duration, flags, deadline);
            
            safe_free_image(img_encode);
        }
        
        // Cleanup encoder
        vpx_codec_destroy(ctx);
        free(ctx);
    }

    return 0;
}