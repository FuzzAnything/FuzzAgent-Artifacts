// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

/* Helper function to initialize a decoder context for fuzzing */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;

    if (data == NULL || size == 0) {
        return NULL;
    }

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Initialize config manually */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.w = 1920;
    cfg.h = 1080;
    cfg.allow_lowbitdepth = 1;

    err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper function to initialize an encoder context for fuzzing */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    if (data == NULL || size == 0) {
        return NULL;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Initialize config manually to avoid dependency on AOM_AV1 macro */
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 1920;
    cfg.g_h = 1080;
    cfg.rc_target_bitrate = 1000;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    cfg.g_pass = 0;
    cfg.g_usage = AOM_USAGE_REALTIME;
    cfg.rc_end_usage = AOM_CBR;
    cfg.g_threads = 0;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize decoder context */
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    
    if (dec_ctx) {
        int32_t ret;
        int32_t val;
        aom_tile_info tile_info;
        int32_t quantizer;
        int32_t ref_updates[32];
        int32_t altref_present;
        int32_t last_ref_used[32];

        /* 1. aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES */
        memset(ref_updates, 0, sizeof(ref_updates));
        ret = aom_codec_control(dec_ctx, AOMD_GET_LAST_REF_UPDATES, ref_updates);
        (void)ret;

        /* 2. aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT */
        ret = aom_codec_control(dec_ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
        (void)ret;

        /* 3. aom_codec_control_typechecked_AOMD_GET_LAST_REF_USED */
        memset(last_ref_used, 0, sizeof(last_ref_used));
        ret = aom_codec_control(dec_ctx, AOMD_GET_LAST_REF_USED, last_ref_used);
        (void)ret;

        /* 4. aom_codec_control_typechecked_AOMD_GET_TILE_INFO */
        memset(&tile_info, 0, sizeof(tile_info));
        ret = aom_codec_control(dec_ctx, AOMD_GET_TILE_INFO, &tile_info);
        (void)ret;

        /* 6. aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER */
        ret = aom_codec_control(dec_ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
        (void)ret;

        /* Cleanup decoder */
        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
    }

    /* Initialize encoder context */
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);

    if (enc_ctx) {
        int32_t ret;
        int32_t enable_filter_intra = 1; /* Default to 1, could be 0 */

        /* 5. aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA */
        ret = aom_codec_control(enc_ctx, AV1E_SET_ENABLE_FILTER_INTRA, &enable_filter_intra);
        (void)ret;

        /* Try with 0 as well to explore state */
        enable_filter_intra = 0;
        ret = aom_codec_control(enc_ctx, AV1E_SET_ENABLE_FILTER_INTRA, &enable_filter_intra);
        (void)ret;

        /* Cleanup encoder */
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
    }

    return 0;
}