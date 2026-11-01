// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_ARNR_TYPE__value at vp8cx.h:1004:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_ARNR_MAXFRAMES__value at vp8cx.h:1000:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    int ret;
    vpx_image_t img = {0}; // Initialize to zero
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    int success = 0;

    // Initialize encoder configuration
    iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Set some basic config from fuzz input if possible
    if (Size > 0) {
        cfg.g_timebase.num = Data[0] % 100 + 1;
        cfg.g_timebase.den = (Data[0] % 100) + 1;
        cfg.g_threads = (Data[0] % 4) + 1;
        cfg.g_w = 160;
        cfg.g_h = 120;
        cfg.g_error_resilient = 1;
    }

    // Initialize encoder
    ret = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // Create a dummy input frame
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 160, 120, 1) != VPX_CODEC_OK) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    memset(img.planes[0], 0, img.stride[0] * img.d_h);
    memset(img.planes[1], 128, img.stride[1] * (img.d_h / 2));
    memset(img.planes[2], 128, img.stride[2] * (img.d_h / 2));

    // Encode one frame
    ret = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_img_free(&img);
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fuzz control parameters using fuzz data
    // VP8E_SET_CPUUSED (0-8)
    if (Size >= 1) {
        int cpu_used = Data[0] % 9;
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
    }

    // VP8E_SET_ENABLEAUTOALTREF (0 or 1)
    if (Size >= 2) {
        int auto_alt_ref = Data[1] % 2;
        vpx_codec_control(&ctx, VP8E_SET_ENABLEAUTOALTREF, auto_alt_ref);
    }

    // VP8E_SET_NOISE_SENSITIVITY (0-63)
    if (Size >= 3) {
        int noise_sens = Data[2] % 64;
        vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sens);
    }

    // VP8E_SET_ARNR_STRENGTH (0-4)
    if (Size >= 4) {
        int arnr_strength = Data[3] % 5;
        vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    }

    // VP8E_SET_ARNR_MAXFRAMES (0-4)
    if (Size >= 5) {
        int arnr_maxframes = Data[4] % 5;
        vpx_codec_control(&ctx, VP8E_SET_ARNR_MAXFRAMES, arnr_maxframes);
    }

    // VP8E_SET_ARNR_TYPE (0-2)
    if (Size >= 6) {
        int arnr_type = Data[5] % 3;
        vpx_codec_control(&ctx, VP8E_SET_ARNR_TYPE, arnr_type);
    }

    // Encode another frame to exercise new settings
    ret = vpx_codec_encode(&ctx, &img, 1, 1, 0, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_img_free(&img);
        cleanup_encoder(&ctx);
        return 0;
    }

    // Drain remaining frames
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Do nothing with output, just consume
        }
    }

    // Cleanup
    vpx_img_free(&img);
    cleanup_encoder(&ctx);
    return 0;
}