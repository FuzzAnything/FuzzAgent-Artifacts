// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t encoder_ctx = {0};
    aom_codec_ctx_t decoder_ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_iface_t *enc_iface;
    aom_codec_iface_t *dec_iface;
    int ret;
    int value;

    // Initialize Encoder
    enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    ret = aom_codec_enc_init(&encoder_ctx, enc_iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize Decoder
    dec_iface = aom_codec_av1_dx();
    if (!dec_iface) {
        cleanup_encoder(&encoder_ctx);
        return 0;
    }

    dec_cfg.threads = 1;
    ret = aom_codec_dec_init(&decoder_ctx, dec_iface, &dec_cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&encoder_ctx);
        return 0;
    }

    // Fuzz Encoder Controls
    // AV1E_SET_ENABLE_RECT_TX
    value = Data[0] % 2;
    aom_codec_control(&encoder_ctx, AV1E_SET_ENABLE_RECT_TX, value);

    // AV1E_SET_TUNE_CONTENT
    value = Data[1] % 3; 
    aom_codec_control(&encoder_ctx, AV1E_SET_TUNE_CONTENT, value);

    // AV1E_SET_ENABLE_INTERINTRA_WEDGE
    value = Data[2] % 2;
    aom_codec_control(&encoder_ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value);

    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    value = Data[3] % 2;
    aom_codec_control(&encoder_ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, value);

    // AV1E_SET_ENABLE_DUAL_FILTER
    value = Data[0] % 2;
    aom_codec_control(&encoder_ctx, AV1E_SET_ENABLE_DUAL_FILTER, value);

    // Fuzz Decoder Controls
    // AOMD_GET_ORDER_HINT
    int order_hint = 0;
    aom_codec_control(&decoder_ctx, AOMD_GET_ORDER_HINT, &order_hint);

    // Cleanup
    cleanup_encoder(&encoder_ctx);
    cleanup_decoder(&decoder_ctx);

    return 0;
}