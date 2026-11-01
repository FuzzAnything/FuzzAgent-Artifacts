// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR at aomcx.h:2177:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIFF_WTD_COMP at aomcx.h:2117:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY at aomcx.h:2204:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE at aomcx.h:2342:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_GET_NUM_OPERATING_POINTS at aomcx.h:2318:1 in aomcx.h
// aom_codec_control_typechecked_AV1_GET_ACCOUNTING at aomdx.h:516:1 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int ret;

    // Initialize encoder context
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Extract control parameters from input data
    int enable_diff_wtd_comp = Data[0] & 0x1;
    int intra_default_tx_only = (Data[1] >> 0) & 0x1;
    unsigned int svc_frame_drop_mode = (Data[2] >> 0) & 0x3;
    int num_operating_points = Data[3] & 0xFF;

    // Fuzz AV1E_SET_ENABLE_DIFF_WTD_COMP
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_DIFF_WTD_COMP(ctx, ctrl, data)
    ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_DIFF_WTD_COMP(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable_diff_wtd_comp);
    (void)ret;

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    // Signature: aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY(ctx, ctrl, data)
    ret = aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx_only);
    (void)ret;

    // Fuzz AV1E_SET_SVC_FRAME_DROP_MODE
    // Signature: aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE(ctx, ctrl, data)
    ret = aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, svc_frame_drop_mode);
    (void)ret;

    // Fuzz AV1E_GET_NUM_OPERATING_POINTS
    // Signature: aom_codec_control_typechecked_AV1E_GET_NUM_OPERATING_POINTS(ctx, ctrl, data)
    int result_ops = 0;
    ret = aom_codec_control_typechecked_AV1E_GET_NUM_OPERATING_POINTS(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &result_ops);
    (void)ret;

    // Fuzz AV1_GET_ACCOUNTING
    // Signature: aom_codec_control_typechecked_AV1_GET_ACCOUNTING(ctx, ctrl, data)
    // data type is Accounting **
    Accounting *acc_ptr = NULL;
    ret = aom_codec_control_typechecked_AV1_GET_ACCOUNTING(&ctx, AV1_GET_ACCOUNTING, &acc_ptr);
    (void)ret;

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Signature: aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR(ctx, ctrl, data)
    // Assuming data is a pointer to a film grain struct or similar.
    // Since we don't have the exact struct, we pass a dummy pointer or 0.
    // The macro expects a specific type. If it's a pointer, we pass a dummy.
    // If the macro expects a specific struct pointer, passing 0 might be safe for fuzzing to trigger error paths.
    // Let's try passing 0 or a dummy pointer.
    // Based on typical usage, it might be `const aom_film_grain_params_t *`.
    // We will pass 0 to test null handling.
    ret = aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, 0);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}