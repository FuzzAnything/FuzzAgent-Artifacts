// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Helper function to initialize an encoder context.
 * Returns a valid context or NULL on failure.
 */
static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    if (size == 0) return NULL;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzzing data to configure width/height within safe bounds */
    cfg.g_w = (data[0] % 64) + 16;
    cfg.g_h = (data[1] % 64) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to initialize a decoder context.
 * Returns a valid context or NULL on failure.
 */
static aom_codec_ctx_t *init_decoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t res;

    if (size == 0) return NULL;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    /* Initialize config manually. Only use fields present in the struct. */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    /* The following fields were removed/renamed in newer versions or are not present:
     * allow_highbitdepth, allow_multiref, allow_screencontenttools, allow_intrabc, allow_superres.
     * We only set 'allow_lowbitdepth' if it exists, or leave defaults.
     */
    cfg.allow_lowbitdepth = 1;

    res = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;
    int value;

    if (Size < 2) return 0;

    /* Determine whether to test encoder or decoder based on input */
    if (Data[0] & 0x01) {
        ctx = init_encoder_context(Data, Size);
    } else {
        ctx = init_decoder_context(Data, Size);
    }

    if (!ctx) return 0;

    /* Extract a value from fuzzing input for control parameters */
    value = (int)(Data[1] & 0xFF);

    /*
     * The target functions provided in the prompt are internal/type-checked wrappers
     * that are not directly exposed in the public API with the exact signatures provided.
     * However, the standard public API `aom_codec_control` is used to set these parameters.
     * We map the requested parameters to their standard enum constants.
     */

    /* 1. AOME_SET_CQ_LEVEL */
    res = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, value);
    (void)res;

    /* 2. AOME_SET_ENABLEAUTOALTREF */
    res = aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, value);
    (void)res;

    /* 3. AV1E_SET_COEFF_COST_UPD_FREQ */
    res = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, value);
    (void)res;

    /* 4. AV1E_SET_ENABLE_TPL_MODEL */
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, value);
    (void)res;

    /* 5. AOME_SET_ENABLEAUTOBWDREF */
    res = aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, value);
    (void)res;

    /* 6. AV1E_SET_QM_MAX */
    res = aom_codec_control(ctx, AV1E_SET_QM_MAX, value);
    (void)res;

    /* Cleanup */
    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}