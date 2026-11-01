// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

/*
 * Helper function to initialize an encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height, int fps_num, int fps_den) {
    if (!ctx || !cfg) return -1;

    memset(cfg, 0, sizeof(*cfg));
    aom_codec_enc_config_default(NULL, cfg, 0);

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = fps_num;
    cfg->g_timebase.den = fps_den;
    cfg->g_profile = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, NULL, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_344(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Extract parameters from fuzz input to vary encoder configuration
    int width = (Data[0] % 128) + 16; // 16 to 143
    int height = (Data[1] % 128) + 16;
    int fps_num = (Data[2] % 60) + 1;
    int fps_den = (Data[3] % 60) + 1;

    // Ensure reasonable dimensions
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    if (init_encoder(&ctx, &cfg, width, height, fps_num, fps_den) != 0) {
        return 0;
    }

    // Try to invoke the target control functions with varying integer values
    // Since the signatures are not provided, we assume they follow the standard
    // aom_codec_control pattern: aom_codec_control(ctx, CONTROL_ID, value)
    // We will attempt to call them using the standard macro if available,
    // or direct function calls if the specific typechecked wrappers exist.
    // Given the names, they likely map to standard AV1 encoder controls.
    
    // We use a loop to try different values for the controls to explore state space
    for (size_t i = 4; i + 3 < Size; i += 4) {
        int val = (Data[i] | (Data[i+1] << 8) | (Data[i+2] << 16) | (Data[i+3] << 24));
        int val_bool = (val & 1); // Use as boolean for enable/disable flags

        // 1. AV1E_SET_INTER_DCT_ONLY
        // Assuming it takes an int (0 or 1)
        if (aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val_bool) != AOM_CODEC_OK) {
            // Ignore errors, continue fuzzing
        }

        // 2. AV1E_SET_SKIP_POSTPROC_FILTERING
        if (aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val_bool) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // 3. AV1E_SET_ENABLE_OBMC
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_OBMC, val_bool) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // 4. AV1E_SET_ENABLE_INTRABC
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val_bool) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // 5. AV1E_SET_ENABLE_KEYFRAME_FILTERING
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val_bool) != AOM_CODEC_OK) {
            // Ignore errors
        }

        // 6. AV1E_SET_ENABLE_CDEF
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, val_bool) != AOM_CODEC_OK) {
            // Ignore errors
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}