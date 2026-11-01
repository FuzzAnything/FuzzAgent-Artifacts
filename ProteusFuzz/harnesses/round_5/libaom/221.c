// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get the AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure some parameters
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.g_profile = (Data[4] % 4);

    // Initialize the encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_set_option with various keys and values
    char option_name[64];
    char option_value[64];

    // Ensure we have enough data to process at least one iteration safely
    size_t max_idx = Size - 1;
    for (size_t i = 0; i < max_idx; i += 2) {
        size_t name_len = (Data[i] % 32) + 1;
        if (name_len >= sizeof(option_name)) name_len = sizeof(option_name) - 1;
        
        // Check if we have enough data for the name
        if (i + name_len > Size) break;

        for (size_t j = 0; j < name_len; j++) {
            option_name[j] = (char)((Data[i + j] % 95) + 32);
        }
        option_name[name_len] = '\0';

        size_t val_len = (Data[i + 1] % 32) + 1;
        if (val_len >= sizeof(option_value)) val_len = sizeof(option_value) - 1;

        // Check if we have enough data for the value
        if (i + 1 + val_len > Size) break;

        for (size_t j = 0; j < val_len; j++) {
            option_value[j] = (char)((Data[i + 1 + j] % 95) + 32);
        }
        option_value[val_len] = '\0';

        aom_codec_set_option(&ctx, option_name, option_value);
    }

    // Invoke control functions using correct constants
    int enable_intrabc = Data[0] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    int skip_postproc = Data[1] % 2;
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);

    int tier_mask = Data[2] % 4;
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, tier_mask);

    int enable_cdef = Data[3] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);

    int denoise_block_size = Data[4] % 8;
    aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}