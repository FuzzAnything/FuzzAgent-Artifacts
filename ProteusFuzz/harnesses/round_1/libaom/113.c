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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int32_t val1 = 0;
    int32_t val2 = 0;
    int32_t val3 = 0;
    int32_t val4 = 0;
    int32_t val5 = 0;
    int32_t val6 = 0;

    if (size >= 24) {
        memcpy(&val1, data, 4);
        memcpy(&val2, data + 4, 4);
        memcpy(&val3, data + 8, 4);
        memcpy(&val4, data + 12, 4);
        memcpy(&val5, data + 16, 4);
        memcpy(&val6, data + 20, 4);
    } else if (size >= 4) {
        memcpy(&val1, data, 4);
    }

    if (aom_codec_control(ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val1) != AOM_CODEC_OK) {
        // Ignore errors
    }

    if (aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, val2) != AOM_CODEC_OK) {
        // Ignore errors
    }

    if (aom_codec_control(ctx, AV1E_SET_MAX_GF_INTERVAL, val3) != AOM_CODEC_OK) {
        // Ignore errors
    }

    if (aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, val4) != AOM_CODEC_OK) {
        // Ignore errors
    }

    if (aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val5) != AOM_CODEC_OK) {
        // Ignore errors
    }

    if (aom_codec_control(ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val6) != AOM_CODEC_OK) {
        // Ignore errors
    }

    return 0;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *encoder = aom_codec_av1_cx();

    if (!encoder) return 0;

    if (aom_codec_enc_config_default(encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (Size > 2 ? Data[2] % 10000 : 0);
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(&ctx, encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_controls(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}