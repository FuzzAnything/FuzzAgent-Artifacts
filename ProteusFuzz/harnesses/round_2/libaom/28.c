// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom.h"

/*
 * Fuzz driver for libaom codec control functions.
 * Target functions:
 * 1. AOME_SET_ARNR_STRENGTH
 * 2. AV1E_SET_ENABLE_DIFF_WTD_COMP
 * 3. AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
 * 4. AV1E_SET_ENABLE_PALETTE
 * 5. AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * 6. AV1E_SET_ENABLE_GLOBAL_MOTION
 */

static int setup_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(codec, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t enc_cfg;
    int result;
    int idx = Data[0] % 6;

    // Initialize codec context
    memset(&codec, 0, sizeof(codec));

    // All target functions are encoder-side controls (AV1E_* or AOME_*)
    // We only need to set up the encoder.
    if (!setup_encoder(&codec, &enc_cfg)) {
        return 0;
    }

    switch (idx) {
        case 0:
            // AOME_SET_ARNR_STRENGTH
            // Value from input, clamped to reasonable range (0-63)
            int arnr_strength = (Size > 1) ? (Data[1] % 64) : 0;
            result = aom_codec_control(&codec, AOME_SET_ARNR_STRENGTH, arnr_strength);
            break;
        case 1:
            // AV1E_SET_ENABLE_DIFF_WTD_COMP
            int enable_diff_wtd = (Size > 1) ? (Data[1] % 2) : 0;
            result = aom_codec_control(&codec, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable_diff_wtd);
            break;
        case 2:
            // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
            int enable_low_complexity = (Size > 1) ? (Data[1] % 2) : 0;
            result = aom_codec_control(&codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, enable_low_complexity);
            break;
        case 3:
            // AV1E_SET_ENABLE_PALETTE
            int enable_palette = (Size > 1) ? (Data[1] % 2) : 0;
            result = aom_codec_control(&codec, AV1E_SET_ENABLE_PALETTE, enable_palette);
            break;
        case 4:
            // AV1E_SET_ENABLE_TX_SIZE_SEARCH
            int enable_tx_search = (Size > 1) ? (Data[1] % 2) : 0;
            result = aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_search);
            break;
        case 5:
            // AV1E_SET_ENABLE_GLOBAL_MOTION
            int enable_global_motion = (Size > 1) ? (Data[1] % 2) : 0;
            result = aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);
            break;
        default:
            result = 0;
            break;
    }

    cleanup_encoder(&codec);
    
    (void)result; // Suppress unused variable warning
    return 0;
}