// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void setup_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return;

    err = aom_codec_enc_config_default(iface, cfg, 0);
    if (err != AOM_CODEC_OK) return;

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_profile = 0;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        memset(ctx, 0, sizeof(aom_codec_ctx_t));
    }
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
        memset(ctx, 0, sizeof(aom_codec_ctx_t));
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    aom_svc_params_t svc_params = {0};
    int val;
    aom_codec_err_t err;

    if (Size < 4) return 0;

    // Use input data to seed configuration variations
    int seed = *(const int *)Data;
    int val1 = (seed & 0xFF) ? 1 : 0;
    int val2 = ((seed >> 8) & 0xFF) ? 1 : 0;
    int val3 = ((seed >> 16) & 0xFF) ? 1 : 0;
    int val4 = ((seed >> 24) & 0xFF) ? 1 : 0;

    setup_encoder_context(&ctx, &cfg);
    if (!ctx.iface) return 0;

    // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
    err = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val1);
    (void)err;

    // 2. aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING
    err = aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val2);
    (void)err;

    // 3. aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL
    // Assuming standard int control for fuzzing unless specific struct is required by macro.
    err = aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, val3);
    (void)err;

    // 4. aom_codec_control_typechecked_AV1_SET_SKIP_LOOP_FILTER
    // This is often a decoder control, but we try on encoder context to test error handling
    err = aom_codec_control(&ctx, AV1_SET_SKIP_LOOP_FILTER, val4);
    (void)err;

    // 5. aom_codec_control_typechecked_AOME_GET_LOOPFILTER_LEVEL
    // This is a GET operation, returns value via pointer
    int loopfilter_level = 0;
    err = aom_codec_control(&ctx, AOME_GET_LOOPFILTER_LEVEL, &loopfilter_level);
    (void)err;

    // 6. aom_codec_control_typechecked_AV1E_SET_SVC_PARAMS
    // Initialize SVC params with fuzzed data
    svc_params.number_spatial_layers = (Size > 4) ? (Data[3] % 3) + 1 : 1;
    svc_params.number_temporal_layers = (Size > 5) ? (Data[4] % 3) + 1 : 1;
    
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.max_quantizers[i] = (Size > 6 + i) ? Data[6 + i] : 32;
        svc_params.min_quantizers[i] = (Size > 16 + i) ? Data[16 + i] : 0;
        svc_params.layer_target_bitrate[i] = (Size > 26 + i) ? Data[26 + i] * 10 : 100;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        svc_params.scaling_factor_num[i] = (Size > 36 + i) ? Data[36 + i] : 1;
        svc_params.scaling_factor_den[i] = (Size > 46 + i) ? Data[46 + i] : 1;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        svc_params.framerate_factor[i] = (Size > 56 + i) ? Data[56 + i] : 1;
    }

    err = aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    (void)err;

    cleanup_encoder_context(&ctx);

    return 0;
}