// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to avoid buffer overflows
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some configuration parameters safely
    cfg.g_w = (Data[0] % 100) + 16;
    cfg.g_h = (Data[1] % 100) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions with fuzzing data
    uint8_t val = Data[2] % 2;

    // AV1E_SET_ENABLE_INTRABC
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // AV1E_SET_CHROMA_SAMPLE_POSITION
    int chroma_pos = Data[3] % 4;
    if (aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // AV1E_SET_LOOPFILTER_CONTROL
    int loopfilter_level = Data[4] % 64;
    if (aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_level) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // AV1E_SET_ENABLE_RECT_PARTITIONS
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // AV1E_SET_DENOISE_NOISE_LEVEL
    int noise_level = Data[5] % 100;
    if (aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, noise_level) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // AV1E_SET_ENABLE_WARPED_MOTION
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, val) != AOM_CODEC_OK) {
        // Ignore errors, continue fuzzing
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}