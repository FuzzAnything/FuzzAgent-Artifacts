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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, int val) {
    aom_codec_err_t res;
    int enable = (val != 0) ? 1 : 0;

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable);
    if (res != AOM_CODEC_OK) return 0;

    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, enable);
    if (res != AOM_CODEC_OK) return 0;

    // AV1E_ENABLE_EXT_TILE_DEBUG
    res = aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, enable);
    if (res != AOM_CODEC_OK) return 0;

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable);
    if (res != AOM_CODEC_OK) return 0;

    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable);
    if (res != AOM_CODEC_OK) return 0;

    // AV1E_SET_INTRA_DEFAULT_TX_ONLY
    res = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, enable);
    if (res != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_iface_t *iface;
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int val = Data[0];

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    fuzz_encoder_controls(&ctx, val);

    aom_codec_destroy(&ctx);

    return 0;
}