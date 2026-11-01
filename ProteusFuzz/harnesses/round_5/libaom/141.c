// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static int try_fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    
    // Attempt to set the control with the fuzzed value
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    
    // We don't return error as success/failure for the fuzzer, 
    // we just want to trigger the code path.
    (void)err;
    return 1;
}

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // 1. Prepare the environment
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    // Initialize configuration
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some basic config values to ensure valid initialization
    cfg.g_w = 128;
    cfg.g_h = 128;
    // Fixed member name from g_time_base to g_timebase
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Initialize the encoder context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Explore more program states by invoking the target function diversely
    // We map the first byte of input to select which control to fuzz
    // and the second byte (or a derived value) to the value to set.
    
    int control_selector = Data[0] % 6;
    int control_id = 0;
    int value_to_set = (int)(Data[1] & 0xFF);

    // Map selector to the specific control IDs.
    switch (control_selector) {
        case 0: // AV1E_SET_MIN_PARTITION_SIZE
            control_id = AV1E_SET_MIN_PARTITION_SIZE;
            break;
        case 1: // AV1E_SET_ENABLE_OVERLAY
            control_id = AV1E_SET_ENABLE_OVERLAY;
            break;
        case 2: // AV1E_SET_ENABLE_ANGLE_DELTA
            control_id = AV1E_SET_ENABLE_ANGLE_DELTA;
            break;
        case 3: // AV1E_SET_ENABLE_1TO4_PARTITIONS
            control_id = AV1E_SET_ENABLE_1TO4_PARTITIONS;
            break;
        case 4: // AV1E_SET_ENABLE_DIST_WTD_COMP
            control_id = AV1E_SET_ENABLE_DIST_WTD_COMP;
            break;
        case 5: // AV1E_SET_ENABLE_DNL_DENOISING
            control_id = AV1E_SET_ENABLE_DNL_DENOISING;
            break;
        default:
            control_id = 0;
            break;
    }

    if (control_id != 0) {
        try_fuzz_codec_control(&ctx, control_id, value_to_set);
    }

    // Additional diversity: Try with different values derived from input
    if (Size > 4) {
        int val2 = (Data[2] << 8) | Data[3];
        try_fuzz_codec_control(&ctx, control_id, val2);
    }

    // 2. Conduct cleanup
    aom_codec_destroy(&ctx);

    return 0;
}