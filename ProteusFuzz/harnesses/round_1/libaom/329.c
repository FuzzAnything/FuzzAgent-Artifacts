// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t *init_encoder_context(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_usage = AOM_CBR;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_329(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) return 0;

    uint32_t value = 0;
    if (Size >= 4) {
        value = *(const uint32_t *)Data;
    }

    // Fuzz AV1E_SET_QM_MAX
    int ret = aom_codec_control(ctx, AV1E_SET_QM_MAX, value);
    (void)ret;

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    ret = aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, value);
    (void)ret;

    // Fuzz AV1E_SET_GF_CBR_BOOST_PCT
    ret = aom_codec_control(ctx, AV1E_SET_GF_CBR_BOOST_PCT, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, value);
    (void)ret;

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_Y
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, value);
    (void)ret;

    cleanup_codec(ctx);
    return 0;
}