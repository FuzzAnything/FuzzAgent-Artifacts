// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    const aom_codec_cx_pkt_t *pkt;
    aom_codec_err_t res;

    // Initialize encoder config
    aom_codec_enc_config_default(NULL, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) return 0;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Prepare a dummy image frame
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    // Fill image with fuzz data if available
    if (Size > 0) {
        size_t plane_size = (size_t)img.stride[0] * (size_t)img.h;
        size_t copy_size = (Size > plane_size) ? plane_size : Size;
        memcpy(img.planes[0], Data, copy_size);
    }

    // Convert fuzz data to control parameters
    int param_val = (Size > 0) ? (Data[0] % 2) : 0; // 0 or 1 for boolean-like params
    int int_param = (Size > 1) ? (Data[1] % 100) : 0; // 0-99 for integer params

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, param_val);
    if (res != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_RESTORATION, param_val);
    if (res != AOM_CODEC_OK) {
        // Ignore error
    }

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, param_val);
    if (res != AOM_CODEC_OK) {
        // Ignore error
    }

    // Fuzz AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, param_val);
    if (res != AOM_CODEC_OK) {
        // Ignore error
    }

    // Fuzz AOME_SET_SHARPNESS (integer value)
    res = aom_codec_control(ctx, AOME_SET_SHARPNESS, int_param);
    if (res != AOM_CODEC_OK) {
        // Ignore error
    }

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ (integer value)
    res = aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, int_param);
    if (res != AOM_CODEC_OK) {
        // Ignore error
    }

    // Encode one frame to exercise the state
    res = aom_codec_encode(ctx, &img, 0, 1, 0);
    if (res == AOM_CODEC_OK) {
        while ((pkt = aom_codec_get_cx_data(ctx, NULL)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process packet if needed
            }
        }
    }

    cleanup_codec(ctx);
    aom_img_free(&img);

    return 0;
}