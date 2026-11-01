// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t **ctx) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!*ctx) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return -1;
    }

    ret = aom_codec_enc_init(*ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    int ret;
    int val;
    aom_svc_ref_frame_comp_pred_t svc_pred;

    if (Size < 1) return 0;

    // Initialize encoder
    if (init_encoder(&ctx) != 0) return 0;

    // Use input data to drive fuzzing decisions
    val = Data[0] % 2; // 0 or 1

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DUAL_FILTER
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, val);
    (void)ret;

    // Fuzz AV1E_SET_INTER_DCT_ONLY
    ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
    (void)ret;

    // Fuzz AV1E_SET_REDUCED_TX_TYPE_SET
    ret = aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    (void)ret;

    // Fuzz AV1E_SET_SVC_REF_FRAME_COMP_PRED
    if (Size >= 4) {
        svc_pred.use_comp_pred[0] = (Data[1] % 2);
        svc_pred.use_comp_pred[1] = (Data[2] % 2);
        svc_pred.use_comp_pred[2] = (Data[3] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &svc_pred);
        (void)ret;
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}