// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * - AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
 * - AV1E_SET_SVC_REF_FRAME_COMP_PRED
 * - AV1E_SET_FILM_GRAIN_TEST_VECTOR
 * - AV1E_SET_INTRA_DEFAULT_TX_ONLY
 * - AV1E_SET_TUNE_CONTENT
 */

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL) return 0;

    int idx = 0;
    int ret = 0;

    /* 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH: expects int (0 or 1) */
    if (idx < (int)size) {
        int val = data[idx++] ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &val);
        (void)ret;
    }

    /* 2. AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE: expects int (0 or 1) */
    if (idx < (int)size) {
        int val = data[idx++] ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, &val);
        (void)ret;
    }

    /* 3. AV1E_SET_SVC_REF_FRAME_COMP_PRED: expects aom_svc_ref_frame_comp_pred_t */
    if (idx + sizeof(aom_svc_ref_frame_comp_pred_t) <= (int)size) {
        aom_svc_ref_frame_comp_pred_t svc_pred;
        memcpy(&svc_pred, &data[idx], sizeof(svc_pred));
        idx += sizeof(svc_pred);
        ret = aom_codec_control(ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &svc_pred);
        (void)ret;
    }

    /* 4. AV1E_SET_FILM_GRAIN_TEST_VECTOR: expects int (0 or 1) */
    if (idx < (int)size) {
        int val = data[idx++] ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, &val);
        (void)ret;
    }

    /* 5. AV1E_SET_INTRA_DEFAULT_TX_ONLY: expects int (0 or 1) */
    if (idx < (int)size) {
        int val = data[idx++] ? 1 : 0;
        ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, &val);
        (void)ret;
    }

    /* 6. AV1E_SET_TUNE_CONTENT: expects aom_tune_content (enum) */
    if (idx < (int)size) {
        int val = data[idx++];
        aom_tune_content tune = (aom_tune_content)val;
        ret = aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, &tune);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_341(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Set minimal valid config */
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Run fuzz logic */
    fuzz_encoder_control(&ctx, Data, Size);

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}