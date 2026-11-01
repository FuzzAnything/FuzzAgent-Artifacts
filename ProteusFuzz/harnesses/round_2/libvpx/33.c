// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP8E_GET_LAST_QUANTIZER__value at vp8cx.h:996:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx = {0};
    const vpx_codec_iface_t *encoder_iface = NULL;
    vpx_codec_enc_cfg_t cfg = {0};

    // Determine codec based on input data to diversify states
    int use_vp8 = Data[0] % 2;
    int use_vp9 = (Data[0] % 2) == 0;

    if (use_vp8) {
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        encoder_iface = vpx_codec_vp9_cx();
    }

    if (!encoder_iface) return 0;

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters safely
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = (Data[2] % 30) + 1;
    cfg.g_timebase.den = (Data[3] % 30) + 1;
    
    if (Size > 4) {
        cfg.rc_target_bitrate = (Data[4] % 100) + 100;
    }
    
    if (Size > 5) {
        cfg.g_error_resilient = (Data[5] % 2);
    }

    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare dummy input frame
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    vpx_codec_err_t res;
    vpx_image_t img;

    // vpx_img_alloc returns a pointer on success, NULL on failure.
    // The original code incorrectly checked for != 0, treating success as failure.
    if (!vpx_img_alloc(&img, fmt, cfg.g_w, cfg.g_h, 1)) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data if enough size
    if (Size > 10) {
        size_t plane_size = (size_t)img.stride[0] * img.d_h;
        size_t copy_size = (Size - 10) < plane_size ? (Size - 10) : plane_size;
        memcpy(img.planes[0], Data + 10, copy_size);
    }

    // Encode a frame
    res = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    (void)res;

    // Extract packets (cleanup)
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Consume data
        }
    }

    // Fuzz Control Commands
    // 1. VP9E_SET_COLOR_RANGE (if VP9)
    if (!use_vp8) {
        int color_range = (Data[0] % 2); // 0: Limited, 1: Full
        vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, color_range);
    }

    // 2. VP8E_GET_LAST_QUANTIZER (if VP8)
    if (use_vp8) {
        int quantizer;
        vpx_codec_control(&ctx, VP8E_GET_LAST_QUANTIZER, &quantizer);
    }

    // 3. VP9E_SET_TUNE_CONTENT (if VP9)
    if (!use_vp8) {
        int tune_content = Data[1] % 3; // 0: Screen, 1: Film, 2: Animation
        vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, tune_content);
    }

    // 4. Generic control with variadic args simulation
    if (Size > 6) {
        int dummy_int = Data[6];
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, dummy_int);
    }

    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}