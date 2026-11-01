// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_QM_MAX at aomcx.h:2042:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ at aomcx.h:2219:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SCREEN_CONTENT_DETECTION_MODE at aomcx.h:2363:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_S_FRAME_MODE at aomcx.h:1960:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC at aomcx.h:2351:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC at aomcx.h:2306:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

/*
 * The target functions are defined as static inline functions via macros in aomcx.h.
 * The macro AOM_CTRL_USE_TYPE expands to a function with signature:
 * static aom_codec_err_t aom_codec_control_typechecked_<ID>(aom_codec_ctx_t *ctx, int ctrl, typ data)
 * 
 * Therefore, we must pass the control ID (e.g., AV1E_SET_QM_MAX) as the second argument.
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return 0;
    }
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_266(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    /* Initialize encoder */
    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    /* Extract values from fuzz input to drive the API */
    unsigned int val1 = (unsigned int)Data[0];
    unsigned int val2 = Size > 1 ? (unsigned int)Data[1] : 0;
    int val3 = Size > 2 ? (int)Data[2] : 0;
    int val4 = Size > 3 ? (int)Data[3] : 0;
    int val6 = Size > 5 ? (int)Data[5] : 0;
    int get_val = 0;

    /* 1. aom_codec_control_typechecked_AV1E_SET_QM_MAX */
    /* Signature: (ctx, int ctrl, unsigned int data) */
    aom_codec_control_typechecked_AV1E_SET_QM_MAX(&ctx, AV1E_SET_QM_MAX, val1);

    /* 2. aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ */
    /* Signature: (ctx, int ctrl, unsigned int data) */
    aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val2);

    /* 3. aom_codec_control_typechecked_AV1E_SET_SCREEN_CONTENT_DETECTION_MODE */
    /* Signature: (ctx, int ctrl, int data) */
    aom_codec_control_typechecked_AV1E_SET_SCREEN_CONTENT_DETECTION_MODE(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, val3);

    /* 4. aom_codec_control_typechecked_AV1E_SET_S_FRAME_MODE */
    /* Signature: (ctx, int ctrl, int data) */
    aom_codec_control_typechecked_AV1E_SET_S_FRAME_MODE(&ctx, AV1E_SET_S_FRAME_MODE, val4);

    /* 5. aom_codec_control_typechecked_AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC */
    /* Signature: (ctx, int ctrl, int *data) */
    aom_codec_control_typechecked_AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC(&ctx, AV1E_GET_HIGH_MOTION_CONTENT_SCREEN_RTC, &get_val);

    /* 6. aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC */
    /* Signature: (ctx, int ctrl, int data) */
    aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val6);

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}