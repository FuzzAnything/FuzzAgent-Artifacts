// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Use fuzzing data to configure width/height if size is sufficient
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)data;
        uint32_t h = *(const uint32_t *)(data + 4);
        if (w > 0 && h > 0 && w <= 8192 && h <= 8192) {
            cfg.g_w = w;
            cfg.g_h = h;
        } else {
            cfg.g_w = 32;
            cfg.g_h = 32;
        }
    } else {
        cfg.g_w = 32;
        cfg.g_h = 32;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Extract values from fuzz input for diversity
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    int val3 = (Size > 2) ? (int)Data[2] : 0;
    int val4 = (Size > 3) ? (int)Data[3] : 0;
    int val5 = (Size > 4) ? (int)Data[4] : 0;
    int val6 = (Size > 5) ? (int)Data[5] : 0;

    // 1. AOME_SET_ARNR_STRENGTH
    aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, val1);

    // 2. AOME_SET_MAX_INTRA_BITRATE_PCT
    aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val2);

    // 3. AV1E_SET_DENOISE_NOISE_LEVEL
    aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val3);

    // 4. AV1E_SET_ENABLE_GLOBAL_MOTION
    aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val4);

    // 5. AOME_SET_ARNR_MAXFRAMES
    aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, val5);

    // 6. AOME_SET_SHARPNESS
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, val6);

    // Try to encode a frame to trigger state changes
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.w = ctx.config.enc->g_w;
    img.h = ctx.config.enc->g_h;
    img.d_w = img.w;
    img.d_h = img.h;
    
    // aom_img_alloc returns 0 on success, not AOM_IMG_OK
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, img.w, img.h, 1) != 0) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fill image with fuzz data if available
    if (Size >= img.w * img.h * 3 / 2) {
        memcpy(img.planes[0], Data, img.w * img.h);
        memcpy(img.planes[1], Data + img.w * img.h, (img.w * img.h) / 4);
        memcpy(img.planes[2], Data + img.w * img.h + (img.w * img.h) / 4, (img.w * img.h) / 4);
    } else {
        memset(img.planes[0], 0, img.w * img.h);
        memset(img.planes[1], 128, (img.w * img.h) / 4);
        memset(img.planes[2], 128, (img.w * img.h) / 4);
    }

    aom_enc_frame_flags_t flags = 0;
    // Fixed: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t pkt;
    // Fixed: aom_codec_get_cx_data takes 2 arguments and returns a pointer
    const aom_codec_cx_pkt_t *pkt_ptr;
    while ((pkt_ptr = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt_ptr->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Do nothing with the output frame
        }
    }

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}