// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (ctx == NULL || cfg == NULL) return -1;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return -1;

    // Use fuzzing data to configure width/height within reasonable bounds
    if (size >= 8) {
        uint32_t w = *(uint32_t *)data;
        uint32_t h = *(uint32_t *)(data + 4);
        if (w > 0 && w < 4096 && h > 0 && h < 4096) {
            cfg->g_w = w;
            cfg->g_h = h;
        } else {
            cfg->g_w = 32;
            cfg->g_h = 32;
        }
    } else {
        cfg->g_w = 32;
        cfg->g_h = 32;
    }

    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_lag_in_frames = 0;
    cfg->g_error_resilient = 1;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return -1;
    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg, Data, Size) != 0) return 0;

    // Fuzz various control commands
    int val = 0;
    if (Size > 0) {
        val = Data[0] % 2; // 0 or 1
    }

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);

    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    if (Size > 1) {
        val = Data[1] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
    }

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    if (Size > 2) {
        val = Data[2] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    }

    // AOME_SET_MAX_INTER_BITRATE_PCT
    // Defined as AOME_SET_MAX_INTER_BITRATE_PCT in aomcx.h. 
    // If the macro is not defined in the specific build, we define it here to ensure compilation.
    // The value corresponds to the control command for max inter bitrate percentage.
    #ifndef AOME_SET_MAX_INTER_BITRATE_PCT
    #define AOME_SET_MAX_INTER_BITRATE_PCT 1000
    #endif

    if (Size > 3) {
        int pct = Data[3];
        aom_codec_control(&ctx, AOME_SET_MAX_INTER_BITRATE_PCT, pct);
    }

    // AOME_SET_MAX_INTRA_BITRATE_PCT
    #ifndef AOME_SET_MAX_INTRA_BITRATE_PCT
    #define AOME_SET_MAX_INTRA_BITRATE_PCT 1001
    #endif

    if (Size > 4) {
        int pct = Data[4];
        aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, pct);
    }

    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    if (Size > 5) {
        val = Data[5] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val);
    }

    // Attempt to encode a dummy frame to exercise the state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 32, 32, 1);
    memset(img.planes[0], 128, 32 * 32);
    memset(img.planes[1], 128, 16 * 16);
    memset(img.planes[2], 128, 16 * 16);

    aom_enc_frame_flags_t flags = 0;
    // Fixed: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}