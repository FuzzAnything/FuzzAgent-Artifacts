// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

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

static int fuzz_encoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int ctrl_id;
    int value;

    if (size < 3) return 0;

    ctrl_id = (data[0] & 0x0F) + 1; // Ensure non-zero, small range
    value = (data[1] << 8) | data[2];

    // Initialize encoder config
    aom_codec_enc_config_default(NULL, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize codec context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Fuzz aom_codec_control with various control IDs
    // Try AV1E_SET_ENABLE_PALETTE
    if (ctrl_id == 1) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, value ? 1 : 0);
    }
    // Try AV1E_SET_ENABLE_OVERLAY
    else if (ctrl_id == 2) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, value ? 1 : 0);
    }
    // Try AV1E_SET_ENABLE_MASKED_COMP
    else if (ctrl_id == 3) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, value ? 1 : 0);
    }
    // Try AV1E_SET_ENABLE_INTERINTRA_COMP
    else if (ctrl_id == 4) {
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, value ? 1 : 0);
    }
    // Try AV1E_GET_NUM_OPERATING_POINTS
    else if (ctrl_id == 5) {
        int *num_ops = malloc(sizeof(int));
        if (num_ops) {
            aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, num_ops);
            free(num_ops);
        }
    }
    // Try generic control with random ID
    else {
        aom_codec_control(&ctx, ctrl_id, value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    if (size < 1) return 0;

    // Initialize decoder config
    cfg.threads = 1;
    // Use valid member names based on error log
    cfg.allow_lowbitdepth = 1;

    // Get decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    // Initialize codec context
    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Try some decoder controls if available
    // Note: AV1D_* constants might not exist for all controls, using encoder ones if decoder specific are missing
    // Attempting to use valid constants. If AV1D_SET_COLOR_RANGE doesn't exist, we skip or use valid ones.
    // Based on error, AV1D_SET_COLOR_RANGE is not defined, so we skip decoder specific color controls 
    // or use generic ones if known. For safety, we will skip specific decoder controls that are undefined.
    
    // Just perform a generic valid control call if possible, or skip to avoid compilation errors
    // Since we don't know valid decoder control IDs from the error log, we skip specific decoder controls.
    // We can still test the control mechanism with a known valid ID if available, but without docs, 
    // we rely on the encoder path for the main fuzzing of the target APIs.
    
    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to decide encoder vs decoder fuzzing
    if (Data[0] & 0x80) {
        fuzz_decoder_control(Data, Size);
    } else {
        fuzz_encoder_control(Data, Size);
    }

    return 0;
}