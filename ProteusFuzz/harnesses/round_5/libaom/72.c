// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

/*
 * Fuzz driver for libaom AV1 encoder control commands.
 * Target APIs:
 * - AV1E_SET_ENABLE_PALETTE
 * - AV1E_SET_MIN_PARTITION_SIZE
 * - AV1E_SET_ENABLE_RECT_PARTITIONS
 * - AV1E_SET_ENABLE_AB_PARTITIONS
 * - AV1E_SET_ENABLE_MASKED_COMP
 * - AV1E_SET_ENABLE_INTERINTER_WEDGE
 *
 * Note: The specific type-checked wrapper functions (e.g., aom_codec_control_typechecked_...)
 * are internal or generated macros. The standard API to invoke these controls is
 * aom_codec_control() with the corresponding AV1E_* enum constants.
 * This driver uses aom_codec_control() to exercise these specific flags.
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    // Initialize configuration with defaults
    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Set some basic parameters to ensure valid initialization
    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 30;
    cfg->g_timebase.den = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    err = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int val;

    // Ensure minimum data size to avoid trivial crashes
    if (Size < 1) {
        return 0;
    }

    // Initialize encoder context
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use input data to determine which control to test and what value to set
    int cmd_index = Data[0] % 6;
    int set_value = 0;

    // Extract a value for the command if needed
    if (Size > 1) {
        set_value = Data[1];
    }

    // Map index to specific AV1 encoder control commands
    // We use aom_codec_control directly as the type-checked wrappers are internal
    // and the standard API is the primary interface for these settings.
    switch (cmd_index) {
        case 0:
            // AV1E_SET_ENABLE_PALETTE: Enable/Disable palette mode (0 or 1)
            val = set_value ? 1 : 0;
            err = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val);
            break;

        case 1:
            // AV1E_SET_MIN_PARTITION_SIZE: Set minimum partition size (e.g., 4, 8, 16, 32, 64)
            // Valid values are typically powers of 2. We constrain fuzzing to reasonable range.
            if (set_value < 4) set_value = 4;
            if (set_value > 64) set_value = 64;
            // Ensure it's a power of 2 for valid testing, or let the codec handle invalid input
            err = aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, set_value);
            break;

        case 2:
            // AV1E_SET_ENABLE_RECT_PARTITIONS: Enable/Disable rectangular partitions (0 or 1)
            val = set_value ? 1 : 0;
            err = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
            break;

        case 3:
            // AV1E_SET_ENABLE_AB_PARTITIONS: Enable/Disable adaptive block partitions (0 or 1)
            val = set_value ? 1 : 0;
            err = aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);
            break;

        case 4:
            // AV1E_SET_ENABLE_MASKED_COMP: Enable/Disable masked compound (0 or 1)
            val = set_value ? 1 : 0;
            err = aom_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, val);
            break;

        case 5:
            // AV1E_SET_ENABLE_INTERINTER_WEDGE: Enable/Disable inter-inter wedge (0 or 1)
            val = set_value ? 1 : 0;
            err = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
            break;

        default:
            err = AOM_CODEC_OK;
            break;
    }

    // We don't strictly check err here to allow the fuzzer to explore error paths
    // as well as success paths. The library handles invalid inputs gracefully usually.

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}