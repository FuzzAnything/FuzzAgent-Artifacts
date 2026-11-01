// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

/*
 * The target functions are specific control commands for the AV1 encoder.
 * Since the exact signatures were not provided in the prompt, we use the
 * standard aom_codec_control macro which is the intended usage pattern
 * for these type-checked control functions in libaom.
 *
 * We will fuzz the encoder configuration and control calls.
 */

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data) return 0;

    int value;
    int ret;

    // Helper to extract an int from fuzz data safely
    #define GET_INT(idx) ((idx + 4 <= size) ? (int)(data[idx] | (data[idx+1] << 8) | (data[idx+2] << 16) | (data[idx+3] << 24)) : 0)

    size_t idx = 0;

    // 1. AV1E_ENABLE_EXT_TILE_DEBUG
    if (idx + 4 <= size) {
        value = GET_INT(idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, value);
        if (ret != AOM_CODEC_OK) {
            // Continue fuzzing even on error
        }
    }

    // 2. AV1E_SET_ENABLE_RECT_PARTITIONS
    if (idx + 4 <= size) {
        value = GET_INT(idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, value);
    }

    // 3. AV1E_SET_ENABLE_PALETTE
    if (idx + 4 <= size) {
        value = GET_INT(idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, value);
    }

    // 4. AV1_SET_TILE_MODE (This is usually a decoder control, but we try it)
    // Note: AV1_SET_TILE_MODE is typically for decoding. Applying to encoder might fail gracefully.
    if (idx + 4 <= size) {
        value = GET_INT(idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1_SET_TILE_MODE, value);
    }

    // 5. AV1E_SET_SINGLE_TILE_DECODING
    if (idx + 4 <= size) {
        value = GET_INT(idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, value);
    }

    // 6. AV1E_SET_TILE_COLUMNS
    if (idx + 4 <= size) {
        value = GET_INT(idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, value);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int idx = 0;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder config
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz some config parameters based on input
    cfg.g_w = (Data[idx] * 2) + 16; idx++;
    cfg.g_h = (Data[idx] * 2) + 16; idx++;
    cfg.g_timebase.num = (Data[idx] % 10) + 1; idx++;
    cfg.g_timebase.den = (Data[idx] % 10) + 1; idx++;
    cfg.g_threads = (Data[idx] % 8) + 1; idx++;
    cfg.rc_target_bitrate = (Data[idx] * 100) + 100; idx++;
    cfg.g_pass = 0; // Single pass
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    // Initialize the codec
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Perform the control fuzzing
    fuzz_encoder_controls(&ctx, Data + idx, Size - idx);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}