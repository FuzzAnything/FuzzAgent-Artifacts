// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * This driver initializes an AV1 encoder context and invokes control operations
 * corresponding to the target APIs using aom_codec_control.
 */

int LLVMFuzzerTestOneInput_280(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    aom_codec_iface_t *iface;
    int ret;
    int value;

    // Minimum data check to avoid trivial inputs
    if (Size < 2) {
        return 0;
    }

    // Initialize the encoder configuration
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzed data to configure basic encoder settings
    cfg.g_w = (Data[0] % 128) + 16; // Width between 16 and 143
    cfg.g_h = (Data[1] % 128) + 16; // Height between 16 and 143
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize the codec context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Determine which control to test based on input data
    int selected_idx = Data[0] % 6;
    
    // Prepare a value for the control based on fuzzed data
    int fuzz_val = (int)Data[1];
    
    switch (selected_idx) {
        case 0: // AV1E_SET_ENABLE_QM
            ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, fuzz_val);
            break;
        case 1: // AV1E_SET_NOISE_SENSITIVITY
            ret = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, fuzz_val);
            break;
        case 2: // AV1E_SET_AQ_MODE
            ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, fuzz_val);
            break;
        case 3: // AOME_SET_STATIC_THRESHOLD
            ret = aom_codec_control(&ctx, AOME_SET_STATIC_THRESHOLD, fuzz_val);
            break;
        case 4: // AV1E_SET_DELTAQ_STRENGTH
            ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, fuzz_val);
            break;
        case 5: // AV1E_ENABLE_RATE_GUIDE_DELTAQ
            ret = aom_codec_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, fuzz_val);
            break;
        default:
            ret = 0;
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}