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
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aom_integer.h"
#include "aom/aomcx.h"

static int test_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val;

    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret = aom_codec_control(ctx, AOME_SET_SHARPNESS, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) + 4) {
        memcpy(&val, data + sizeof(int), sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) * 2 + 4) {
        memcpy(&val, data + sizeof(int) * 2, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) * 3 + 4) {
        memcpy(&val, data + sizeof(int) * 3, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    if (size >= sizeof(int) * 4 + 4) {
        memcpy(&val, data + sizeof(int) * 4, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_ROW_MT, val);
        if (ret != AOM_CODEC_OK) return ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_284(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *encoder_iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    if (Size == 0) return 0;

    // Initialize Encoder
    encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) return 0;

    err = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(&ctx, encoder_iface, &cfg, 0);
    if (err == AOM_CODEC_OK) {
        test_encoder_controls(&ctx, Data, Size);
        aom_codec_destroy(&ctx);
    }

    return 0;
}