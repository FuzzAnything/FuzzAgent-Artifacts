// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_SUPERRES
 * - AV1E_SET_ENABLE_QM
 * - AV1E_SET_COLOR_PRIMARIES
 * - AV1E_SET_ALLOW_WARPED_MOTION
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 * - AV1E_GET_SEQ_LEVEL_IDX
 */

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

static int get_val(const uint8_t *data, size_t size, size_t offset, int max_val) {
    if (offset >= size) return 0;
    return data[offset] % (max_val + 1);
}

int LLVMFuzzerTestOneInput_223(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;
    int idx;

    // 1. Prepare the environment
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration with defaults
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to vary configuration slightly to explore states
    // Ensure bounds checking before accessing Data array
    cfg.g_w = (Data[0] % 100) + 16; // Width between 16 and 115
    cfg.g_h = (Data[1] % 100) + 16; // Height between 16 and 115
    cfg.g_bit_depth = 8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    
    // Safe access for bitrate
    int bitrate_val = (Size > 2) ? Data[2] : 0;
    cfg.rc_target_bitrate = (bitrate_val % 1000) + 100;

    // Initialize the codec
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // 2. & 3. Explore program states by invoking target functions diversely
    int op_selector = get_val(Data, Size, 0, 5);

    switch (op_selector) {
        case 0: // AV1E_SET_ENABLE_SUPERRES
            // Value: 0 or 1
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_SUPERRES, get_val(Data, Size, 1, 1));
            break;

        case 1: // AV1E_SET_ENABLE_QM
            // Value: 0 or 1
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_QM, get_val(Data, Size, 1, 1));
            break;

        case 2: // AV1E_SET_COLOR_PRIMARIES
            // Value: AOM_CPRIMARIES_BT709 (0) to AOM_CPRIMARIES_XYZ (2)
            // Using a small range to stay within valid enum bounds mostly, 
            // but fuzzing might pass invalid values to test error handling.
            ret = aom_codec_control(&codec, AV1E_SET_COLOR_PRIMARIES, get_val(Data, Size, 1, 10));
            break;

        case 3: // AV1E_SET_ALLOW_WARPED_MOTION
            // Value: 0 or 1
            ret = aom_codec_control(&codec, AV1E_SET_ALLOW_WARPED_MOTION, get_val(Data, Size, 1, 1));
            break;

        case 4: // AV1E_SET_COEFF_COST_UPD_FREQ
            // Value: integer frequency
            ret = aom_codec_control(&codec, AV1E_SET_COEFF_COST_UPD_FREQ, get_val(Data, Size, 1, 100));
            break;

        case 5: // AV1E_GET_SEQ_LEVEL_IDX
            // This is a GET operation, returns value via pointer
            idx = 0;
            ret = aom_codec_control(&codec, AV1E_GET_SEQ_LEVEL_IDX, &idx);
            break;
            
        default:
            // Fallback to a random control if selector is out of expected range
            ret = aom_codec_control(&codec, AV1E_SET_ENABLE_SUPERRES, get_val(Data, Size, 1, 1));
            break;
    }

    // 4. Wrap logic (already inside LLVMFuzzerTestOneInput)
    // 5. No file operations required for these specific control functions.
    
    // 6. Cleanup
    cleanup_encoder(&codec);

    return 0;
}