// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 */

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    
    // Use the standard interface getter for AV1 encoder
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) {
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    // Use fuzzing data to configure encoder settings randomly
    if (size > 0) cfg.g_w = (data[0] % 128) + 16;
    if (size > 1) cfg.g_h = (data[1] % 128) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 60) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 60) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 10000) + 100;
    if (size > 5) cfg.g_error_resilient = data[5] % 2;
    
    // Ensure valid dimensions
    if (cfg.g_w < 16) cfg.g_w = 16;
    if (cfg.g_h < 16) cfg.g_h = 16;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Prepare a dummy frame to ensure the encoder is in a valid state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, ctx->config.enc->g_w, ctx->config.enc->g_h, 1);
    
    // Fill image with fuzzing data
    if (Size > 0) {
        size_t plane_size = img.planes[0] ? img.planes[0][0] * img.stride[0] : 0;
        if (plane_size == 0) plane_size = ctx->config.enc->g_w * ctx->config.enc->g_h; // Fallback calc
        size_t copy_size = (Size > plane_size) ? plane_size : Size;
        if (img.planes[0]) {
            memcpy(img.planes[0], Data, copy_size);
        }
    }

    // Apply control functions using the standard aom_codec_control interface
    // with the specific AV1E_* control IDs.
    
    // 1. AV1E_SET_QM_V
    int qm_v_val = (Size > 10) ? (Data[10] << 8 | Data[11]) : 0;
    aom_codec_control(ctx, AV1E_SET_QM_V, &qm_v_val);

    // 2. AV1E_SET_INTER_DCT_ONLY
    int inter_dct_only = (Size > 12) ? (Data[12] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, &inter_dct_only);

    // 3. AV1E_SET_QUANT_B_ADAPT
    int quant_b_adapt = (Size > 13) ? (Data[13] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_QUANT_B_ADAPT, &quant_b_adapt);

    // 4. AV1E_SET_ENABLE_ANGLE_DELTA
    int enable_angle_delta = (Size > 14) ? (Data[14] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, &enable_angle_delta);

    // 5. AV1E_SET_ENABLE_FLIP_IDTX
    int enable_flip_idtx = (Size > 15) ? (Data[15] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, &enable_flip_idtx);

    // 6. AV1E_SET_ENABLE_DIFF_WTD_COMP
    int enable_diff_wtd_comp = (Size > 16) ? (Data[16] % 2) : 0;
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, &enable_diff_wtd_comp);

    // Try to encode a frame to trigger any latent issues from the configuration changes
    aom_codec_encode(ctx, &img, 0, 1, 0);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}