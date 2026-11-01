// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include "aom_integer.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"
#include "aomdx.h"
#include "aomcx.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Use fuzzing data to configure width/height if possible, with bounds checking
    if (size >= 8) {
        int w = *(const int32_t *)(data);
        int h = *(const int32_t *)(data + 4);
        // Ensure reasonable bounds to avoid allocation failures or crashes
        if (w > 0 && w < 4096 && h > 0 && h < 4096) {
            cfg.g_w = w;
            cfg.g_h = h;
        } else {
            cfg.g_w = 64;
            cfg.g_h = 64;
        }
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Extract parameters from fuzzing data
    int cmd = 0;
    int value = 0;

    if (Size >= 4) {
        cmd = *(const int32_t *)(Data);
    }
    if (Size >= 8) {
        value = *(const int32_t *)(Data + 4);
    }

    // Clamp command ID to a reasonable range to avoid potential issues
    if (cmd < 0) cmd = 0;
    if (cmd > 10000) cmd = 10000;

    // Invoke the control function with fuzzed parameters
    // This attempts to exercise the internal dispatch logic of aom_codec_control
    aom_codec_control(&ctx, cmd, &value);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}