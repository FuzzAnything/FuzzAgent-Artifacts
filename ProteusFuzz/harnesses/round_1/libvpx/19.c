// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static int fuzz_encode(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    vpx_image_t *img = NULL;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (iface == NULL) {
        return 0;
    }

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters with bounds checking
    if (size >= 4) {
        cfg.g_w = (data[0] % 128) + 16;
        cfg.g_h = (data[1] % 128) + 16;
        cfg.g_timebase.num = (data[2] % 30) + 1;
        cfg.g_timebase.den = (data[3] % 30) + 1;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
    }

    if (size >= 5) {
        cfg.rc_target_bitrate = (data[4] % 1000) + 100;
    } else {
        cfg.rc_target_bitrate = 100;
    }

    // Initialize encoder
    err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Set max intra bitrate percentage (VP8 specific control)
    int intra_pct = (size > 5) ? (data[5] % 200) + 1 : 100;
    err = vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, intra_pct);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Set frame flags
    int frame_flags = (size > 6) ? (data[6] % 4) : 0;
    err = vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, frame_flags);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Set temporal layer ID
    int layer_id = (size > 7) ? (data[7] % 4) : 0;
    err = vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Allocate image
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzzing data
    if (size > 8) {
        size_t img_size = img->d_w * img->d_h * 3 / 2;
        size_t copy_size = (size - 8) < img_size ? (size - 8) : img_size;
        memcpy(img->planes[VPX_PLANE_Y], data + 8, copy_size);
    }

    // Encode frame
    err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Get encoded data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
    }

    // Flush encoder
    err = vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Get remaining encoded data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
    }

    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_encode(Data, Size);

    return 0;
}