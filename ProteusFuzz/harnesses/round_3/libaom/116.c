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
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Set some basic config to ensure valid initialization
    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    // Initialize encoder
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use input data to drive control parameters
    int val = (int)(Data[0] % 2); // 0 or 1 for boolean flags
    int val_int = (int)(Data[0] % 100); // Small integer for other params

    // We will invoke aom_codec_control with various AV1E_* constants.
    // Note: The specific "typechecked" functions mentioned in the prompt are likely
    // internal macros or not directly callable in this manner. We use the standard
    // aom_codec_control with the corresponding enum values.

    // 1. AV1E_SET_S_FRAME_MODE
    if (aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // 2. AV1E_SET_ENABLE_AB_PARTITIONS
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 3. AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    if (aom_codec_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 4. AV1E_SET_FRAME_PERIODIC_BOOST
    // This usually expects an integer, we use val_int
    if (aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val_int) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 5. AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC
    // This is a GET command, expects a pointer to store result
    int result_get = 0;
    if (aom_codec_control(&ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &result_get) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 6. AV1E_SET_RTC_EXTERNAL_RC
    if (aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}