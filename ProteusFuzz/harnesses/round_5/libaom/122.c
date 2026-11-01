// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzz data where possible
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1 % Size] % 512) + 16;
    cfg.g_timebase.num = (Data[2 % Size] % 100) + 1;
    cfg.g_timebase.den = (Data[3 % Size] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4 % Size] % 10000) + 100;
    cfg.g_threads = (Data[5 % Size] % 8) + 1;
    cfg.g_error_resilient = (Data[6 % Size] % 2);
    cfg.g_lag_in_frames = (Data[7 % Size] % 10);
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame to ensure the encoder is in a valid state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], Data[8 % Size], cfg.g_w * cfg.g_h);
    memset(img.planes[1], Data[9 % Size], (cfg.g_w * cfg.g_h) / 4);
    memset(img.planes[2], Data[10 % Size], (cfg.g_w * cfg.g_h) / 4);

    aom_enc_frame_flags_t flags = 0;
    if (Size > 11) flags = Data[11 % Size] & 0xFF;

    if (aom_codec_encode(&ctx, &img, 0, 1, flags) != AOM_CODEC_OK) {
        aom_img_free(&img);
        cleanup_codec(&ctx);
        return 0;
    }

    aom_img_free(&img);

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    int enable_restoration = (Size > 12) ? (Data[12 % Size] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int enable_tx_size_search = (Size > 13) ? (Data[13 % Size] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    int mode_cost_upd_freq = (Size > 14) ? (Data[14 % Size] % 100) : 0;
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_upd_freq);

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode = (Size > 15) ? (Data[15 % Size] % 3) : 0;
    aom_codec_control(&ctx, AV1E_SET_AQ_MODE, aq_mode);

    // Fuzz AOME_SET_ARNR_STRENGTH
    int arnr_strength = (Size > 16) ? (Data[16 % Size] % 10) : 0;
    aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    int deltaq_strength = (Size > 17) ? (Data[17 % Size] % 10) : 0;
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);

    // Try to encode another frame to trigger potential issues
    aom_image_t img2;
    aom_img_alloc(&img2, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img2.planes[0], Data[18 % Size], cfg.g_w * cfg.g_h);
    memset(img2.planes[1], Data[19 % Size], (cfg.g_w * cfg.g_h) / 4);
    memset(img2.planes[2], Data[20 % Size], (cfg.g_w * cfg.g_h) / 4);

    aom_codec_encode(&ctx, &img2, 1, 1, flags);
    aom_img_free(&img2);

    cleanup_codec(&ctx);
    return 0;
}