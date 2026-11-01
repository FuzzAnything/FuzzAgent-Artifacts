// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static void free_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

static int create_dummy_image(aom_image_t *img, int w, int h, aom_img_fmt_t fmt, int bit_depth) {
    if (w <= 0 || h <= 0) return 0;
    if (aom_img_alloc(img, fmt, w, h, 16) == NULL) return 0;
    img->bit_depth = bit_depth;
    return 1;
}

int LLVMFuzzerTestOneInput_277(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    uint32_t params[4];
    memcpy(params, Data, sizeof(uint32_t));
    uint32_t width = params[0];
    uint32_t height = params[1];
    uint32_t bit_depth = params[2] & 0x3;
    uint32_t fmt_idx = params[3] & 0x7;

    if (width == 0 || height == 0) return 0;
    if (width > 4096 || height > 4096) return 0;

    int actual_bit_depth = 8;
    if (bit_depth == 1) actual_bit_depth = 10;
    else if (bit_depth == 2) actual_bit_depth = 12;
    else if (bit_depth == 3) actual_bit_depth = 16;

    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (actual_bit_depth > 8) {
        if (fmt_idx == 0) fmt = AOM_IMG_FMT_I42016;
        else if (fmt_idx == 1) fmt = AOM_IMG_FMT_I42216;
        else if (fmt_idx == 2) fmt = AOM_IMG_FMT_I44416;
        else if (fmt_idx == 3) fmt = AOM_IMG_FMT_YV1216;
    } else {
        if (fmt_idx == 0) fmt = AOM_IMG_FMT_I420;
        else if (fmt_idx == 1) fmt = AOM_IMG_FMT_I422;
        else if (fmt_idx == 2) fmt = AOM_IMG_FMT_I444;
        else if (fmt_idx == 3) fmt = AOM_IMG_FMT_YV12;
        else if (fmt_idx == 4) fmt = AOM_IMG_FMT_NV12;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = actual_bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0;
    cfg.g_pass = 0;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_image_t img;
    if (!create_dummy_image(&img, width, height, fmt, actual_bit_depth)) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    size_t plane_size = img.planes[0] ? img.stride[0] * img.h : 0;
    if (plane_size > 0 && Size > 4) {
        size_t data_len = Size - 4;
        size_t copy_len = (data_len < plane_size) ? data_len : plane_size;
        memcpy(img.planes[0], Data + 4, copy_len);
    }

    if (Size > 10) {
        char key[32] = {0};
        char val[32] = {0};
        size_t key_len = (Size - 10) / 2;
        size_t val_len = (Size - 10) - key_len;
        if (key_len > 31) key_len = 31;
        if (val_len > 31) val_len = 31;
        
        memcpy(key, Data + 4, key_len);
        memcpy(val, Data + 4 + key_len, val_len);
        
        key[key_len] = '\0';
        val[val_len] = '\0';

        aom_codec_set_option(&ctx, key, val);
        aom_codec_set_option(&ctx, key, NULL);
    }

    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    if (Size > 12) {
        uint32_t pts_val, dur_val, flg_val;
        memcpy(&pts_val, Data + 4, sizeof(uint32_t));
        memcpy(&dur_val, Data + 8, sizeof(uint32_t));
        memcpy(&flg_val, Data + 12, sizeof(uint32_t));
        
        pts = (aom_codec_pts_t)pts_val;
        duration = (unsigned long)dur_val;
        flags = (aom_enc_frame_flags_t)flg_val;
    }

    if (duration == 0) duration = 1;

    aom_codec_encode(&ctx, &img, pts, duration, flags);

    while (1) {
        aom_codec_cx_pkt_t pkt;
        const aom_codec_cx_pkt_t *pkt_ptr = aom_codec_get_cx_data(&ctx, &pkt);
        if (!pkt_ptr) break;
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Consume the frame data if needed, but for fuzzing we just loop
        }
    }

    free_image(&img);
    aom_codec_destroy(&ctx);

    return 0;
}