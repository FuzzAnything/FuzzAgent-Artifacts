// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static aom_codec_ctx_t *create_encoder_context(aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_image_t *create_test_image(int width, int height, int bit_depth) {
    aom_image_t *img = (aom_image_t *)malloc(sizeof(aom_image_t));
    if (!img) return NULL;
    memset(img, 0, sizeof(aom_image_t));

    img->fmt = AOM_IMG_FMT_I420;
    img->w = width;
    img->h = height;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    img->bit_depth = bit_depth;
    img->monochrome = 0;
    img->cp = AOM_CICP_CP_UNSPECIFIED;
    img->tc = AOM_CICP_TC_UNSPECIFIED;
    img->mc = AOM_CICP_MC_UNSPECIFIED;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_UNKNOWN;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = bit_depth;

    // Allocate planes
    int stride_y = width;
    int stride_uv = width;
    int plane_size_y = stride_y * height;
    int plane_size_uv = stride_uv * (height / 2);

    img->planes[0] = (unsigned char *)malloc(plane_size_y);
    img->planes[1] = (unsigned char *)malloc(plane_size_uv);
    img->planes[2] = (unsigned char *)malloc(plane_size_uv);

    img->stride[0] = stride_y;
    img->stride[1] = stride_uv;
    img->stride[2] = stride_uv;

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
        return NULL;
    }

    // Initialize with simple pattern
    for (int i = 0; i < plane_size_y; i++) img->planes[0][i] = (i % 256);
    for (int i = 0; i < plane_size_uv; i++) img->planes[1][i] = 128;
    for (int i = 0; i < plane_size_uv; i++) img->planes[2][i] = 128;

    return img;
}

static void cleanup_test_image(aom_image_t *img) {
    if (img) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
    }
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Extract parameters from fuzz data
    int width = (Data[0] % 32) + 16; // 16-48
    int height = (Data[1] % 32) + 16; // 16-48
    int bit_depth = (Data[2] % 3) + 8; // 8, 10, 12
    int usage = Data[3] % 10;
    int threads = (Data[4] % 4) + 1;

    // Initialize encoder config
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, usage) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = (aom_bit_depth_t)bit_depth;
    cfg.g_input_bit_depth = bit_depth;
    cfg.g_threads = threads;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_end_usage = AOM_VBR;
    cfg.rc_target_bitrate = 500;
    cfg.g_limit = 0;

    aom_codec_ctx_t *ctx = create_encoder_context(&cfg);
    if (!ctx) return 0;

    // Test aom_codec_set_option with various inputs
    const char *option_names[] = {"cpu-used", "crf", "arnr-maxframes", "lag-in-frames", "tile-columns"};
    const char *option_values[] = {"0", "10", "5", "10", "1"};
    
    for (int i = 0; i < 5; i++) {
        if (Size > (size_t)(i + 10)) {
            aom_codec_set_option(ctx, option_names[i], option_values[i]);
        }
    }

    // Test aom_codec_enc_config_set
    aom_codec_enc_cfg_t new_cfg = cfg;
    new_cfg.g_threads = (Data[5] % 8) + 1;
    aom_codec_enc_config_set(ctx, &new_cfg);

    // Create test image
    aom_image_t *img = create_test_image(width, height, bit_depth);
    if (!img) {
        cleanup_encoder_context(ctx);
        return 0;
    }

    // Test aom_codec_encode with valid image
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    if (Size > 10) {
        aom_codec_encode(ctx, img, pts, duration, flags);
    }

    // Test aom_codec_encode with NULL image (flush)
    aom_codec_encode(ctx, NULL, 0, 0, 0);

    // Test error handling functions
    const char *err_str = aom_codec_error(ctx);
    (void)err_str; // Suppress unused warning

    const char *err_detail = aom_codec_error_detail(ctx);
    (void)err_detail; // Suppress unused warning

    // Test aom_codec_err_to_string with various error codes
    aom_codec_err_t test_errors[] = {AOM_CODEC_OK, AOM_CODEC_ERROR, AOM_CODEC_INVALID_PARAM, AOM_CODEC_INCAPABLE};
    for (int i = 0; i < 4; i++) {
        const char *err_msg = aom_codec_err_to_string(test_errors[i]);
        (void)err_msg; // Suppress unused warning
    }

    // Test with invalid parameters
    aom_codec_enc_config_set(NULL, &cfg);
    aom_codec_enc_config_set(ctx, NULL);
    aom_codec_error(NULL);
    aom_codec_error_detail(NULL);
    aom_codec_set_option(NULL, "test", "value");
    aom_codec_encode(NULL, img, 0, 1, 0);
    aom_codec_encode(ctx, NULL, 0, 0, 0);

    cleanup_test_image(img);
    cleanup_encoder_context(ctx);

    return 0;
}