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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    int32_t value;
    if (size >= sizeof(int32_t)) {
        memcpy(&value, data, sizeof(int32_t));
    } else {
        value = (int32_t)data[0];
    }

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    if (aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, value) != AOM_CODEC_OK) {
        // Ignore errors, fuzzing expects diverse states
    }

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    if (aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    if (aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    if (aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_S_FRAME_MODE
    if (aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, value) != AOM_CODEC_OK) {
        // Ignore errors
    }

    return 1;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config values to ensure valid state
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing on control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}