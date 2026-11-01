// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int width, height;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return NULL;

    if (size >= 4) {
        width = data[0] + 1;
        height = data[1] + 1;
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_timebase.num = data[2] + 1;
        cfg.g_timebase.den = data[3] + 1;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_189(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_MAX_GF_INTERVAL
    int max_gf_interval = 0;
    if (Size > 0) max_gf_interval = Data[0];
    aom_codec_control(ctx, AV1E_SET_MAX_GF_INTERVAL, max_gf_interval);

    // Fuzz AV1E_SET_ERROR_RESILIENT_MODE
    int error_resilient = 0;
    if (Size > 1) error_resilient = Data[1] % 3; // 0, 1, or 2
    aom_codec_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient);

    // Fuzz AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    int min_pyramid_height = 0;
    if (Size > 2) min_pyramid_height = Data[2] % 4;
    aom_codec_control(ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, min_pyramid_height);

    // Fuzz AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    int max_pyramid_height = 0;
    if (Size > 3) max_pyramid_height = Data[3] % 4;
    aom_codec_control(ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, max_pyramid_height);

    // Fuzz AV1E_SET_MIN_GF_INTERVAL
    int min_gf_interval = 0;
    if (Size > 4) min_gf_interval = Data[4];
    aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, min_gf_interval);

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    int max_partition_size = 0;
    if (Size > 5) max_partition_size = Data[5] * 16; // Scale up for variety
    aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, max_partition_size);

    // Attempt to encode a frame to exercise state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, ctx->config.enc->g_w, ctx->config.enc->g_h, 16);
    
    // Fill image with fuzz data if available
    if (Size > 10) {
        for (int i = 0; i < ctx->config.enc->g_w * ctx->config.enc->g_h; i++) {
            img.planes[0][i] = Data[i % Size];
        }
    }

    aom_codec_frame_flags_t flags = 0;
    if (Size > 6) flags = Data[6];
    
    aom_codec_encode(ctx, &img, 0, 1, flags);

    aom_img_free(&img);
    cleanup_encoder(ctx);

    return 0;
}