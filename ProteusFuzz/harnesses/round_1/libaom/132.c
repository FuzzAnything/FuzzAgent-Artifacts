// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    // Extract integer values from input data for various controls
    int val1 = (size > 0) ? data[0] : 0;
    int val2 = (size > 1) ? data[1] : 0;
    int val3 = (size > 2) ? data[2] : 0;
    int val4 = (size > 3) ? data[3] : 0;
    int val5 = (size > 4) ? data[4] : 0;
    int val6 = (size > 5) ? data[5] : 0;

    // AV1E_SET_TILE_ROWS
    if (aom_codec_control(ctx, AV1E_SET_TILE_ROWS, val1) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    // AV1E_SET_ENABLE_ONESIDED_COMP
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val2) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    // AV1E_SET_ENABLE_CFL_INTRA
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val3) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    // AV1E_SET_FRAME_PARALLEL_DECODING
    if (aom_codec_control(ctx, AV1E_SET_FRAME_PARALLEL_DECODING, val4) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    // AV1E_SET_REDUCED_REFERENCE_SET
    if (aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val5) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    // AV1E_SET_ERROR_RESILIENT_MODE
    if (aom_codec_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, val6) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    return 0;
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration from input
    cfg.g_w = (Size > 0) ? Data[0] * 16 : 128;
    cfg.g_h = (Size > 1) ? Data[1] * 16 : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    // Create codec context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}