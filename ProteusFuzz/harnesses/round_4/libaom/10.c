// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <stdarg.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/* Helper to initialize encoder context */
static aom_codec_ctx_t *init_encoder(const aom_codec_iface_t *iface, int width, int height) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width > 0 ? width : 64;
    cfg.g_h = height > 0 ? height : 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_CBR;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

/* Helper to cleanup context */
static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Get AV1 encoder interface */
    const aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    /* Parse input to determine dimensions and control parameters */
    int width = (Size > 0) ? (Data[0] & 0x7F) + 1 : 64;
    int height = (Size > 1) ? (Data[1] & 0x7F) + 1 : 64;
    int val_int = (Size > 2) ? (int)Data[2] : 0;
    int val_int2 = (Size > 3) ? (int)Data[3] : 0;

    /* Ensure reasonable dimensions */
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width < 8) width = 8;
    if (height < 8) height = 8;

    /* Initialize Encoder Context */
    aom_codec_ctx_t *enc_ctx = init_encoder(enc_iface, width, height);
    if (!enc_ctx) return 0;

    /* Fuzz aom_codec_control with various IDs */
    aom_codec_err_t ret;

    /* 1. AV1E_SET_ENABLE_PALETTE */
    ret = aom_codec_control(enc_ctx, AV1E_SET_ENABLE_PALETTE, val_int);
    (void)ret; /* Ignore return for fuzzing stability */

    /* 2. AV1E_SET_REDUCED_REFERENCE_SET */
    ret = aom_codec_control(enc_ctx, AV1E_SET_REDUCED_REFERENCE_SET, val_int2);
    (void)ret;

    /* 3. AV1E_GET_SEQ_LEVEL_IDX */
    int level_idx = 0;
    ret = aom_codec_control(enc_ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx);
    (void)ret;

    /* 4. AV1E_SET_ENABLE_CFL_INTRA */
    ret = aom_codec_control(enc_ctx, AV1E_SET_ENABLE_CFL_INTRA, val_int);
    (void)ret;

    /* 6. AV1E_SET_ENABLE_PAETH_INTRA */
    ret = aom_codec_control(enc_ctx, AV1E_SET_ENABLE_PAETH_INTRA, val_int2);
    (void)ret;

    /* Test with invalid/null contexts to trigger error paths */
    ret = aom_codec_control(NULL, AV1E_SET_ENABLE_PALETTE, 1);
    (void)ret;

    ret = aom_codec_control(enc_ctx, 0, 1); /* Invalid ID */
    (void)ret;

    /* Cleanup */
    cleanup_codec(enc_ctx);

    return 0;
}