// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>

static int get_int(const uint8_t **d, size_t *s) {
    if (*s < 4) return 0;
    int val = (*d)[0] | (*d)[1] << 8 | (*d)[2] << 16 | (*d)[3] << 24;
    *d += 4;
    *s -= 4;
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->iface) return 0;

    const uint8_t *ptr = data;
    size_t remaining = size;

    // AV1E_SET_INTER_DCT_ONLY
    if (remaining >= 4) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
    }

    // AV1E_ENABLE_SB_QP_SWEEP
    if (remaining >= 4) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_ENABLE_SB_QP_SWEEP, val);
    }

    // AV1E_SET_QM_V
    if (remaining >= 256) {
        aom_codec_control(ctx, AV1E_SET_QM_V, (void *)ptr);
        ptr += 256;
        remaining -= 256;
    }

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR
    if (remaining >= 16) {
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void *)ptr);
        ptr += 16;
        remaining -= 16;
    }

    // AV1E_SET_ENABLE_ANGLE_DELTA
    if (remaining >= 4) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val);
    }

    // AV1E_SET_FP_MT_UNIT_TEST
    if (remaining >= 4) {
        int val = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_FP_MT_UNIT_TEST, val);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_end_usage = AOM_CBR;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_controls(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}