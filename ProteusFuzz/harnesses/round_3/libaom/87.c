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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
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
    aom_codec_ctx_t *codec_ctx;

    // Use the standard function to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    codec_ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec_ctx) {
        return -1;
    }
    memset(codec_ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(codec_ctx);
        return -1;
    }

    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1] % 256) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (data[2] % 60) + 1;
        cfg.rc_target_bitrate = (data[3] % 1000) + 100;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(codec_ctx);
        return -1;
    }

    *ctx = codec_ctx;
    return 0;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    int ret;
    int val;

    if (Size < 1) {
        return 0;
    }

    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    val = (Data[0] % 2);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);
    (void)ret;

    if (Size > 1) {
        val = (Data[1] % 100);
        ret = aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val);
        (void)ret;
    }

    if (Size > 2) {
        val = (Data[2] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, val);
        (void)ret;
    }

    if (Size > 3) {
        val = (Data[3] % 10);
        ret = aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val);
        (void)ret;
    }

    if (Size > 4) {
        val = (Data[4] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val);
        (void)ret;
    }

    if (Size > 5) {
        val = (Data[5] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val);
        (void)ret;
    }

    cleanup_codec(ctx);
    return 0;
}