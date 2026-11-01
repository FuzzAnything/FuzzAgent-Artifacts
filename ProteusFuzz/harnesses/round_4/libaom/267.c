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
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic configuration
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder settings safely
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Safely extract values from input, ensuring bounds checks
    int32_t val1 = 0;
    int32_t val2 = 0;
    int32_t val3 = 0;

    if (Size >= 8) {
        val1 = *(const int32_t *)(Data + 4);
    }
    if (Size >= 12) {
        val2 = *(const int32_t *)(Data + 8);
    }
    if (Size >= 16) {
        val3 = *(const int32_t *)(Data + 12);
    }

    // Fuzz AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    fuzz_codec_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val1);

    // Fuzz AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    fuzz_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val2);

    // Fuzz AV1E_SET_MIN_GF_INTERVAL
    fuzz_codec_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, val3);

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT (boolean-like)
    fuzz_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val1 % 2);

    // Fuzz AV1E_SET_MAX_GF_INTERVAL
    fuzz_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val2);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL (boolean-like)
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val3 % 2);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}