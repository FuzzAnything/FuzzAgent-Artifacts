// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AOME_SET_ENABLEAUTOBWDREF at aomcx.h:2014:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ at aomcx.h:2219:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_MV_COST_UPD_FREQ at aomcx.h:2225:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC at aomcx.h:2306:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COLOR_PRIMARIES at aomcx.h:1978:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TIER_MASK at aomcx.h:2228:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

// Helper to safely initialize encoder context
static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int ret;

    // Use aom_codec_av1_cx() to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    // Initialize default config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Fuzz some config values based on input data
    if (size > 0) cfg.g_w = (data[0] % 1024) + 16; // Width
    if (size > 1) cfg.g_h = (data[1] % 1024) + 16; // Height
    if (size > 2) cfg.g_timebase.num = data[2] + 1;
    if (size > 3) cfg.g_timebase.den = data[3] + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 100) + 100; // Bitrate
    if (size > 5) cfg.g_error_resilient = data[5] % 2;

    // Initialize the codec context
    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int ctrl_val;
    int tier_val;
    int mv_cost_val;
    int coeff_cost_val;
    int rtc_val;
    int color_val;
    int auto_bwd_val;

    // Ensure minimum size to avoid out-of-bounds
    if (Size < 1) return 0;

    // Initialize encoder context
    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz control values based on input data
    if (Size > 0) ctrl_val = Data[0];
    if (Size > 1) tier_val = Data[1];
    if (Size > 2) mv_cost_val = Data[2];
    if (Size > 3) coeff_cost_val = Data[3];
    if (Size > 4) rtc_val = Data[4];
    if (Size > 5) color_val = Data[5];
    if (Size > 6) auto_bwd_val = Data[6];

    // Invoke target API functions with fuzzed values
    // Note: These functions require 3 arguments: ctx, ctrl_id, data

    // 1. AV1E_SET_MV_COST_UPD_FREQ
    ret = aom_codec_control_typechecked_AV1E_SET_MV_COST_UPD_FREQ(&ctx, AV1E_SET_MV_COST_UPD_FREQ, mv_cost_val);
    if (ret != AOM_CODEC_OK) {
        // Error handling: ignore or log
    }

    // 2. AV1E_SET_RTC_EXTERNAL_RC
    ret = aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_val);
    if (ret != AOM_CODEC_OK) {
        // Error handling
    }

    // 3. AV1E_SET_COLOR_PRIMARIES
    ret = aom_codec_control_typechecked_AV1E_SET_COLOR_PRIMARIES(&ctx, AV1E_SET_COLOR_PRIMARIES, color_val);
    if (ret != AOM_CODEC_OK) {
        // Error handling
    }

    // 4. AV1E_SET_TIER_MASK
    ret = aom_codec_control_typechecked_AV1E_SET_TIER_MASK(&ctx, AV1E_SET_TIER_MASK, tier_val);
    if (ret != AOM_CODEC_OK) {
        // Error handling
    }

    // 5. AOME_SET_ENABLEAUTOBWDREF
    ret = aom_codec_control_typechecked_AOME_SET_ENABLEAUTOBWDREF(&ctx, AOME_SET_ENABLEAUTOBWDREF, auto_bwd_val);
    if (ret != AOM_CODEC_OK) {
        // Error handling
    }

    // 6. AV1E_SET_COEFF_COST_UPD_FREQ
    ret = aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_val);
    if (ret != AOM_CODEC_OK) {
        // Error handling
    }

    // Cleanup: destroy the codec context
    aom_codec_destroy(&ctx);

    return 0;
}