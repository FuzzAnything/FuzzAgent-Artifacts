// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_ROW_MT at aomcx.h:1939:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE at aomcx.h:2120:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ at aomcx.h:2219:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ at aomcx.h:2219:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_FP_MT at aomcx.h:2309:1 in aomcx.h
// aom_codec_control_typechecked_AV1D_SET_ROW_MT at aomdx.h:543:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER at aomcx.h:2075:1 in aomcx.h
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

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"

/*
 * Helper function to initialize a valid encoder context for testing.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_encoder_context(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Set minimal configuration for a valid context */
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to initialize a valid decoder context for testing.
 * Returns NULL on failure.
 */
static aom_codec_ctx_t *init_decoder_context(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Fuzz driver entry point
 */
int LLVMFuzzerTestOneInput_310(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Use the first byte to decide which function to fuzz */
    uint8_t choice = Data[0] % 6;
    
    /* We need a valid context for the control functions */
    aom_codec_ctx_t *ctx = NULL;
    int result = 0;

    /* Initialize context based on choice */
    if (choice == 4) {
        /* Decoder functions */
        ctx = init_decoder_context();
    } else {
        /* Encoder functions */
        ctx = init_encoder_context();
    }

    if (!ctx) {
        return 0;
    }

    /* Extract parameters from fuzz data if needed */
    int param_val = 0;
    if (Size > 1) {
        param_val = (int)(Data[1] % 2); /* 0 or 1 for boolean-like flags */
    }
    if (Size > 2) {
        int mt_val = (int)(Data[2] % 4); /* 0 to 3 for multi-threading values */
        if (choice == 0 || choice == 4) {
            param_val = mt_val;
        }
    }

    switch (choice) {
        case 0:
            /* aom_codec_control_typechecked_AV1E_SET_ROW_MT */
            /* Signature: aom_codec_control_typechecked_AV1E_SET_ROW_MT(ctx, AV1E_SET_ROW_MT, value) */
            result = aom_codec_control_typechecked_AV1E_SET_ROW_MT(ctx, AV1E_SET_ROW_MT, param_val);
            break;
        case 1:
            /* aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE */
            /* Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, value) */
            result = aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, param_val);
            break;
        case 2:
            /* aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ */
            if (Size > 3) {
                int freq = (int)(Data[3] % 100);
                /* Signature: aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, value) */
                result = aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, freq);
            } else {
                result = aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, 0);
            }
            break;
        case 3:
            /* aom_codec_control_typechecked_AV1E_SET_FP_MT */
            /* Signature: aom_codec_control_typechecked_AV1E_SET_FP_MT(ctx, AV1E_SET_FP_MT, value) */
            result = aom_codec_control_typechecked_AV1E_SET_FP_MT(ctx, AV1E_SET_FP_MT, param_val);
            break;
        case 4:
            /* aom_codec_control_typechecked_AV1D_SET_ROW_MT */
            /* Signature: aom_codec_control_typechecked_AV1D_SET_ROW_MT(ctx, AV1D_SET_ROW_MT, value) */
            result = aom_codec_control_typechecked_AV1D_SET_ROW_MT(ctx, AV1D_SET_ROW_MT, param_val);
            break;
        case 5:
            /* aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER */
            /* Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value) */
            result = aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, param_val);
            break;
        default:
            break;
    }

    /* Cleanup */
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }

    return 0;
}