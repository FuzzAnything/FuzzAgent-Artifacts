// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

#ifdef __cplusplus
extern "C" {
#endif

static unsigned int get_uint(const uint8_t **data, size_t *size) {
    if (*size < sizeof(unsigned int)) return 0;
    unsigned int val = *(unsigned int*)(*data);
    *data += sizeof(unsigned int);
    *size -= sizeof(unsigned int);
    return val;
}

static vpx_img_fmt_t get_fmt(const uint8_t **data, size_t *size) {
    if (*size < sizeof(vpx_img_fmt_t)) return VPX_IMG_FMT_I420;
    vpx_img_fmt_t fmt = *(vpx_img_fmt_t*)(*data);
    *data += sizeof(vpx_img_fmt_t);
    *size -= sizeof(vpx_img_fmt_t);
    return fmt;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    const uint8_t *orig_data = data;
    size_t orig_size = size;

    uint8_t func_selector = data[0];
    data++; size--;

    switch (func_selector % 6) {
        case 0: { // vpx_img_set_rect
            vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 64, 64, 1);
            if (!img) return 0;
            
            unsigned int x = get_uint(&data, &size);
            unsigned int y = get_uint(&data, &size);
            unsigned int w = get_uint(&data, &size);
            unsigned int h = get_uint(&data, &size);
            
            vpx_img_set_rect(img, x, y, w, h);
            vpx_img_free(img);
            break;
        }
        case 1: { // vpx_img_free
            vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 32, 32, 1);
            vpx_img_free(img);
            vpx_img_free(NULL); // Test with NULL
            break;
        }
        case 2: { // vpx_codec_encode
            vpx_codec_ctx_t ctx;
            vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
            if (!iface) return 0;
            
            vpx_codec_enc_cfg_t cfg;
            if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) return 0;
            
            cfg.g_w = 16;
            cfg.g_h = 16;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 1000;
            
            if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) return 0;
            
            vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 16, 16, 1);
            if (img) {
                vpx_codec_pts_t pts = get_uint(&data, &size);
                unsigned long duration = get_uint(&data, &size);
                vpx_enc_frame_flags_t flags = get_uint(&data, &size);
                vpx_enc_deadline_t deadline = get_uint(&data, &size);
                
                vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
                vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline); // Flush
                vpx_img_free(img);
            }
            
            vpx_codec_destroy(&ctx);
            break;
        }
        case 3: { // vpx_img_wrap
            vpx_img_fmt_t fmt = get_fmt(&data, &size);
            unsigned int d_w = get_uint(&data, &size) % 256;
            unsigned int d_h = get_uint(&data, &size) % 256;
            unsigned int stride_align = (get_uint(&data, &size) % 64) + 1;
            
            size_t buffer_size = d_w * d_h * 3 / 2;
            if (buffer_size > 0 && buffer_size < 1024 * 1024) {
                unsigned char *img_data = (unsigned char*)malloc(buffer_size);
                if (img_data) {
                    vpx_image_t *img = vpx_img_wrap(NULL, fmt, d_w, d_h, stride_align, img_data);
                    if (img) {
                        vpx_img_free(img);
                    }
                    free(img_data);
                }
            }
            break;
        }
        case 4: { // vpx_img_alloc
            vpx_img_fmt_t fmt = get_fmt(&data, &size);
            unsigned int d_w = get_uint(&data, &size) % 1024;
            unsigned int d_h = get_uint(&data, &size) % 1024;
            unsigned int align = (get_uint(&data, &size) % 64) + 1;
            
            vpx_image_t *img = vpx_img_alloc(NULL, fmt, d_w, d_h, align);
            if (img) {
                vpx_img_free(img);
            }
            break;
        }
        case 5: { // vpx_img_flip
            vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 64, 64, 1);
            if (img) {
                vpx_img_flip(img);
                vpx_img_free(img);
            }
            break;
        }
    }

    return 0;
}

#ifdef __cplusplus
}
#endif