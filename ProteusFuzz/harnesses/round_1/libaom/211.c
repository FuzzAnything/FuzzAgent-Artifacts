// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER at aomcx.h:1902:1 in aomcx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER at aomdx.h:519:1 in aomdx.h
// aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64 at aomcx.h:1905:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_QUANTIZER_ONE_PASS at aomcx.h:2327:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_CQ_LEVEL at aomcx.h:1917:1 in aomcx.h
// aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX at aomdx.h:592:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required headers */
#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_frame_buffer.h"

/* 
 * Helper to initialize an encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int width = 16;
    int height = 16;

    if (data && size >= 4) {
        width = (data[0] % 32) + 16;
        height = (data[1] % 32) + 16;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/* 
 * Helper to initialize a decoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_decoder_context(aom_codec_ctx_t *ctx) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_dx();
    if (!iface) return -1;

    /* Initialize struct to defaults */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    /* Removed invalid members: allow_highbitdepth, allow_multiresolution, allow_screencontenttools */
    /* Using valid member if available, otherwise just threads */
    cfg.allow_lowbitdepth = 1;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/* 
 * Fuzzing entry point
 */
int LLVMFuzzerTestOneInput_211(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx_enc;
    aom_codec_ctx_t ctx_dec;
    aom_codec_err_t res;
    int val_int;
    uint64_t val_uint64;
    int quantizer_val;

    if (Size == 0) return 0;

    /* Initialize Encoder Context */
    memset(&ctx_enc, 0, sizeof(ctx_enc));
    if (init_encoder_context(&ctx_enc, Data, Size) != 0) {
        /* If init fails, try with minimal data or skip encoder tests */
        goto test_decoder;
    }

    /* 1. aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER */
    /* Signature: aom_codec_err_t(aom_codec_ctx_t *ctx, int ctrl, int *data) */
    res = aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER(&ctx_enc, AOME_GET_LAST_QUANTIZER, &val_int);
    (void)res;

    /* 2. aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER */
    /* This is a decoder function, so we need a decoder context */
    memset(&ctx_dec, 0, sizeof(ctx_dec));
    if (init_decoder_context(&ctx_dec) == 0) {
        /* Signature: aom_codec_err_t(aom_codec_ctx_t *ctx, int ctrl, int *data) */
        res = aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER(&ctx_dec, AOMD_GET_LAST_QUANTIZER, &val_int);
        (void)res;
    }

    /* 3. aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64 */
    /* Signature: aom_codec_err_t(aom_codec_ctx_t *ctx, int ctrl, int *data) 
     * Note: The macro AOM_CTRL_USE_TYPE uses 'int *' even for 64 bit in some defs, 
     * but let's pass the pointer. The cast happens inside the macro.
     */
    res = aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64(&ctx_enc, AOME_GET_LAST_QUANTIZER_64, (int *)&val_uint64);
    (void)res;

    /* 4. aom_codec_control_typechecked_AV1E_SET_QUANTIZER_ONE_PASS */
    /* Signature: aom_codec_err_t(aom_codec_ctx_t *ctx, int ctrl, int data) */
    if (Size > 0) {
        quantizer_val = Data[0] % 64; 
        res = aom_codec_control_typechecked_AV1E_SET_QUANTIZER_ONE_PASS(&ctx_enc, AV1E_SET_QUANTIZER_ONE_PASS, quantizer_val);
        (void)res;
    }

    /* 5. aom_codec_control_typechecked_AOME_SET_CQ_LEVEL */
    /* Signature: aom_codec_err_t(aom_codec_ctx_t *ctx, int ctrl, int data) */
    if (Size > 1) {
        quantizer_val = Data[1] % 64;
        res = aom_codec_control_typechecked_AOME_SET_CQ_LEVEL(&ctx_enc, AOME_SET_CQ_LEVEL, quantizer_val);
        (void)res;
    }

    /* 6. aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX */
    /* Signature: aom_codec_err_t(aom_codec_ctx_t *ctx, int ctrl, int *data) */
    if (init_decoder_context(&ctx_dec) == 0) {
        res = aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX(&ctx_dec, AOMD_GET_BASE_Q_IDX, &val_int);
        (void)res;
    }

    /* Cleanup Encoder */
    aom_codec_destroy(&ctx_enc);

test_decoder:
    /* Cleanup Decoder if initialized */
    if (ctx_dec.iface) {
        aom_codec_destroy(&ctx_dec);
    }

    return 0;
}