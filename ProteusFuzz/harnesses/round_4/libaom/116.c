// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int test_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;

    if (size >= 4) {
        val = *(const int32_t *)data;
        ret = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= 8) {
        val = *(const int32_t *)(data + 4);
        ret = aom_codec_control(ctx, AV1E_SET_AUTO_TILES, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= 12) {
        val = *(const int32_t *)(data + 8);
        ret = aom_codec_control(ctx, AV1E_SET_TILE_ROWS, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= 16) {
        val = *(const int32_t *)(data + 12);
        ret = aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    return AOM_CODEC_OK;
}

static int test_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;
    int tile_count;

    if (size >= 4) {
        val = *(const int32_t *)data;
        ret = aom_codec_control(ctx, AV1D_GET_TILE_COUNT, &tile_count);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= 8) {
        val = *(const int32_t *)(data + 4);
        ret = aom_codec_control(ctx, AV1D_EXT_TILE_DEBUG, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    return AOM_CODEC_OK;
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *encoder_iface;
    aom_codec_iface_t *decoder_iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;
    int ret;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));

    // Test Encoder Controls
    encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) return 0;

    ret = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
    if (ret == AOM_CODEC_OK) {
        test_encoder_controls(&ctx, Data, Size);
        aom_codec_destroy(&ctx);
    }

    // Test Decoder Controls
    decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(&ctx, decoder_iface, &dec_cfg, 0);
    if (ret == AOM_CODEC_OK) {
        test_decoder_controls(&ctx, Data, Size);
        aom_codec_destroy(&ctx);
    }

    return 0;
}