// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static aom_codec_ctx_t *create_encoder_ctx(aom_codec_iface_t *iface, aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder_ctx(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void cleanup_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

static void cleanup_global_headers(aom_fixed_buf_t *hdr) {
    if (hdr && hdr->buf) {
        free(hdr->buf);
    }
    if (hdr) {
        free(hdr);
    }
}

int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Extract parameters from fuzz data
    uint32_t width = Data[0] | (Data[1] << 8);
    uint32_t height = Data[2] | (Data[3] << 8);
    uint32_t align = Data[4] | (Data[5] << 8);
    aom_img_fmt_t fmt = (aom_img_fmt_t)(Data[6] % 10); // Limit to known formats
    uint8_t bit_depth = Data[7] % 16 + 8; // 8, 10, 12
    uint8_t flags = Data[8];
    uint8_t duration_val = Data[9];

    // Validate dimensions to prevent overflow and excessive allocation
    if (width == 0 || height == 0 || width > 16384 || height > 16384) return 0;
    if (align == 0 || align > 65536) align = 16;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_bit_depth = bit_depth;
    cfg.g_profile = 0;
    // Removed cfg.aq_mode as it is not a member of aom_codec_enc_cfg in this version
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    aom_codec_ctx_t *ctx = create_encoder_ctx(iface, &cfg);
    if (!ctx) return 0;

    aom_image_t img;
    aom_image_t *img_ptr = NULL;
    aom_image_t *wrapped_img = NULL;
    unsigned char *user_buffer = NULL;

    // Test aom_img_alloc
    img_ptr = aom_img_alloc(&img, fmt, width, height, align);
    if (img_ptr) {
        // Fill image data with fuzz data to avoid undefined behavior
        for (int i = 0; i < 3; i++) {
            if (img_ptr->planes[i]) {
                size_t plane_size = (size_t)img_ptr->h * img_ptr->stride[i];
                size_t fill_len = (Size > plane_size) ? plane_size : Size;
                memset(img_ptr->planes[i], 0, plane_size);
                memcpy(img_ptr->planes[i], Data, fill_len);
            }
        }
    }

    // Test aom_img_wrap
    if (img_ptr) {
        // Calculate required size for wrapping
        size_t required_size = img_ptr->sz;
        user_buffer = (unsigned char *)malloc(required_size);
        if (user_buffer) {
            memcpy(user_buffer, img_ptr->img_data, required_size);
            wrapped_img = aom_img_wrap(NULL, fmt, width, height, align, user_buffer);
            if (wrapped_img) {
                // Fill wrapped image
                for (int i = 0; i < 3; i++) {
                    if (wrapped_img->planes[i]) {
                        size_t plane_size = (size_t)wrapped_img->h * wrapped_img->stride[i];
                        size_t fill_len = (Size > plane_size) ? plane_size : Size;
                        memset(wrapped_img->planes[i], 0, plane_size);
                        memcpy(wrapped_img->planes[i], Data, fill_len);
                    }
                }
            }
        }
    }

    // Test aom_codec_encode with allocated image
    aom_codec_pts_t pts = 0;
    unsigned long duration = duration_val;
    if (duration == 0) duration = 1; // Ensure non-zero duration for non-NULL img

    if (img_ptr) {
        aom_codec_encode(ctx, img_ptr, pts, duration, flags);
        pts += duration;
    }

    // Test aom_codec_encode with wrapped image
    if (wrapped_img) {
        aom_codec_encode(ctx, wrapped_img, pts, duration, flags);
        pts += duration;
    }

    // Test aom_codec_encode with NULL to flush
    while (1) {
        aom_codec_err_t res = aom_codec_encode(ctx, NULL, pts, duration, flags);
        if (res != AOM_CODEC_OK) break;
        
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(ctx, &iter);
        if (!pkt) break;
    }

    // Test aom_codec_get_preview_frame
    const aom_image_t *preview = aom_codec_get_preview_frame(ctx);
    (void)preview; // Use the result to avoid unused warning

    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *global_hdr = aom_codec_get_global_headers(ctx);
    cleanup_global_headers(global_hdr);

    // Cleanup
    if (img_ptr) cleanup_image(img_ptr);
    if (wrapped_img) cleanup_image(wrapped_img);
    if (user_buffer) free(user_buffer);
    cleanup_encoder_ctx(ctx);

    return 0;
}