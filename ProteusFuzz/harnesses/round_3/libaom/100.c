// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    int ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

static int fuzz_encoder_control_int(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    int ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use aom_codec_av1_cx() which is the correct API for getting the AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Data[4] % 100) + 100;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    int val1 = (int)(Data[0] % 2);
    int val2 = (int)(Data[1] % 2);
    int val3 = (int)(Data[2] % 2);
    int val4 = (int)(Data[3] % 4);
    int val5 = (int)(Data[4] % 101);
    int val6 = (int)(Data[5] % 2);

    // Use the correct control IDs defined in aom_encoder.h
    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);

    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val2);

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val3);

    // AV1E_SET_TUNE_CONTENT
    fuzz_encoder_control(&ctx, AV1E_SET_TUNE_CONTENT, val4);

    // AOME_SET_MAX_INTRA_BITRATE_PCT
    fuzz_encoder_control_int(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val5);

    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val6);

    aom_codec_destroy(&ctx);

    return 0;
}