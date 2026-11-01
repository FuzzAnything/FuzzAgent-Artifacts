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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * - AV1E_SET_ERROR_RESILIENT_MODE
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_NOISE_SENSITIVITY
 * - AOME_SET_TUNING
 * - AV1E_SET_ENABLE_DNL_DENOISING
 * - AV1E_SET_ENABLE_OVERLAY
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    // Get the AV1 encoder interface using the correct API
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Set a minimal valid configuration to ensure initialization succeeds
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_212(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    // Initialize the encoder
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Extract values from fuzz input to drive the control functions
    int val1 = (int)Data[0];
    int val2 = (int)Data[1 % Size];
    int val3 = (int)Data[2 % Size];
    int val4 = (int)Data[3 % Size];
    int val5 = (int)Data[4 % Size];
    int val6 = (int)Data[5 % Size];

    // 1. AV1E_SET_ERROR_RESILIENT_MODE
    aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, val1);

    // 2. AV1E_SET_S_FRAME_MODE
    aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val2);

    // 3. AV1E_SET_NOISE_SENSITIVITY
    aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val3);

    // 4. AOME_SET_TUNING
    aom_codec_control(&ctx, AOME_SET_TUNING, val4);

    // 5. AV1E_SET_ENABLE_DNL_DENOISING
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val5);

    // 6. AV1E_SET_ENABLE_OVERLAY
    aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val6);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}