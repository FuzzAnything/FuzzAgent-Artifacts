// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

// Helper to safely initialize a codec context for fuzzing
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal valid config
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

// Helper to destroy context
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    // Initialize encoder context
    if (!init_encoder_context(&ctx, &cfg)) {
        return 0;
    }

    // Use input data to drive control parameters
    int val_int = 0;
    int val_bool = 0;
    aom_svc_params_t svc_params = {0};

    if (Size >= sizeof(int)) {
        memcpy(&val_int, Data, sizeof(int));
    }
    if (Size >= sizeof(int) + 1) {
        val_bool = Data[sizeof(int)] % 2;
    }

    // 1. AV1E_SET_DENOISE_NOISE_LEVEL
    // Expected: int value
    aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val_int);

    // 2. AV1E_SET_NOISE_SENSITIVITY
    // Expected: int value
    aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val_int);

    // 3. AV1E_SET_ENABLE_INTRABC
    // Expected: int (0 or 1)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val_bool);

    // 4. AV1E_SET_SVC_PARAMS
    // Prepare a valid SVC params structure based on input
    svc_params.number_spatial_layers = 1 + (val_int % 3); // 1 to 3
    svc_params.number_temporal_layers = 1 + (val_int % 3); // 1 to 3
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.max_quantizers[i] = 0;
        svc_params.min_quantizers[i] = 0;
        svc_params.layer_target_bitrate[i] = 1000;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        svc_params.scaling_factor_num[i] = 1;
        svc_params.scaling_factor_den[i] = 1;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        svc_params.framerate_factor[i] = 1;
    }
    aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);

    // 5. AV1E_SET_ENABLE_DNL_DENOISING
    // Expected: int (0 or 1)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val_bool);

    // 6. AV1E_SET_SKIP_POSTPROC_FILTERING
    // Expected: int (0 or 1)
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val_bool);

    // Cleanup
    cleanup_encoder_context(&ctx);

    return 0;
}