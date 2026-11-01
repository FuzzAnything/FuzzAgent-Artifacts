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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int width = 16;
    int height = 16;

    if (size > 2) {
        width = (data[0] % 32) + 16;
        height = (data[1] % 32) + 16;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int value;

    if (Size < 2) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    value = (Data[0] % 2);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    value = (Data[1] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DUAL_FILTER
    if (Size > 2) value = (Data[2] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    if (Size > 3) value = (Data[3] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, value);
    (void)ret;

    // Fuzz AOME_SET_TUNING
    // Tuning modes are typically 0 (PSNR) or 1 (SSIM)
    if (Size > 4) value = (Data[4] % 2);
    ret = aom_codec_control(&ctx, AOME_SET_TUNING, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    if (Size > 5) value = (Data[5] % 2);
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, value);
    (void)ret;

    cleanup_codec(&ctx);

    return 0;
}