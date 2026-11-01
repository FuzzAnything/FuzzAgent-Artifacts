// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>

static int test_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;

    if (size > 0) val = data[0];
    else val = 0;

    // AV1E_ENABLE_EXT_TILE_DEBUG
    if (aom_codec_control(codec, AV1E_ENABLE_EXT_TILE_DEBUG, val) != AOM_CODEC_OK) {
        ret = 1;
    }

    if (size > 1) val = data[1];
    else val = 1;

    // AV1E_SET_TILE_ROWS
    if (aom_codec_control(codec, AV1E_SET_TILE_ROWS, val) != AOM_CODEC_OK) {
        ret = 1;
    }

    if (size > 2) val = data[2];
    else val = 0;

    // AV1E_SET_AUTO_TILES
    if (aom_codec_control(codec, AV1E_SET_AUTO_TILES, val) != AOM_CODEC_OK) {
        ret = 1;
    }

    if (size > 3) val = data[3];
    else val = 1;

    // AV1E_SET_TILE_COLUMNS
    if (aom_codec_control(codec, AV1E_SET_TILE_COLUMNS, val) != AOM_CODEC_OK) {
        ret = 1;
    }

    if (size > 4) val = data[4];
    else val = 0;

    // AV1E_SET_SINGLE_TILE_DECODING
    if (aom_codec_control(codec, AV1E_SET_SINGLE_TILE_DECODING, val) != AOM_CODEC_OK) {
        ret = 1;
    }

    return ret;
}

int LLVMFuzzerTestOneInput_238(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t enc_ctx;
    aom_codec_iface_t *enc_iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int result = 0;

    // Initialize Encoder
    enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    err = aom_codec_enc_config_default(enc_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use fuzzing data to set some config parameters safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 10000) + 100;

    err = aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Test Encoder Controls
    result |= test_encoder_controls(&enc_ctx, Data, Size);

    // Cleanup Encoder
    aom_codec_destroy(&enc_ctx);

    return result;
}