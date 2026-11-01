// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targeted Control IDs:
 * - AV1E_SET_REDUCED_REFERENCE_SET
 * - AV1E_SET_ENABLE_INTRA_EDGE_FILTER
 * - AV1E_SET_COLOR_PRIMARIES
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 * - AV1E_SET_ALLOW_WARPED_MOTION
 * - AV1E_SET_ENABLE_SUPERRES
 */

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (!ctx || !cfg) return 0;

    // Get the AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal valid config for fuzzing
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 100;

    // Initialize codec context
    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    // Invoke the control function
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_194(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    // Initialize encoder
    if (!initialize_encoder(&ctx, &cfg)) {
        return 0;
    }

    // Extract values from input data to drive fuzzing
    int val1 = (int)(Data[0] & 0xFF);
    int val2 = (int)(Data[1] & 0xFF);
    int val3 = (int)(Data[Size > 2 ? 2 : 1] & 0xFF);
    int val4 = (int)(Data[Size > 3 ? 3 : 1] & 0xFF);
    int val5 = (int)(Data[Size > 4 ? 4 : 1] & 0xFF);
    int val6 = (int)(Data[Size > 5 ? 5 : 1] & 0xFF);

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    fuzz_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val1);

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    fuzz_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val2);

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    fuzz_control(&ctx, AV1E_SET_COLOR_PRIMARIES, val3);

    // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
    fuzz_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val4);

    // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
    fuzz_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val5);

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    fuzz_control(&ctx, AV1E_SET_ENABLE_SUPERRES, val6);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}