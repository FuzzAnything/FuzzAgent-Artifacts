// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_INTRA_DCT_ONLY
 * - AV1E_SET_FORCE_VIDEO_MODE
 * - AV1E_SET_ENABLE_REF_FRAME_MVS
 * - AV1E_SET_COLOR_PRIMARIES
 * - AV1E_SET_ENABLE_SUPERRES
 * - AV1E_SET_ENABLE_CHROMA_DELTAQ
 */

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder_from_data(aom_codec_ctx_t **ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 4) {
        return 0;
    }

    aom_codec_ctx_t *c = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!c) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(c);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(c);
        return 0;
    }

    // Derive width/height from input data to vary states
    unsigned int width = (data[0] % 64) + 16;
    unsigned int height = (data[1] % 64) + 16;
    // Ensure alignment
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 30) + 30;
    cfg.g_threads = (data[3] % 4) + 1;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    aom_codec_err_t res = aom_codec_enc_init(c, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(c);
        return 0;
    }

    *ctx = c;
    return 1;
}

int LLVMFuzzerTestOneInput_205(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    if (!init_encoder_from_data(&ctx, Data, Size)) {
        return 0;
    }

    // Extract parameters from fuzz input
    int intra_dct_only = (Data[0] & 0x01);
    int force_video_mode = (Data[1] & 0x01);
    int enable_ref_frame_mvs = (Data[2] & 0x01);
    int enable_superres = (Data[3] & 0x01);
    
    // Fixed ternary operator syntax error
    int enable_chroma_deltaq = (Size > 4) ? (Data[4] & 0x01) : 0;
    
    // Color primaries: AOM_CP_BT709, AOM_CP_SMPTE170M, etc.
    // Clamp to a safe range [0, 10] to avoid invalid enum values
    int color_primaries = (Size > 5) ? (Data[5] % 11) : 0;

    // Invoke control functions
    // 1. AV1E_SET_INTRA_DCT_ONLY
    aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);

    // 2. AV1E_SET_FORCE_VIDEO_MODE
    aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);

    // 3. AV1E_SET_ENABLE_REF_FRAME_MVS
    aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, enable_ref_frame_mvs);

    // 4. AV1E_SET_COLOR_PRIMARIES
    aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);

    // 5. AV1E_SET_ENABLE_SUPERRES
    aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, enable_superres);

    // 6. AV1E_SET_ENABLE_CHROMA_DELTAQ
    aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_chroma_deltaq);

    // Optional: Trigger a single encode frame to exercise state
    // Create a dummy image
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 
                  (unsigned int)(ctx->config.enc->g_w), 
                  (unsigned int)(ctx->config.enc->g_h), 16);
    
    // Fill with fuzz data if available
    if (Size > 10) {
        // img.planes[0] is uint8_t*, img.stride[0] is int.
        size_t correct_plane_size = (size_t)img.stride[0] * img.h;
        size_t copy_size = (Size - 10) < correct_plane_size ? (Size - 10) : correct_plane_size;
        memcpy(img.planes[0], Data + 10, copy_size);
    }

    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);

    cleanup_encoder(ctx);
    return 0;
}