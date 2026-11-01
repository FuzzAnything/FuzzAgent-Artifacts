// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const aom_codec_enc_cfg_t *cfg) {
    if (ctx == NULL) return 0;
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_200(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (ctx == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);

    // Use input data to vary configuration parameters
    if (Size >= 4) {
        cfg.g_w = (Data[0] % 128) + 16;
        cfg.g_h = (Data[1] % 128) + 16;
        cfg.g_timebase.num = (Data[2] % 30) + 1;
        cfg.g_timebase.den = (Data[3] % 30) + 1;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
    }

    if (!init_encoder_context(ctx, &cfg)) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz various control functions using input data
    if (Size > 0) {
        int32_t val1 = (int32_t)Data[0];
        aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val1);
    }

    if (Size > 1) {
        int32_t val2 = (int32_t)Data[1];
        aom_codec_control(ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, val2);
    }

    if (Size > 2) {
        int32_t val3 = (int32_t)Data[2];
        aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, val3);
    }

    if (Size > 3) {
        int32_t val4 = (int32_t)Data[3];
        aom_codec_control(ctx, AV1E_SET_MAX_GF_INTERVAL, val4);
    }

    if (Size > 4) {
        int32_t val5 = (int32_t)Data[4];
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val5);
    }

    if (Size > 5) {
        int32_t val6 = (int32_t)Data[5];
        aom_codec_control(ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val6);
    }

    // Attempt to encode a frame to exercise more code paths
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);

    // Corrected call: aom_codec_encode takes 5 arguments
    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}