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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL) return 0;

    int ret = 0;
    int val = 0;
    int range_val = 0;
    int prim_val = 0;
    int trans_val = 0;
    int matrix_val = 0;

    if (size > 0) val = data[0];
    if (size > 1) range_val = data[1];
    if (size > 2) prim_val = data[2];
    if (size > 3) trans_val = data[3];
    if (size > 4) matrix_val = data[4];

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    (void)ret;

    // AV1E_SET_ENABLE_CFL_INTRA
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    (void)ret;

    // AV1E_SET_COLOR_RANGE
    ret = aom_codec_control(ctx, AV1E_SET_COLOR_RANGE, range_val);
    (void)ret;

    // AV1E_SET_COLOR_PRIMARIES
    ret = aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, prim_val);
    (void)ret;

    // AV1E_SET_TRANSFER_CHARACTERISTICS
    ret = aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, trans_val);
    (void)ret;

    // AV1E_SET_MATRIX_COEFFICIENTS
    ret = aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, matrix_val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_312(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;

    aom_codec_ctx_t ctx;
    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_control(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}