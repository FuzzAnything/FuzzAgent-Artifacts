// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int width = 16, height = 16;

    if (size >= 4) {
        width = (data[0] % 128) + 16;
        height = (data[1] % 128) + 16;
    }

    // Use the standard API to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!*ctx) return 0;
    memset(*ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    ret = aom_codec_enc_init(*ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t ret;
    int val;

    if (Size < 1) return 0;

    if (!init_encoder_context(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    if (Size > 0) {
        val = (Data[0] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_INTER_DCT_ONLY
    if (Size > 1) {
        val = (Data[1] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_DUAL_FILTER
    if (Size > 2) {
        val = (Data[2] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    if (Size > 3) {
        val = (Data[3] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_TUNE_CONTENT
    if (Size > 4) {
        val = (Data[4] % 4); // Assuming enum values 0-3
        ret = aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    if (Size > 5) {
        val = (Data[5] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);
        (void)ret;
    }

    cleanup_codec(ctx);
    return 0;
}