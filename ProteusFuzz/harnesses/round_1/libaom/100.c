// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/*
 * Fuzz driver for libaom encoder control functions.
 * Target APIs:
 * - AOME_SET_ARNR_STRENGTH
 * - AV1E_SET_DISABLE_TRELLIS_QUANT
 * - AV1E_SET_DENOISE_NOISE_LEVEL
 * - AV1E_SET_ENABLE_GLOBAL_MOTION
 * - AOME_SET_SHARPNESS
 * - AOME_SET_ARNR_MAXFRAMES
 */

static void init_codec_ctx(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    // Get the encoder interface first to ensure it exists and is valid
    iface = aom_codec_av1_cx();
    if (!iface) {
        return;
    }

    // Initialize configuration with defaults
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return;
    }

    // Fuzz the configuration slightly based on input data to explore states
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (data[1] % 1024) + 16;
        
        // Use g_timebase (struct aom_rational) for timing
        if (size > 3) {
            cfg.g_timebase.num = (data[2] % 30) + 1;
            cfg.g_timebase.den = (data[3] % 30) + 1;
        }
        
        cfg.rc_target_bitrate = (data[4] % 100) * 1000 + 1000;
        cfg.g_threads = (data[5] % 4) + 1;
        cfg.g_error_resilient = (data[6] % 3);
    }

    // Initialize the codec context
    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return;
    }
}

static void cleanup_codec_ctx(aom_codec_ctx_t *ctx) {
    if (ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val;

    // Ensure we have enough data to initialize
    if (Size < 7) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    // Initialize the codec context
    init_codec_ctx(&ctx, Data, Size);

    // If initialization failed, nothing to fuzz
    if (!ctx.iface) {
        return 0;
    }

    // Extract values from input data for fuzzing parameters
    // We use modulo to keep values within reasonable bounds for the API
    if (Size > 0) val = Data[0] % 100;
    else val = 0;

    // 1. AOME_SET_ARNR_STRENGTH
    // Expected: int value (strength level)
    if (Size > 0) val = Data[0] % 100;
    ret = aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, val);
    (void)ret;

    // 2. AV1E_SET_DISABLE_TRELLIS_QUANT
    // Expected: int (0 or 1)
    if (Size > 1) val = Data[1] % 2;
    else val = 0;
    ret = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val);
    (void)ret;

    // 3. AV1E_SET_DENOISE_NOISE_LEVEL
    // Expected: int (noise level)
    if (Size > 2) val = Data[2] % 256;
    else val = 0;
    ret = aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val);
    (void)ret;

    // 4. AV1E_SET_ENABLE_GLOBAL_MOTION
    // Expected: int (0 or 1)
    if (Size > 3) val = Data[3] % 2;
    else val = 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    (void)ret;

    // 5. AOME_SET_SHARPNESS
    // Expected: int (sharpness level)
    if (Size > 4) val = Data[4] % 100;
    else val = 0;
    ret = aom_codec_control(&ctx, AOME_SET_SHARPNESS, val);
    (void)ret;

    // 6. AOME_SET_ARNR_MAXFRAMES
    // Expected: int (max frames)
    if (Size > 5) val = Data[5] % 100;
    else val = 0;
    ret = aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, val);
    (void)ret;

    // Cleanup
    cleanup_codec_ctx(&ctx);

    return 0;
}