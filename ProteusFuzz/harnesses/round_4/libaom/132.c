// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static int try_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || ctx->err != AOM_CODEC_OK) return 0;

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR
    if (size >= 4) {
        uint32_t val = *(const uint32_t *)(data);
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val);
    }

    // AOME_SET_NUMBER_SPATIAL_LAYERS
    if (size >= 8) {
        uint32_t val = *(const uint32_t *)(data + 4);
        aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, val);
    }

    // AV1E_SET_SVC_FRAME_DROP_MODE
    if (size >= 12) {
        uint32_t val = *(const uint32_t *)(data + 8);
        aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val);
    }

    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    if (size >= 16) {
        uint32_t val = *(const uint32_t *)(data + 12);
        aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
    }

    // AOME_SET_SPATIAL_LAYER_ID
    if (size >= 20) {
        uint32_t val = *(const uint32_t *)(data + 16);
        aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, val);
    }

    return 1;
}

static int try_decoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || ctx->err != AOM_CODEC_OK) return 0;

    // AV1D_SET_OPERATING_POINT
    if (size >= 4) {
        uint32_t val = *(const uint32_t *)(data);
        aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, val);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t enc_ctx;
    aom_codec_ctx_t dec_ctx;
    aom_codec_err_t res;
    aom_codec_iface_t *enc_iface;
    aom_codec_iface_t *dec_iface;
    aom_codec_enc_cfg_t cfg;

    memset(&enc_ctx, 0, sizeof(enc_ctx));
    memset(&dec_ctx, 0, sizeof(dec_ctx));

    enc_iface = aom_codec_av1_cx();
    dec_iface = aom_codec_av1_dx();

    if (!enc_iface || !dec_iface) return 0;

    // Initialize encoder config
    res = aom_codec_enc_config_default(enc_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder
    res = aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    // Initialize decoder
    res = aom_codec_dec_init(&dec_ctx, dec_iface, NULL, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    // Run encoder controls
    try_encoder_control(&enc_ctx, Data, Size);

    // Run decoder controls
    try_decoder_control(&dec_ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&enc_ctx);
    aom_codec_destroy(&dec_ctx);

    return 0;
}