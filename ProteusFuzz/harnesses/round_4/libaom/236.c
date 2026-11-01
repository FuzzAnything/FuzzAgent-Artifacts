// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_236(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_err_t ret;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg = {0};
    int32_t val_int = 0;
    int64_t val_int64 = 0;
    int32_t val_idx = 0;

    // Initialize encoder config
    ret = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1 % Size] % 128) + 16;
    cfg.g_timebase.num = (Data[2 % Size] % 100) + 1;
    cfg.g_timebase.den = (Data[3 % Size] % 100) + 1;
    cfg.g_threads = (Data[4 % Size] % 4) + 1;
    cfg.g_error_resilient = (Data[5 % Size] % 3);
    cfg.rc_target_bitrate = (Data[6 % Size] * 1000) + 100;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_QUANTIZER_ONE_PASS
    int quantizer = (Data[7 % Size] % 64) + 1;
    ret = aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, quantizer);
    (void)ret;

    // Fuzz AV1E_SET_QUANT_B_ADAPT
    int quant_b_adapt = (Data[8 % Size] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_QUANT_B_ADAPT, quant_b_adapt);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_RECT_TX
    int enable_rect_tx = (Data[9 % Size] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);
    (void)ret;

    // Fuzz AOME_GET_LAST_QUANTIZER
    ret = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &val_int);
    (void)ret;

    // Fuzz AOME_GET_LAST_QUANTIZER_64
    ret = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &val_int64);
    (void)ret;

    cleanup_encoder(&ctx);

    // Initialize decoder
    iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    dec_cfg.threads = (Data[10 % Size] % 4) + 1;
    // Corrected field name from allow_high_bitdepth to allow_lowbitdepth
    dec_cfg.allow_lowbitdepth = (Data[11 % Size] % 2);
    // Removed non-existent fields allow_multiref and allow_screencontenttools

    ret = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AOMD_GET_BASE_Q_IDX
    ret = aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &val_idx);
    (void)ret;

    cleanup_decoder(&ctx);

    return 0;
}