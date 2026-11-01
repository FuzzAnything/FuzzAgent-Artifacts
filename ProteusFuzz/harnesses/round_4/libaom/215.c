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

static int init_encoder(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!*ctx) {
        return -1;
    }

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return -1;
    }

    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (size > 1 ? (data[1] % 1024) + 16 : 16);
        cfg.g_timebase.num = (size > 2 ? data[2] : 1);
        cfg.g_timebase.den = (size > 3 ? (data[3] % 255) + 1 : 1);
        cfg.rc_target_bitrate = (size > 4 ? data[4] * 1000 : 1000);
        cfg.g_error_resilient = (size > 5 ? data[5] % 2 : 0);
    }

    err = aom_codec_enc_init(*ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(*ctx);
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    int ret = 0;
    int value;
    int enable_val;
    int width, height;

    if (Size < 1) {
        return 0;
    }

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    enable_val = (Data[0] % 2);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, enable_val);
    (void)ret;

    ret = aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &value);
    (void)ret;

    width = (Size > 1 ? Data[1] % 4096 : 128);
    height = (Size > 2 ? Data[2] % 4096 : 128);
    
    if (width > 0 && height > 0) {
        int render_size[2];
        render_size[0] = width;
        render_size[1] = height;
        ret = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, render_size);
        (void)ret;
    }

    enable_val = (Size > 3 ? Data[3] % 2 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, enable_val);
    (void)ret;

    enable_val = (Size > 4 ? Data[4] % 2 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_val);
    (void)ret;

    enable_val = (Size > 5 ? Data[5] % 2 : 0);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, enable_val);
    (void)ret;

    cleanup_codec(ctx);

    return 0;
}