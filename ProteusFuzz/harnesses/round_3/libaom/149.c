// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <limits.h>

#include "aom.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

/* Helper to initialize encoder context */
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    /* Use aom_codec_av1_cx() as the correct interface function for AV1 */
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to tweak config slightly, but keep it valid
    cfg.g_w = (data[0] % 512) + 16;
    cfg.g_h = (data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 60) + 1;
    cfg.rc_target_bitrate = (data[3] % 10000) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to initialize decoder context */
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = (data[0] % 8) + 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Fuzzing entry point
 */
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /*
     * Strategy:
     * 1. Initialize an encoder and a decoder.
     * 2. Attempt to call the target control functions on both contexts.
     * 3. Handle errors gracefully.
     * 4. Clean up resources.
     */

    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);

    if (!enc_ctx && !dec_ctx) {
        return 0;
    }

    /*
     * We define the control IDs here. These correspond to the standard
     * libaom control IDs.
     */
    int get_loopfilter = AOME_GET_LOOPFILTER_LEVEL;
    int set_loopfilter_ctrl = AV1E_SET_LOOPFILTER_CONTROL;
    int set_enable_intrabc = AV1E_SET_ENABLE_INTRABC;
    int set_skip_postproc = AV1E_SET_SKIP_POSTPROC_FILTERING;
    int set_skip_loop_filter = AV1_SET_SKIP_LOOP_FILTER;
    int set_svc_params = AV1E_SET_SVC_PARAMS;

    /*
     * Prepare arguments for the control calls based on fuzzing data
     */
    int loopfilter_val = (Data[0] % 64);
    int intrabc_val = (Data[1] % 2);
    int skip_postproc_val = (Data[2] % 2);
    int skip_loop_val = (Data[3] % 2);

    /* Prepare SVC params */
    aom_svc_params_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = (Data[4] % 3) + 1;
    svc_params.number_temporal_layers = (Data[5] % 3) + 1;
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.max_quantizers[i] = (Data[6 + i] % 64) + 1;
        svc_params.min_quantizers[i] = (Data[7 + i] % 64) + 1;
        svc_params.layer_target_bitrate[i] = (Data[8 + i] % 1000) + 100;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        svc_params.framerate_factor[i] = (Data[9 + i] % 10) + 1;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        svc_params.scaling_factor_num[i] = (Data[10 + i] % 100) + 1;
        svc_params.scaling_factor_den[i] = (Data[11 + i] % 100) + 1;
    }

    /*
     * Attempt calls on Encoder Context
     */
    if (enc_ctx) {
        // 1. AOME_GET_LOOPFILTER_LEVEL
        int val;
        aom_codec_control(enc_ctx, get_loopfilter, &val);

        // 2. AV1E_SET_LOOPFILTER_CONTROL
        // Note: This usually requires a specific struct, but for fuzzing we try passing int pointer
        // or a dummy if the API allows. If it crashes, sanitizer will catch it.
        aom_codec_control(enc_ctx, set_loopfilter_ctrl, &loopfilter_val); 

        // 3. AV1E_SET_ENABLE_INTRABC
        aom_codec_control(enc_ctx, set_enable_intrabc, &intrabc_val);

        // 4. AV1E_SET_SKIP_POSTPROC_FILTERING
        aom_codec_control(enc_ctx, set_skip_postproc, &skip_postproc_val);

        // 5. AV1_SET_SKIP_LOOP_FILTER (Usually decoder side, but try on encoder)
        aom_codec_control(enc_ctx, set_skip_loop_filter, &skip_loop_val);

        // 6. AV1E_SET_SVC_PARAMS
        aom_codec_control(enc_ctx, set_svc_params, &svc_params);
    }

    /*
     * Attempt calls on Decoder Context
     */
    if (dec_ctx) {
        // 5. AV1_SET_SKIP_LOOP_FILTER (Decoder specific)
        aom_codec_control(dec_ctx, set_skip_loop_filter, &skip_loop_val);
        
        // 1. AOME_GET_LOOPFILTER_LEVEL (Decoder might support)
        int val;
        aom_codec_control(dec_ctx, get_loopfilter, &val);
    }

    /* Cleanup */
    if (enc_ctx) {
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
    }
    if (dec_ctx) {
        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
    }

    return 0;
}