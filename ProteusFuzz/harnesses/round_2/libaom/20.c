// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_BITRATE_ONE_PASS_CBR at aomcx.h:2339:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ERROR_RESILIENT_MODE at aomcx.h:1957:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_INTER_DCT_ONLY at aomcx.h:2201:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_CPUUSED at aomcx.h:1890:1 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_ALLOW_REF_FRAME_MVS at aomcx.h:2096:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

/*
 * Helper function to safely initialize an encoder context.
 * Returns 1 on success, 0 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    res = aom_codec_enc_config_default(iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Set some minimal valid configuration to ensure init succeeds
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

/*
 * Helper function to safely cleanup an encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
        ctx->priv = NULL;
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    aom_codec_err_t res;

    // Initialize encoder context
    if (!init_encoder_context(&ctx, &cfg)) {
        return 0;
    }

    // Extract control ID and value from input data
    // Using first 4 bytes as control ID (cast to int)
    int ctrl_id = 0;
    memcpy(&ctrl_id, Data, sizeof(int));

    // Use next 4 bytes as value if available
    int value = 0;
    if (Size >= 8) {
        memcpy(&value, Data + 4, sizeof(int));
    }

    // Map raw ctrl_id to known control IDs for type-checked macros
    // We will try multiple known control IDs based on the input value
    // to maximize coverage of different code paths

    // Try AV1E_SET_ALLOW_REF_FRAME_MVS if value is within reasonable range
    if (value >= 0 && value <= 1) {
        res = AOM_CODEC_CONTROL_TYPECHECKED(&ctx, AV1E_SET_ALLOW_REF_FRAME_MVS, value);
        if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
            // Unexpected error, but continue fuzzing
        }
    }

    // Try AV1E_SET_BITRATE_ONE_PASS_CBR
    if (value >= 0 && value <= 1) {
        res = AOM_CODEC_CONTROL_TYPECHECKED(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, value);
        if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
            // Unexpected error, but continue fuzzing
        }
    }

    // Try AV1E_SET_ERROR_RESILIENT_MODE
    if (value >= 0 && value <= 2) { // Assuming 0-2 are valid modes
        res = AOM_CODEC_CONTROL_TYPECHECKED(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, value);
        if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
            // Unexpected error, but continue fuzzing
        }
    }

    // Try AV1E_SET_INTER_DCT_ONLY
    if (value >= 0 && value <= 1) {
        res = AOM_CODEC_CONTROL_TYPECHECKED(&ctx, AV1E_SET_INTER_DCT_ONLY, value);
        if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
            // Unexpected error, but continue fuzzing
        }
    }

    // Try AOME_SET_CPUUSED
    if (value >= -8 && value <= 8) { // Typical CPU usage range
        res = AOM_CODEC_CONTROL_TYPECHECKED(&ctx, AOME_SET_CPUUSED, value);
        if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
            // Unexpected error, but continue fuzzing
        }
    }

    // Also try generic aom_codec_control with the raw ctrl_id from input
    // This tests the dispatch mechanism and error handling for invalid IDs
    res = aom_codec_control(&ctx, ctrl_id, value);
    // We don't check res here as we expect errors for invalid IDs

    // Cleanup
    cleanup_encoder_context(&ctx);

    return 0;
}