// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

// Helper to safely destroy codec context
static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

// Helper to initialize encoder context for testing
static aom_codec_ctx_t *init_encoder_context(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize encoder context
    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) return 0;

    // Use input data to determine which control to test and what parameters to use
    uint8_t cmd_selector = Data[0] % 6;
    int32_t param_int = 0;
    int32_t param_int2 = 0;
    aom_rc_funcs_t rc_funcs = {0};

    // Extract parameters from input if available
    if (Size > 1) param_int = (int32_t)(Data[1] * 256 + (Size > 2 ? Data[2] : 0));
    if (Size > 3) param_int2 = (int32_t)(Data[3] * 256 + (Size > 4 ? Data[4] : 0));

    // Ensure parameters are within reasonable bounds for fuzzing
    param_int = (param_int % 100) + 1; // 1 to 100
    param_int2 = (param_int2 % 100) + 1; // 1 to 100

    aom_codec_err_t err = AOM_CODEC_OK;

    switch (cmd_selector) {
        case 0:
            // AV1E_SET_EXTERNAL_RATE_CONTROL
            rc_funcs.rc_type = (aom_rc_type_t)(param_int % 7);
            rc_funcs.priv = (void *)(size_t)param_int2;
            // Set dummy callbacks to avoid null pointer issues if the codec checks them
            rc_funcs.create_model = NULL;
            rc_funcs.send_firstpass_stats = NULL;
            rc_funcs.send_tpl_gop_stats = NULL;
            rc_funcs.get_encodeframe_decision = NULL;
            rc_funcs.update_encodeframe_result = NULL;
            rc_funcs.get_key_frame_decision = NULL;
            rc_funcs.get_gop_decision = NULL;
            rc_funcs.get_frame_rdmult = NULL;
            rc_funcs.delete_model = NULL;
            err = aom_codec_control(ctx, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);
            break;

        case 1:
            // AOME_SET_MAX_INTRA_BITRATE_PCT
            err = aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, param_int);
            break;

        case 2:
            // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, param_int % 2);
            break;

        case 3:
            // AV1E_SET_ENABLE_DIST_WTD_COMP
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, param_int % 2);
            break;

        case 4:
            // AV1E_SET_GF_CBR_BOOST_PCT
            err = aom_codec_control(ctx, AV1E_SET_GF_CBR_BOOST_PCT, param_int);
            break;

        case 5:
            // AV1E_SET_QUANT_B_ADAPT
            err = aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, param_int % 2);
            break;

        default:
            break;
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}