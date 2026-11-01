// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

// Helper to initialize a dummy encoder context
static aom_codec_ctx_t* create_dummy_encoder(aom_codec_iface_t *iface) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to destroy encoder context
static void destroy_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Get AV1 encoder interface using the correct API from aomcx.h
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t *ctx = create_dummy_encoder(iface);
    if (!ctx) return 0;

    int ret = 0;

    // Interpret input data to drive different control functions
    // We use the first few bytes to select which API to fuzz and provide parameters
    uint32_t param_val = 0;
    if (Size >= 4) {
        param_val = *(uint32_t *)Data;
    } else if (Size >= 1) {
        param_val = Data[0];
    }

    // Normalize param_val to safe ranges for specific APIs to avoid immediate crashes
    // while still exploring state space.
    int safe_param = param_val % 100; // 0-99

    switch (Data[0] % 6) {
        case 0:
            // Fuzz AOME_SET_MAX_INTRA_BITRATE_PCT
            // Expected: int (0-100)
            ret = aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, safe_param);
            break;
        case 1:
            // Fuzz AV1E_SET_AQ_MODE
            // Expected: int (AQ mode enum)
            ret = aom_codec_control(ctx, AV1E_SET_AQ_MODE, safe_param % 5);
            break;
        case 2:
            // Fuzz AV1E_SET_DELTAQ_STRENGTH
            // Expected: int (0-32)
            ret = aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, safe_param % 33);
            break;
        case 3:
            // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
            // Expected: int (0 or 1)
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, safe_param % 2);
            break;
        case 4:
            // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
            // Expected: int (0 or 1)
            ret = aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, safe_param % 2);
            break;
        case 5:
            // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
            // Expected: int (0 or 1)
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, safe_param % 2);
            break;
    }

    // Cleanup
    destroy_encoder(ctx);

    return 0;
}