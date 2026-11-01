// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, aom_codec_enc_cfg_t *cfg) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_266(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to vary configuration slightly
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000 + (Data[0] * 10);

    aom_codec_ctx_t *ctx = init_encoder(iface, &cfg);
    if (!ctx) return 0;

    int val;
    aom_codec_err_t err;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    val = Data[2] % 2;
    err = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)err;

    // Fuzz AV1E_SET_QM_U
    val = Data[3] % 256;
    err = aom_codec_control(ctx, AV1E_SET_QM_U, val);
    (void)err;

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    val = Data[4] % 2;
    err = aom_codec_control(ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
    (void)err;

    // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING
    val = Data[5] % 2;
    err = aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);
    (void)err;

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_Y
    val = Data[6] % 4;
    err = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val);
    (void)err;

    // Fuzz AV1E_SET_ENABLE_CDEF
    val = Data[7] % 2;
    err = aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, val);
    (void)err;

    cleanup_encoder(ctx);
    return 0;
}