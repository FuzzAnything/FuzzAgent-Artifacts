// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

/*
 * Helper function to initialize the codec context.
 * Returns 0 on success, non-zero on failure.
 */
static int init_codec_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    if (!ctx) return 1;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 1;

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    // Use fuzzing data to set some configuration parameters safely
    if (size > 0) {
        cfg.g_w = (data[0] % 512) + 16; // Width between 16 and 527
        if (size > 1) cfg.g_h = (data[1] % 512) + 16; // Height between 16 and 527
        if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3] % 1000) + 1;
        if (size > 4) cfg.rc_target_bitrate = (data[4] % 10000) + 100;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 500;
    }

    // Initialize the encoder
    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

/*
 * Helper function to cleanup the codec context.
 */
static void cleanup_codec_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

/*
 * Fuzzing entry point
 */
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val;

    // Initialize the codec context
    if (init_codec_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Extract values from input data for control functions
    if (Size > 0) val = Data[0] % 100;
    else val = 50;

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    ret = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, val);
    (void)ret; // Ignore return value for fuzzing

    // Fuzz AV1E_ENABLE_RATE_GUIDE_DELTAQ
    if (Size > 1) val = Data[1] % 2;
    else val = 1;
    ret = aom_codec_control(&ctx, AV1E_ENABLE_RATE_GUIDE_DELTAQ, val);
    (void)ret;

    // Fuzz AV1E_SET_AQ_MODE
    if (Size > 2) val = Data[2] % 5;
    else val = 0;
    ret = aom_codec_control(&ctx, AV1E_SET_AQ_MODE, val);
    (void)ret;

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    if (Size > 3) val = Data[3] % 100;
    else val = 50;
    ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_QM
    if (Size > 4) val = Data[4] % 2;
    else val = 1;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val);
    (void)ret;

    // Fuzz AV1E_SET_QM_MAX
    if (Size > 5) val = Data[5] % 256;
    else val = 128;
    ret = aom_codec_control(&ctx, AV1E_SET_QM_MAX, val);
    (void)ret;

    // Cleanup
    cleanup_codec_context(&ctx);

    return 0;
}