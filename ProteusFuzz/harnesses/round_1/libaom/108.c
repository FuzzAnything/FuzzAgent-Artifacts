// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    const aom_codec_iface_t *iface;
    int ret;
    int32_t val1, val2, val3, val4, val5, val6;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 codec interface (encoder is selected via config flags or usage)
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic config from fuzz data
    cfg.g_w = (Data[0] % 4096) + 16;
    cfg.g_h = (Data[1] % 4096) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.g_profile = Data[5] % 4;
    cfg.rc_target_bitrate = (Data[6] * 256 + Data[7]) + 1000;
    cfg.g_usage = Data[8] % 4;

    // Initialize encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from fuzz data for control parameters
    val1 = (int32_t)(Data[9] % 2); // 0 or 1 for boolean-like flags
    val2 = (int32_t)(Data[10] % 64) + 1; // GF interval
    val3 = (int32_t)(Data[11] % 2);
    val4 = (int32_t)(Data[12] % 2);
    val5 = (int32_t)(Data[13] % 100) + 1; // Periodic boost
    val6 = (int32_t)(Data[14] % 5) + 1; // Pyramid height

    // Invoke target API functions
    // Fuzzing AV1E_SET_DISABLE_TRELLIS_QUANT
    ret = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val1);
    if (ret != AOM_CODEC_OK) {
        // Error is acceptable in fuzzing
    }

    // Fuzzing AV1E_SET_MAX_GF_INTERVAL
    ret = aom_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val2);
    if (ret != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzzing AV1E_SET_ENABLE_TPL_MODEL
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val3);
    if (ret != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzzing AV1E_SET_ENABLE_REF_FRAME_MVS
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val4);
    if (ret != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzzing AV1E_SET_FRAME_PERIODIC_BOOST
    ret = aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val5);
    if (ret != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzzing AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    ret = aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val6);
    if (ret != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}