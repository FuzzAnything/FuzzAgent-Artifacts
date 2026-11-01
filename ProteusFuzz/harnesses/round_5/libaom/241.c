// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void cleanup_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_241(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int success = 0;

    // Parse input for configuration
    uint32_t width = (Data[0] % 16) + 16;
    uint32_t height = (Data[1] % 16) + 16;
    uint32_t threads = (Data[2] % 4) + 1;
    uint32_t bit_depth = (Data[3] % 3) + 1; // 0->8, 1->10, 2->12
    aom_bit_depth_t bd = (bit_depth == 0) ? AOM_BITS_8 : (bit_depth == 1 ? AOM_BITS_10 : AOM_BITS_12);
    
    // Initialize default config
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = threads;
    cfg.g_bit_depth = bd;
    cfg.g_input_bit_depth = bd;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (Data[4] * 100);
    cfg.g_limit = 0; // Unlimited
    cfg.rc_end_usage = AOM_CQ;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize encoder
    if (!init_encoder_context(&ctx, &cfg)) {
        return 0;
    }
    success = 1;

    // Fuzz aom_codec_set_option
    if (Size > 5) {
        char key[64], val[64];
        size_t key_len = (Data[5] % 10) + 1;
        size_t val_len = (Data[6] % 10) + 1;
        if (key_len < sizeof(key) && val_len < sizeof(val)) {
            memcpy(key, Data + 7, key_len);
            key[key_len] = '\0';
            memcpy(val, Data + 7 + key_len, val_len);
            val[val_len] = '\0';
            aom_codec_set_option(&ctx, key, val);
        }
    }

    // Fuzz aom_codec_enc_config_set
    if (Size > 8) {
        aom_codec_enc_cfg_t new_cfg = cfg;
        new_cfg.g_w = (Data[8] % 100) + 10;
        new_cfg.g_h = (Data[9] % 100) + 10;
        aom_codec_enc_config_set(&ctx, &new_cfg);
    }

    // Fuzz aom_codec_set_cx_data_buf
    if (Size > 10) {
        size_t buf_size = (Data[10] * Data[11]) + 1024;
        uint8_t *custom_buf = malloc(buf_size);
        if (custom_buf) {
            memset(custom_buf, 0, buf_size);
            aom_fixed_buf_t fixed_buf;
            fixed_buf.buf = custom_buf;
            fixed_buf.sz = buf_size;
            unsigned int pad_before = Data[12] % 100;
            unsigned int pad_after = Data[13] % 100;
            aom_codec_set_cx_data_buf(&ctx, &fixed_buf, pad_before, pad_after);
            free(custom_buf);
        }
        // Test with NULL
        aom_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
    }

    // Create dummy image for encoding
    aom_image_t img = {0};
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (bd == AOM_BITS_10 || bd == AOM_BITS_12) {
        fmt = AOM_IMG_FMT_I42016;
    }

    if (aom_img_alloc(&img, fmt, cfg.g_w, cfg.g_h, 1) != 0) {
        cleanup_context(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    size_t plane_size = img.planes[0][0] * img.stride[0];
    if (Size > 14) {
        size_t data_avail = Size - 14;
        size_t copy_len = (data_avail < plane_size) ? data_avail : plane_size;
        memcpy(img.planes[0], Data + 14, copy_len);
        // Fill rest with pattern
        for (size_t i = copy_len; i < plane_size; i++) {
            img.planes[0][i] = (Data[14] ^ (i & 0xFF));
        }
        // Fill chroma planes
        if (img.planes[1]) {
            size_t uv_size = img.planes[1][0] * img.stride[1];
            for (size_t i = 0; i < uv_size; i++) img.planes[1][i] = 128;
        }
        if (img.planes[2]) {
            size_t uv_size = img.planes[2][0] * img.stride[2];
            for (size_t i = 0; i < uv_size; i++) img.planes[2][i] = 128;
        }
    }

    // Fuzz aom_codec_encode
    aom_codec_pts_t pts = 0;
    unsigned long duration = (Data[15] % 100) + 1;
    aom_enc_frame_flags_t flags = (Data[16] % 4);

    // Encode a few frames
    for (int i = 0; i < 3; i++) {
        aom_codec_encode(&ctx, &img, pts, duration, flags);
        pts += duration;
        
        // Drain packets
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Free if needed, though usually managed by codec
                // Just consuming the packet
            }
        }
    }

    // Flush encoder
    while (1) {
        aom_codec_encode(&ctx, NULL, 0, 0, 0);
        const aom_codec_cx_pkt_t *pkt;
        int has_data = 0;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            has_data = 1;
        }
        if (!has_data) break;
    }

    // Fuzz aom_codec_error_detail
    const char *err_detail = aom_codec_error_detail(&ctx);
    (void)err_detail; // Use to suppress warning

    // Cleanup
    aom_img_free(&img);
    cleanup_context(&ctx);

    return 0;
}