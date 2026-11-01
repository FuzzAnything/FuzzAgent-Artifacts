// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static int fuzz_encode(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = NULL;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();

    if (!enc_iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(enc_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Safe access to input data
    uint8_t w_val = 16;
    uint8_t h_val = 16;
    uint8_t intra_pct_val = 100;
    uint8_t frame_flags_val = 0;
    uint8_t layer_id_val = 0;

    if (Size > 0) w_val = Data[0];
    if (Size > 1) h_val = Data[1];
    if (Size > 2) intra_pct_val = Data[2];
    if (Size > 3) frame_flags_val = Data[3];
    if (Size > 4) layer_id_val = Data[4];

    cfg.g_w = (w_val > 0 && w_val <= 4096) ? w_val : 16;
    cfg.g_h = (h_val > 0 && h_val <= 4096) ? h_val : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.rc_2pass_vbr_bias_pct = 50;

    if (vpx_codec_enc_init(&ctx, enc_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Invoke VP8E_SET_MAX_INTRA_BITRATE_PCT__value
    int intra_pct = intra_pct_val % 101; // 0-100
    vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, intra_pct);

    // Invoke VP8E_SET_FRAME_FLAGS__value
    int frame_flags = frame_flags_val;
    vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, frame_flags);

    // Invoke VP8E_SET_TEMPORAL_LAYER_ID__value
    int layer_id = layer_id_val % 4; // Assuming 4 layers max
    vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);

    // Create image for encoding
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzz data safely
    if (Size > 0) {
        size_t plane_size = (size_t)img->planes[0] ? (size_t)img->stride[0] * (size_t)img->h : 0;
        size_t total_needed = plane_size * 3; // Y, U, V
        size_t copy_size = (Size < total_needed) ? Size : total_needed;
        
        if (img->planes[0] && copy_size > 0) {
            memcpy(img->planes[0], Data, copy_size);
        }
        if (img->planes[1] && copy_size > plane_size) {
            size_t u_size = (copy_size > plane_size) ? (copy_size - plane_size) : 0;
            memcpy(img->planes[1], Data + plane_size, u_size);
        }
        if (img->planes[2] && copy_size > plane_size * 2) {
            size_t v_size = (copy_size > plane_size * 2) ? (copy_size - plane_size * 2) : 0;
            memcpy(img->planes[2], Data + (plane_size * 2), v_size);
        }
    }

    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0; // Infinite deadline for fuzzing

    // Encode one frame
    if (vpx_codec_encode(&ctx, img, pts, duration, flags, deadline) == VPX_CODEC_OK) {
        // Get output data
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process packet if needed, here we just iterate
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Do nothing with the data, just consume it
            }
        }
    }

    // Flush the encoder
    if (vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline) == VPX_CODEC_OK) {
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Process flush packets
        }
    }

    vpx_codec_destroy(&ctx);
    vpx_img_free(img);
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    fuzz_encode(Data, Size);
    return 0;
}