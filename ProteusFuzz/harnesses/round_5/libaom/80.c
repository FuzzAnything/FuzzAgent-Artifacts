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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom_external_partition.h>
#include <aom_decoder.h>
#include <aom_image.h>
#include <aom_frame_buffer.h>
#include <aom_integer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    const aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Initialize encoder configuration
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration from fuzz input
    // Ensure we have enough data before accessing indices
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 100) + 100;

    // Initialize codec context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fuzz various control functions
    int32_t value = Data[4] % 2; // 0 or 1

    // AV1E_SET_ENABLE_DIAGONAL_INTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, value);
    (void)ret;

    // AV1E_SET_INTRA_DCT_ONLY
    ret = aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, value);
    (void)ret;

    // AV1E_SET_ENABLE_WARPED_MOTION
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, value);
    (void)ret;

    // AV1E_SET_ENABLE_FLIP_IDTX
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, value);
    (void)ret;

    // AV1E_SET_CDF_UPDATE_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, value);
    (void)ret;

    // AV1E_SET_LOOPFILTER_CONTROL
    // This typically takes a struct, so we use a simple int for fuzzing purposes
    // Note: In a real scenario, this might need a specific struct type.
    ret = aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, value);
    (void)ret;

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}