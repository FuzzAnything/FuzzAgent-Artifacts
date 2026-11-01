// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
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
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_image.h>

static int fuzz_encode(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    vpx_image_t *img = NULL;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    const vpx_codec_iface_t *iface;

    // Get the VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Fuzz configuration parameters based on input data
    // Ensure dimensions are valid and not too large to avoid allocation failures
    unsigned int w = 16, h = 16;
    if (Size > 0) w = (Data[0] % 512) + 16;
    if (Size > 1) h = (Data[1] % 512) + 16;
    
    cfg.g_w = w;
    cfg.g_h = h;
    
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    if (Size > 5) cfg.g_error_resilient = (Data[5] % 3);
    if (Size > 6) cfg.g_lag_in_frames = (Data[6] % 20);

    // Initialize the encoder
    err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Create an image to encode
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzz data safely
    // Calculate exact sizes needed for each plane
    size_t y_size = (size_t)img->d_w * img->d_h;
    size_t uv_size = (size_t)img->d_w * img->d_h / 4;
    size_t total_needed = y_size + 2 * uv_size;
    
    // Use available data or zero-fill if insufficient
    size_t available = Size;
    
    // Fill Y plane
    if (available > 0) {
        size_t copy_y = (available < y_size) ? available : y_size;
        memcpy(img->planes[VPX_PLANE_Y], Data, copy_y);
        if (copy_y < y_size) {
            memset(img->planes[VPX_PLANE_Y] + copy_y, 0, y_size - copy_y);
        }
        available -= copy_y;
    } else {
        memset(img->planes[VPX_PLANE_Y], 0, y_size);
    }

    // Fill U plane
    if (available > 0) {
        size_t copy_u = (available < uv_size) ? available : uv_size;
        memcpy(img->planes[VPX_PLANE_U], Data + (Size - available), copy_u);
        if (copy_u < uv_size) {
            memset(img->planes[VPX_PLANE_U] + copy_u, 0, uv_size - copy_u);
        }
        available -= copy_u;
    } else {
        memset(img->planes[VPX_PLANE_U], 0, uv_size);
    }

    // Fill V plane
    if (available > 0) {
        size_t copy_v = (available < uv_size) ? available : uv_size;
        memcpy(img->planes[VPX_PLANE_V], Data + (Size - available), copy_v);
        if (copy_v < uv_size) {
            memset(img->planes[VPX_PLANE_V] + copy_v, 0, uv_size - copy_v);
        }
    } else {
        memset(img->planes[VPX_PLANE_V], 0, uv_size);
    }

    // Invoke VP8E_SET_MAX_INTRA_BITRATE_PCT__value (using vpx_codec_control)
    int max_intra_pct = (Size > 7) ? (Data[7] % 100) : 50;
    vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct);

    // Invoke VP8E_SET_FRAME_FLAGS__value
    int frame_flags = (Size > 8) ? (Data[8] % 4) : 0;
    vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, frame_flags);

    // Invoke VP8E_SET_TEMPORAL_LAYER_ID__value
    int temporal_layer_id = (Size > 9) ? (Data[9] % 4) : 0;
    vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, temporal_layer_id);

    // Encode the frame
    vpx_codec_pts_t pts = (Size > 10) ? (Data[10] % 1000) : 0;
    unsigned long duration = (Size > 11) ? (Data[11] % 100) + 1 : 1;
    vpx_enc_frame_flags_t flags = (Size > 12) ? (Data[12] % 2) : 0;
    vpx_enc_deadline_t deadline = (Size > 13) ? (Data[13] % 1000000) : 0;

    err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Retrieve encoded data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed
        }
    }

    // Flush the encoder
    err = vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Retrieve any remaining encoded data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed
        }
    }

    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzz_encode(Data, Size);
}