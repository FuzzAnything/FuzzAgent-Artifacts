// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - AV1E_SET_ENABLE_QM
 * - AV1E_SET_INTRA_DCT_ONLY
 * - AV1E_SET_LOSSLESS
 * - AV1E_SET_FORCE_VIDEO_MODE
 * - AV1E_SET_ENABLE_CDEF
 * - AV1E_SET_ENABLE_KEYFRAME_FILTERING
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Get the AV1 encoder interface using the correct API
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzz data to vary configuration slightly
    cfg.g_w = (size > 0 && data[0] > 0) ? (data[0] & 0x3F) + 16 : 16; // Min 16
    cfg.g_h = (size > 1 && data[1] > 0) ? (data[1] & 0x3F) + 16 : 16; // Min 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (size > 2 && data[2] > 0) ? (data[2] % 60) + 1 : 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
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

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // We will try to invoke various control commands using the fuzz data
    // to determine which command to send and what value to use.

    uint8_t cmd_idx = (Size > 0) ? Data[0] % 6 : 0;
    int value = (Size > 1) ? (Data[1] % 2) : 0; // 0 or 1 for boolean flags

    aom_codec_err_t res = AOM_CODEC_OK;

    switch (cmd_idx) {
        case 0: // AV1E_SET_ENABLE_QM
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, value);
            break;
        case 1: // AV1E_SET_INTRA_DCT_ONLY
            res = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, value);
            break;
        case 2: // AV1E_SET_LOSSLESS
            res = aom_codec_control(ctx, AV1E_SET_LOSSLESS, value);
            break;
        case 3: // AV1E_SET_FORCE_VIDEO_MODE
            res = aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, value);
            break;
        case 4: // AV1E_SET_ENABLE_CDEF
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, value);
            break;
        case 5: // AV1E_SET_ENABLE_KEYFRAME_FILTERING
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, value);
            break;
        default:
            break;
    }

    // Optionally, try to encode a frame to ensure the state is valid
    if (res == AOM_CODEC_OK) {
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
        memset(img.planes[0], 128, 16 * 16);
        memset(img.planes[1], 128, 8 * 8);
        memset(img.planes[2], 128, 8 * 8);

        aom_codec_frame_flags_t flags = 0;
        aom_codec_encode(ctx, &img, 0, 1, flags);

        aom_img_free(&img);
    }

    cleanup_encoder(ctx);
    return 0;
}