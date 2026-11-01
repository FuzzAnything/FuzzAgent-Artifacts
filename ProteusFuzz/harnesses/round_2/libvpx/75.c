// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// VP9E_SET_KEY_FRAME_FILTERING__value at vp8cx.h:1109:1 in vp8cx.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

// Define VP9 control IDs if not present in the included headers
#ifndef VP9E_SET_SVC
#define VP9E_SET_SVC 100
#endif

#ifndef VP9E_SET_TILE_COLUMNS
#define VP9E_SET_TILE_COLUMNS 101
#endif

#ifndef VP9E_SET_KEY_FRAME_FILTERING
#define VP9E_SET_KEY_FRAME_FILTERING 102
#endif

static int fuzz_encode_frame(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size, vpx_codec_pts_t pts) {
    if (ctx == NULL) return 0;

    vpx_image_t img;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    int w = 16;
    int h = 16;
    int bit_depth = 8;

    // Fuzz dimensions based on input data if size allows
    if (size >= 4) {
        uint32_t dims = 0;
        if (size >= 4) {
            memcpy(&dims, data, sizeof(uint32_t));
        }
        w = (dims & 0xFFFF) % 1024;
        if (w < 2) w = 16;
        h = ((dims >> 16) & 0xFFFF) % 1024;
        if (h < 2) h = 16;
    }

    if (vpx_img_alloc(&img, fmt, w, h, 16) != 0) {
        return 0;
    }

    // Fill image planes with fuzz data
    size_t plane_size = img.stride[0] * img.h;
    size_t total_size = plane_size * 3; // Y, U, V
    size_t copy_size = (size < total_size) ? size : total_size;
    
    if (copy_size > 0) {
        size_t y_size = (size < plane_size) ? size : plane_size;
        memcpy(img.planes[VPX_PLANE_Y], data, y_size);
        
        if (size > plane_size) {
            size_t u_size = (size - plane_size < plane_size) ? (size - plane_size) : plane_size;
            memcpy(img.planes[VPX_PLANE_U], data + plane_size, u_size);
        }
        if (size > plane_size * 2) {
            size_t v_size = (size - plane_size * 2 < plane_size) ? (size - plane_size * 2) : plane_size;
            memcpy(img.planes[VPX_PLANE_V], data + plane_size * 2, v_size);
        }
    }

    vpx_codec_err_t err = vpx_codec_encode(ctx, &img, pts, 1, 0, VPX_DL_REALTIME);
    vpx_img_free(&img);

    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Drain output
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Consume the packet
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    // Ensure minimum size to avoid buffer overflows
    if (Size < 20) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    if (iface == NULL) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        return 0;
    }

    // Configure encoder with fuzzed values safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = (Data[4] % 4) + 1;
    cfg.g_profile = (Data[5] % 4);
    cfg.rc_target_bitrate = (Data[6] % 1000) + 100;
    cfg.rc_min_quantizer = (Data[7] % 60) + 10;
    cfg.rc_max_quantizer = (Data[8] % 60) + 10;
    cfg.kf_mode = (Data[9] % 3);
    cfg.kf_min_dist = (Data[10] % 100) + 1;
    cfg.kf_max_dist = (Data[11] % 100) + 1;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != 0) {
        return 0;
    }

    // Fuzz VP9E_SET_SVC
    int svc_val = Data[12];
    vpx_codec_control(&ctx, VP9E_SET_SVC, svc_val);

    // Fuzz VP9E_SET_TILE_COLUMNS
    int tile_cols = Data[13] % 64;
    if (tile_cols < 1) tile_cols = 1;
    vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, tile_cols);

    // Fuzz VP9E_SET_KEY_FRAME_FILTERING
    int kf_filter = Data[14] % 2;
    vpx_codec_control(&ctx, VP9E_SET_KEY_FRAME_FILTERING, kf_filter);

    // Fuzz vpx_codec_enc_config_set with modified config
    cfg.g_threads = (Data[15] % 8) + 1;
    cfg.rc_target_bitrate = (Data[16] % 2000) + 100;
    vpx_codec_enc_config_set(&ctx, &cfg);

    // Encode a frame
    vpx_codec_pts_t pts = 0;
    if (Size >= 25) {
        int64_t pts_val = 0;
        memcpy(&pts_val, Data + 17, sizeof(int64_t));
        pts = pts_val;
    }
    
    fuzz_encode_frame(&ctx, Data + 17, Size - 17, pts);

    // Flush the encoder
    vpx_codec_encode(&ctx, NULL, pts + 1, 1, 0, VPX_DL_REALTIME);
    
    // Drain remaining data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Consume
        }
    }

    vpx_codec_destroy(&ctx);
    return 0;
}