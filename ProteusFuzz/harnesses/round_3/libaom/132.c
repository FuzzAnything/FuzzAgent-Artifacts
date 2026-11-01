// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    // Get the AV1 encoder interface directly
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Set basic config to ensure valid state
    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    if (Size < 2) return 0;

    // Initialize encoder
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use input data to drive control parameters
    // We use the first byte to select which control to test
    uint8_t control_selector = Data[0] % 6;
    
    // Use remaining bytes to construct arguments
    int32_t arg_int = 0;
    if (Size > 4) {
        arg_int = (int32_t)(Data[1] | (Data[2] << 8) | (Data[3] << 16) | (Data[4] << 24));
    }

    // AV1 Encoder Control IDs (Standard libaom definitions)
    // These correspond to the typechecked macros mentioned in the task
    #define AOM_CODEC_AV1E_SET_ENABLE_QM 0x10000001
    #define AOM_CODEC_AV1E_SET_MV_COST_UPD_FREQ 0x10000002
    #define AOM_CODEC_AV1E_SET_QM_V 0x10000003
    #define AOM_CODEC_AV1_SET_TILE_MODE 0x10000004
    #define AOM_CODEC_AV1E_SET_REDUCED_TX_TYPE_SET 0x10000005
    #define AOM_CODEC_AV1E_SET_TILE_COLUMNS 0x10000006

    switch (control_selector) {
        case 0:
            // AV1E_SET_ENABLE_QM: expects int32_t (0 or 1)
            ret = aom_codec_control(&ctx, AOM_CODEC_AV1E_SET_ENABLE_QM, arg_int & 1);
            break;
        case 1:
            // AV1E_SET_MV_COST_UPD_FREQ: expects int32_t
            ret = aom_codec_control(&ctx, AOM_CODEC_AV1E_SET_MV_COST_UPD_FREQ, arg_int);
            break;
        case 2:
            // AV1E_SET_QM_V: expects int32_t
            ret = aom_codec_control(&ctx, AOM_CODEC_AV1E_SET_QM_V, arg_int);
            break;
        case 3:
            // AV1_SET_TILE_MODE: expects int32_t
            ret = aom_codec_control(&ctx, AOM_CODEC_AV1_SET_TILE_MODE, arg_int);
            break;
        case 4:
            // AV1E_SET_REDUCED_TX_TYPE_SET: expects int32_t
            ret = aom_codec_control(&ctx, AOM_CODEC_AV1E_SET_REDUCED_TX_TYPE_SET, arg_int);
            break;
        case 5:
            // AV1E_SET_TILE_COLUMNS: expects int32_t
            // Ensure value is within reasonable bounds for columns (1 to 64)
            int32_t cols = (arg_int % 64) + 1;
            ret = aom_codec_control(&ctx, AOM_CODEC_AV1E_SET_TILE_COLUMNS, cols);
            break;
        default:
            ret = AOM_CODEC_OK;
            break;
    }

    cleanup_encoder(&ctx);
    return 0;
}