// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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

#include "aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

/*
 * Helper to initialize the encoder context.
 * Returns 1 on success, 0 on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    aom_codec_err_t res;

    if (!ctx || !data) return 0;

    // Use AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters slightly based on input data to explore states
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1] % 256) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 1000) + 1;
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    }

    // Initialize encoder
    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

/*
 * Helper to clean up the encoder context.
 */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_308(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int success = 0;

    // Guard against empty input
    if (!Data || Size == 0) {
        return 0;
    }

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder
    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    // Define Control IDs based on the function names provided.
    // These are the standard AV1 encoder control IDs corresponding to the 
    // internal typechecked functions mentioned in the prompt.
    
    // Map input bytes to control IDs and values
    int control_id = 0;
    int value = 0;
    
    // Cycle through the potential controls based on input
    int selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            // AV1E_ENABLE_SB_QP_SWEEP
            control_id = AV1E_ENABLE_SB_QP_SWEEP;
            value = Data[1] ? 1 : 0;
            break;
        case 1:
            // AV1E_SET_ENABLE_PALETTE
            control_id = AV1E_SET_ENABLE_PALETTE;
            value = Data[1] ? 1 : 0;
            break;
        case 2:
            // AV1E_SET_POSTENCODE_DROP_RTC
            control_id = AV1E_SET_POSTENCODE_DROP_RTC;
            value = Data[1] ? 1 : 0;
            break;
        case 3:
            // AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST
            control_id = AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST;
            value = Data[1] ? 1 : 0;
            break;
        case 4:
            // AV1E_SET_FILM_GRAIN_TEST_VECTOR
            control_id = AV1E_SET_FILM_GRAIN_TEST_VECTOR;
            value = Data[1] ? 1 : 0;
            break;
        case 5:
            // AV1E_SET_FP_MT_UNIT_TEST
            control_id = AV1E_SET_FP_MT_UNIT_TEST;
            value = Data[1] ? 1 : 0;
            break;
        default:
            break;
    }

    // Invoke the control
    aom_codec_err_t res = aom_codec_control(&ctx, control_id, value);
    
    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}