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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_178(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    // Get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure basic parameters safely
    cfg.g_w = (Data[0] % 256) + 16; // Ensure minimum width
    cfg.g_h = (Data[1] % 256) + 16; // Ensure minimum height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control IDs and values from input
    int control_id = Data[0];
    int value = Data[1] % 2; // Toggle between 0 and 1 for boolean flags

    // Map specific control IDs based on known AV1 encoder controls
    switch (control_id) {
        case 0:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, value);
            break;
        case 1:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, value);
            break;
        case 2:
            fuzz_codec_control(&ctx, AOME_SET_TUNING, value);
            break;
        case 3:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, value);
            break;
        case 4:
            fuzz_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, value);
            break;
        case 5:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value);
            break;
        default:
            // Randomly pick one of the controls for fuzzing
            int random_control = control_id % 6;
            switch (random_control) {
                case 0: fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, value); break;
                case 1: fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, value); break;
                case 2: fuzz_codec_control(&ctx, AOME_SET_TUNING, value); break;
                case 3: fuzz_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, value); break;
                case 4: fuzz_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, value); break;
                case 5: fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value); break;
            }
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}