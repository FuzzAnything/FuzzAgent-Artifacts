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

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 10;
    cfg->rc_max_quantizer = 63;
    cfg->g_error_resilient = 1;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Extract dimensions from input to vary encoder state
    int width = (Data[0] % 128) + 16; // 16 to 143
    int height = (Data[1] % 128) + 16; // 16 to 143
    
    // Ensure dimensions are multiples of 8 for AV1
    width = (width / 8) * 8;
    height = (height / 8) * 8;
    if (width < 8) width = 8;
    if (height < 8) height = 8;

    if (!init_encoder(&ctx, &cfg, width, height)) {
        return 0;
    }

    // Use remaining data to determine which control to test and the value
    int control_idx = 0;
    int value = 0;

    if (Size > 2) {
        control_idx = Data[2] % 6; // Select one of the 6 controls
    }
    
    if (Size > 3) {
        value = Data[3];
    }

    // Map control_idx to specific control IDs
    int control_id = 0;
    
    switch (control_idx) {
        case 0: // AV1E_SET_ENABLE_SMOOTH_INTERINTRA
            control_id = AV1E_SET_ENABLE_SMOOTH_INTERINTRA;
            break;
        case 1: // AV1E_SET_ENABLE_DIRECTIONAL_INTRA
            control_id = AV1E_SET_ENABLE_DIRECTIONAL_INTRA;
            break;
        case 2: // AV1E_SET_ENABLE_SUPERRES
            control_id = AV1E_SET_ENABLE_SUPERRES;
            break;
        case 3: // AV1E_SET_ENABLE_DIAGONAL_INTRA
            control_id = AV1E_SET_ENABLE_DIAGONAL_INTRA;
            break;
        case 4: // AV1E_SET_MIN_PARTITION_SIZE
            // This control expects a specific integer type (likely int or unsigned int)
            // We use the value modulo a reasonable range (e.g., 4 to 256)
            value = (value % 253) + 4; 
            control_id = AV1E_SET_MIN_PARTITION_SIZE;
            break;
        case 5: // AV1E_SET_ENABLE_ORDER_HINT
            control_id = AV1E_SET_ENABLE_ORDER_HINT;
            break;
    }

    // Invoke the control function
    aom_codec_err_t result = aom_codec_control(&ctx, control_id, value);

    // We don't necessarily fail on error, as fuzzing should explore error paths too.
    
    cleanup_encoder(&ctx);

    return 0;
}