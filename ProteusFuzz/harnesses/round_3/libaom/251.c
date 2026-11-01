// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

// Helper to initialize a dummy encoder context
static aom_codec_ctx_t *init_dummy_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    ctx->iface = iface;
    ctx->name = "dummy_av1";
    ctx->err = AOM_CODEC_OK;
    ctx->err_detail = "initialized";
    ctx->init_flags = 0;

    // Allocate and setup encoder config
    aom_codec_enc_cfg_t *cfg = (aom_codec_enc_cfg_t *)malloc(sizeof(aom_codec_enc_cfg_t));
    if (!cfg) {
        free(ctx);
        return NULL;
    }
    memset(cfg, 0, sizeof(aom_codec_enc_cfg_t));
    
    if (aom_codec_enc_config_default(ctx->iface, cfg, 0) != AOM_CODEC_OK) {
        free(cfg);
        free(ctx);
        return NULL;
    }
    
    cfg->g_w = 16;
    cfg->g_h = 16;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    
    ctx->config.enc = cfg;

    // Do not manually allocate or access aom_codec_priv_t as it is opaque/incomplete in public headers.
    ctx->priv = NULL;

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (!ctx) return;
    if (ctx->config.enc) {
        free((void *)ctx->config.enc);
    }
    free(ctx);
}

int LLVMFuzzerTestOneInput_251(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize dummy encoder context
    aom_codec_ctx_t *ctx = init_dummy_encoder(Data, Size);
    if (!ctx) return 0;

    // Fuzzing logic for the specified control functions
    // We use aom_codec_control which handles the interface internally.
    
    int val_int;
    
    // 1. AOME_GET_LOOPFILTER_LEVEL
    #ifdef AOME_GET_LOOPFILTER_LEVEL
        aom_codec_control(ctx, AOME_GET_LOOPFILTER_LEVEL, &val_int);
    #endif

    // 2. AV1E_SET_LOOPFILTER_CONTROL
    #ifdef AV1E_SET_LOOPFILTER_CONTROL
        int lf_control = Data[0];
        aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, &lf_control);
    #endif

    // 3. AV1E_SET_ENABLE_INTRABC
    #ifdef AV1E_SET_ENABLE_INTRABC
        int enable_intrabc = Data[0] & 1;
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, &enable_intrabc);
    #endif

    // 4. AV1E_SET_SVC_PARAMS
    #ifdef AV1E_SET_SVC_PARAMS
        aom_svc_params_t svc_params;
        memset(&svc_params, 0, sizeof(svc_params));
        if (Size > 2) {
            svc_params.number_spatial_layers = (Data[0] % 3) + 1; // Range 1-3
            svc_params.number_temporal_layers = (Data[1] % 3) + 1; // Range 1-3
        } else {
            svc_params.number_spatial_layers = 1;
            svc_params.number_temporal_layers = 1;
        }
        aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    #endif

    // 5. AV1_SET_SKIP_LOOP_FILTER
    #ifdef AV1_SET_SKIP_LOOP_FILTER
        int skip_loop_filter = Data[0] & 1;
        aom_codec_control(ctx, AV1_SET_SKIP_LOOP_FILTER, &skip_loop_filter);
    #endif

    // 6. AV1E_SET_SKIP_POSTPROC_FILTERING
    #ifdef AV1E_SET_SKIP_POSTPROC_FILTERING
        int skip_postproc = Data[0] & 1;
        aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, &skip_postproc);
    #endif

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}