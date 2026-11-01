// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *encoder = vpx_codec_vp9_cx();
    if (!encoder) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure encoder settings safely
    cfg.g_w = Size > 0 ? (Data[0] % 4096) + 16 : 16;
    cfg.g_h = Size > 1 ? (Data[1] % 4096) + 16 : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = Size > 2 ? (Data[2] % 10000) + 100 : 100;
    cfg.g_lag_in_frames = Size > 3 ? (Data[3] % 20) : 0;

    if (vpx_codec_enc_init(&ctx, encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Create a dummy frame to send to the encoder
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);

    // Fill image planes with fuzzing data if available
    // Note: vpx_image uses 'planes' not 'plane'
    if (Size >= 3) {
        size_t plane_size = (cfg.g_w * cfg.g_h) / 2;
        size_t copy_len = (Size < plane_size) ? Size : plane_size;
        
        // Copy data to Y plane
        if (img.planes[0]) {
            memcpy(img.planes[0], Data, copy_len);
        }
        // Copy data to U plane (offset by copy_len if data is large enough)
        if (img.planes[1] && Size > copy_len) {
            size_t remaining = Size - copy_len;
            size_t u_copy = (remaining < plane_size) ? remaining : plane_size;
            memcpy(img.planes[1], Data + copy_len, u_copy);
        }
        // Copy data to V plane
        if (img.planes[2] && Size > copy_len * 2) {
            size_t remaining = Size - copy_len * 2;
            size_t v_copy = (remaining < plane_size) ? remaining : plane_size;
            memcpy(img.planes[2], Data + copy_len * 2, v_copy);
        }
    }

    // Encode the frame
    // Note: VPX_EFLAG_NONE is typically 0 or defined in vpx_encoder.h, using 0 directly if not found
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    // Drain the encoder
    vpx_codec_iter_t iter = NULL;
    vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process output frame if needed
            (void)pkt;
        }
    }

    vpx_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}