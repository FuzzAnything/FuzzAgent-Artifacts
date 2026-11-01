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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

/*
 * Helper function to initialize the AV1 encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (ctx == NULL || cfg == NULL) {
        return -1;
    }

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/*
 * Helper function to clean up the encoder context.
 */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use input data to determine which control to test and what value to pass
    int control_type = Data[0] % 6; // 0 to 5 for the 6 target functions
    int value = (Data[1] % 2); // 0 or 1 for boolean-like settings

    // Map control_type to actual AV1 encoder control constants
    int result = AOM_CODEC_OK;
    int arg = value;

    switch (control_type) {
        case 0:
            // AV1E_SET_ENABLE_DIAGONAL_INTRA
            result = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, arg);
            break;
        case 1:
            // AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
            result = aom_codec_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, arg);
            break;
        case 2:
            // AV1E_SET_ENABLE_AB_PARTITIONS
            result = aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, arg);
            break;
        case 3:
            // AV1E_SET_MIN_PARTITION_SIZE
            if (Size > 2) {
                arg = Data[2] + 1; // Avoid 0 if 0 is invalid, min 1
                if (arg > 128) arg = 128; // Cap to reasonable value
            }
            result = aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, arg);
            break;
        case 4:
            // AV1E_SET_ENABLE_1TO4_PARTITIONS
            result = aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, arg);
            break;
        case 5:
            // AV1E_SET_ENABLE_OVERLAY
            result = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, arg);
            break;
        default:
            break;
    }

    // Check for errors, using AOM_CODEC_ERROR as the generic error indicator
    if (result != AOM_CODEC_OK && result != AOM_CODEC_ERROR) {
        // Some controls might return specific errors, which is fine for fuzzing.
        // We continue to cleanup.
    }

    cleanup_encoder(&ctx);

    return 0;
}