// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/*
 * Helper function to initialize the encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    // Set minimal configuration to allow initialization
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->g_lag_in_frames = 1; 

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/*
 * Helper function to cleanup the encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int val_int;
    aom_svc_ref_frame_comp_pred_t svc_pred;

    // Guard against empty input
    if (Size == 0) {
        return 0;
    }

    // Initialize context
    if (init_encoder_context(&ctx, &cfg) != 0) {
        return 0;
    }

    // Derive values from fuzz input to diversify states
    val_int = (Size > 0) ? (Data[0] % 2) : 0;
    svc_pred.use_comp_pred[0] = (Size > 1) ? (Data[1] % 2) : 0;
    svc_pred.use_comp_pred[1] = (Size > 2) ? (Data[2] % 2) : 0;
    svc_pred.use_comp_pred[2] = (Size > 3) ? (Data[3] % 2) : 0;

    // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_ONESIDED_COMP
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val_int);
    (void)ret;

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val_int);
    (void)ret;

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, val_int);
    (void)ret;

    // 4. aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_COMP_PRED
    ret = aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &svc_pred);
    (void)ret;

    // 5. aom_codec_control_typechecked_AV1E_SET_ENABLE_REF_FRAME_MVS
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val_int);
    (void)ret;

    // 6. aom_codec_control_typechecked_AV1E_SET_FP_MT_UNIT_TEST
    ret = aom_codec_control(&ctx, AV1E_SET_FP_MT_UNIT_TEST, val_int);
    (void)ret;

    // Cleanup
    cleanup_encoder_context(&ctx);

    return 0;
}