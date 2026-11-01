// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static void cleanup_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_222(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_fixed_buf_t *global_headers = NULL;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.g_threads = (Data[3] % 8) + 1;
    cfg.g_error_resilient = (Data[4] % 3);
    cfg.rc_target_bitrate = (Data[5] % 100) + 1000;

    // Initialize encoder context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Encode a single frame to ensure context is fully configured
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], Data[6] % 256, cfg.g_w * cfg.g_h);
    memset(img.planes[1], Data[7] % 256, (cfg.g_w * cfg.g_h) / 4);
    memset(img.planes[2], Data[8] % 256, (cfg.g_w * cfg.g_h) / 4);

    aom_enc_frame_flags_t flags = 0;
    if (Data[9] & 0x01) flags |= AOM_EFLAG_FORCE_KF;
    // AOM_EFLAG_LIMIT_FRAMES is not a valid flag in current libaom, removed

    ret = aom_codec_encode(&ctx, &img, 0, 1, flags);
    if (ret == AOM_CODEC_OK) {
        // Retrieve global headers
        global_headers = aom_codec_get_global_headers(&ctx);
        if (global_headers != NULL) {
            // Use the buffer to trigger potential memory issues
            if (global_headers->sz > 0 && global_headers->buf != NULL) {
                volatile uint8_t *ptr = global_headers->buf;
                (void)ptr[0]; // Read first byte
            }
            // Free the buffer as required by API
            free(global_headers->buf);
            free(global_headers);
        }
    }

    aom_img_free(&img);
    cleanup_context(&ctx);

    return 0;
}