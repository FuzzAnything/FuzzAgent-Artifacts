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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

// Helper function to initialize the encoder context
static aom_codec_ctx_t* init_encoder_context(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Get the AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Set basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_undershoot_pct = 0;
    cfg.rc_overshoot_pct = 0;

    // Initialize the codec context
    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper function to cleanup the encoder context
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize encoder context
    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) return 0;

    // Extract a value from the input data to use as a parameter
    int value = Data[0] % 2; // Use 0 or 1 as the parameter value

    // Define the control commands to test
    int commands[] = {
        AV1E_SET_ENABLE_SMOOTH_INTRA,
        AV1E_SET_ENABLE_INTRA_EDGE_FILTER,
        AV1E_SET_ENABLE_OVERLAY,
        AV1E_SET_ENABLE_DUAL_FILTER,
        AV1E_SET_ENABLE_MASKED_COMP,
        AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    };
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    // Iterate over each command and invoke the control function
    for (int i = 0; i < num_commands; i++) {
        aom_codec_err_t err = aom_codec_control(ctx, commands[i], value);
        if (err != AOM_CODEC_OK) {
            // Handle error if necessary
            continue;
        }
    }

    // Cleanup
    cleanup_encoder_context(ctx);

    return 0;
}