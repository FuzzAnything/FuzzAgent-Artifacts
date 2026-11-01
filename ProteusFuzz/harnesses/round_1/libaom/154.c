// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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
    if (ctx == NULL || data == NULL) return 0;
    if (size < 1) return 0;

    // Extract values from fuzz input to set various controls
    int val = data[0];

    // AV1E_SET_NOISE_SENSITIVITY
    if (aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val & 1) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AV1E_SET_ENABLE_INTRABC
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val & 1) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AV1E_SET_MAX_REFERENCE_FRAMES (limit to valid range 0-7)
    int max_ref = val % 8;
    if (aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AV1E_SET_AUTO_INTRA_TOOLS_OFF
    if (aom_codec_control(ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, val & 1) != AOM_CODEC_OK) {
        // Ignore error
    }

    // AV1E_SET_TARGET_SEQ_LEVEL_IDX (limit to valid range 0-127)
    int level_idx = val % 128;
    if (aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx) != AOM_CODEC_OK) {
        // Ignore error
    }

    return 1;
}

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default("libaom-av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config values from fuzz input
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] % 1000) + 100;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}