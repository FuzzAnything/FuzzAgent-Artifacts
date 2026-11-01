// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

// Helper to safely call control functions using varargs
static int try_control_int(aom_codec_ctx_t *ctx, int ctrl_id, int val) {
    if (!ctx || !ctx->priv) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, ctrl_id, val);
    return (res == AOM_CODEC_OK);
}

static int try_control_struct(aom_codec_ctx_t *ctx, int ctrl_id, void *arg) {
    if (!ctx || !ctx->priv) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, ctrl_id, arg);
    return (res == AOM_CODEC_OK);
}

// Helper to initialize encoder
static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int width = 64;
    int height = 64;

    // Fuzz width/height from input if size is sufficient
    if (size >= 4) {
        width = (data[0] % 256) + 8;
        height = (data[1] % 256) + 8;
        // Ensure reasonable bounds
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to cleanup
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_281(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize encoder context
    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Extract parameters from fuzz input
    int val1 = (Size > 0) ? (Data[0] % 2) : 0;
    int val2 = (Size > 1) ? (Data[1] % 2) : 0;
    int val3 = (Size > 2) ? (Data[2] % 2) : 0;
    int val4 = (Size > 3) ? (Data[3] % 2) : 0;
    int val5 = (Size > 4) ? (Data[4] % 2) : 0;

    // 1. AV1E_SET_FP_MT_UNIT_TEST
    // Assuming it takes an int flag.
    try_control_int(ctx, AV1E_SET_FP_MT_UNIT_TEST, val1);

    // 2. AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST
    // Assuming it takes an int flag.
    try_control_int(ctx, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, val2);

    // 3. AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    // Assuming it takes an int flag.
    try_control_int(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val3);

    // 4. AV1E_ENABLE_SB_QP_SWEEP
    // Assuming it takes an int flag.
    try_control_int(ctx, AV1E_ENABLE_SB_QP_SWEEP, val4);

    // 5. AV1E_SET_FP_MT
    // Assuming it takes an int flag.
    try_control_int(ctx, AV1E_SET_FP_MT, val5);

    // 6. AV1E_SET_SVC_REF_FRAME_COMP_PRED
    // This one has a defined struct in the prompt: aom_svc_ref_frame_comp_pred_t
    aom_svc_ref_frame_comp_pred_t svc_pred;
    svc_pred.use_comp_pred[0] = (Size > 5) ? (Data[5] % 2) : 0;
    svc_pred.use_comp_pred[1] = (Size > 6) ? (Data[6] % 2) : 0;
    svc_pred.use_comp_pred[2] = (Size > 7) ? (Data[7] % 2) : 0;
    
    try_control_struct(ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &svc_pred);

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}