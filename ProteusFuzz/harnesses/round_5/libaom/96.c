// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, int value) {
    if (!ctx) return 0;

    // Try to set various AV1 encoder controls if the context is initialized
    // Note: These macros expand to aom_codec_control calls with specific IDs
    // Since the exact macro definitions for the typechecked versions are not fully detailed in the prompt,
    // we assume they map to standard aom_codec_control calls with AV1E_* IDs.
    // We will use the standard aom_codec_control API with the known AV1E_* IDs which these functions wrap.
    
    // AV1E_SET_ENABLE_INTRABC
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, value) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing, we want to exercise the code path
    }

    // AV1E_SET_ENABLE_CFL_INTRA
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_ANGLE_DELTA
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_FILTER_INTRA
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_PAETH_INTRA
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_DIST_WTD_COMP
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    return 1;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from input
    int value = Data[0]; // 0 or 1 for boolean flags, or just an integer
    int width = (Data[1] % 16) + 16; // Ensure minimum size
    int height = (Size > 2 ? (Data[2] % 16) + 16 : 16);

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    // Initialize encoder config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;

    // Initialize the codec context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, value);

    // Try with inverted value if possible (0/1 toggle)
    fuzz_encoder_controls(&ctx, 1 - value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}