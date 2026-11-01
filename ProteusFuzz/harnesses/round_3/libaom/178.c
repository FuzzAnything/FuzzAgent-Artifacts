// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static void init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    aom_codec_enc_config_default(iface, cfg, 0);
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_178(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    init_encoder_context(&ctx, &cfg);
    if (!ctx.iface) return 0;

    // Interpret input data to control behavior
    uint8_t cmd = Data[0] % 6;
    int val = 0;
    if (Size > 4) {
        val = (Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4];
    }

    // Fuzz specific control commands
    switch (cmd) {
        case 0:
            // AV1E_ENABLE_SB_QP_SWEEP
            if (aom_codec_control(&ctx, AV1E_ENABLE_SB_QP_SWEEP, val) != AOM_CODEC_OK) {
                // Ignore errors, continue fuzzing
            }
            break;
        case 1:
            // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
            if (aom_codec_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val) != AOM_CODEC_OK) {
                // Ignore errors
            }
            break;
        case 2:
            // AV1E_SET_ENABLE_PALETTE
            if (aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val) != AOM_CODEC_OK) {
                // Ignore errors
            }
            break;
        case 3:
            // AV1E_SET_SVC_FRAME_DROP_MODE
            if (aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val) != AOM_CODEC_OK) {
                // Ignore errors
            }
            break;
        case 4:
            // AV1E_SET_FILM_GRAIN_TEST_VECTOR
            // This might require a struct, but we pass val as int for fuzzing
            if (aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val) != AOM_CODEC_OK) {
                // Ignore errors
            }
            break;
        case 5:
            // AV1E_SET_FP_MT_UNIT_TEST
            if (aom_codec_control(&ctx, AV1E_SET_FP_MT_UNIT_TEST, val) != AOM_CODEC_OK) {
                // Ignore errors
            }
            break;
    }

    cleanup_encoder_context(&ctx);
    return 0;
}