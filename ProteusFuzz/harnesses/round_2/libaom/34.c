// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_DISABLE_TRELLIS_QUANT
 * - AV1E_SET_ENABLE_TPL_MODEL
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 * - AV1E_SET_FRAME_PERIODIC_BOOST
 * - AV1E_SET_MAX_GF_INTERVAL
 * - AV1E_SET_GF_MIN_PYRAMID_HEIGHT
 */

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Use fuzzing data to configure width/height within reasonable bounds
    int width = 16;
    int height = 16;
    
    if (size >= 2) {
        width = (data[0] % 1024) + 16;
        height = (data[1] % 1024) + 16;
    } else if (size == 1) {
        width = (data[0] % 1024) + 16;
    }

    // Ensure dimensions are multiples of 16 (AV1 requirement)
    width = (width / 16) * 16;
    height = (height / 16) * 16;
    if (width < 16) width = 16;
    if (height < 16) height = 16;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Extract values from fuzz data to drive the control parameters
    int val1 = (Size > 0) ? (Data[0] % 2) : 0; // 0 or 1
    int val2 = (Size > 1) ? (Data[1] % 2) : 0; // 0 or 1
    int val3 = (Size > 2) ? (Data[2] % 2) : 0; // 0 or 1
    int val4 = (Size > 3) ? (Data[3] % 100) : 0; // 0-99
    int val5 = (Size > 4) ? (Data[4] % 128) + 1 : 1; // 1-128
    int val6 = (Size > 5) ? (Data[5] % 8) : 0; // 0-7

    // 1. AV1E_SET_DISABLE_TRELLIS_QUANT (int)
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val1);

    // 2. AV1E_SET_ENABLE_TPL_MODEL (int)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val2);

    // 3. AV1E_SET_ENABLE_REF_FRAME_MVS (int)
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val3);

    // 4. AV1E_SET_FRAME_PERIODIC_BOOST (int)
    aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val4);

    // 5. AV1E_SET_MAX_GF_INTERVAL (int)
    aom_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val5);

    // 6. AV1E_SET_GF_MIN_PYRAMID_HEIGHT (int)
    aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, val6);

    // Attempt to encode a dummy frame to trigger internal state usage
    // Create a minimal image
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 
                  ctx.config.enc->g_w, ctx.config.enc->g_h, 16);
    
    // Fill with pattern based on fuzz data
    // img.planes[y] is a pointer to the start of the plane data
    // img.stride[y] is the stride (width in bytes) of the plane
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            img.planes[0][i * img.stride[0] + j] = Data[(i * img.w + j) % Size];
        }
    }
    
    aom_enc_frame_flags_t flags = 0;
    if (Size > 6) flags = Data[6];
    
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Cleanup image
    aom_img_free(&img);

    // Cleanup encoder
    cleanup_encoder(&ctx);

    return 0;
}