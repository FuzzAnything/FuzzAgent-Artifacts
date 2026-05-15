// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"

static vpx_image_t *create_test_image(unsigned int width, unsigned int height, 
                                      vpx_img_fmt_t fmt, unsigned int bit_depth) {
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    // Fill with dummy data
    for (int plane = 0; plane < 4; plane++) {
        if (img->planes[plane]) {
            size_t plane_size = img->stride[plane] * 
                               (plane == 0 ? height : (height >> img->y_chroma_shift));
            memset(img->planes[plane], 128, plane_size);
        }
    }
    
    return img;
}

static void cleanup_encoder(vpx_codec_ctx_t *codec, vpx_image_t *img) {
    if (codec) {
        vpx_codec_destroy(codec);
    }
    if (img) {
        vpx_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
    if (size < 100) return 0; // Need minimum data
    
    // Initialize encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;
    
    // Setup configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Use fuzz data to modify configuration
    cfg.g_w = 64 + (data[0] % 192);  // 64-256 width
    cfg.g_h = 64 + (data[1] % 192);  // 64-256 height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30 + (data[2] % 60);
    cfg.rc_target_bitrate = 256 + (data[3] * 10);
    cfg.g_pass = (data[4] & 1) ? VPX_RC_ONE_PASS : VPX_RC_FIRST_PASS;
    cfg.g_error_resilient = data[5] & 3;
    cfg.kf_mode = (data[6] & 1) ? VPX_KF_AUTO : VPX_KF_DISABLED;
    cfg.kf_min_dist = data[7];
    cfg.kf_max_dist = 100 + data[8];
    cfg.rc_end_usage = (data[9] % 4);
    
    // Initialize encoder
    vpx_codec_ctx_t codec;
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, 
                                                 VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test vpx_codec_control_ with various control IDs
    int control_ids[] = {
        VP8E_SET_CPUUSED,
        VP8E_SET_ENABLEAUTOALTREF,
        VP8E_SET_SHARPNESS,
        VP8E_SET_STATIC_THRESHOLD,
        VP8E_SET_NOISE_SENSITIVITY,
    };
    
    for (int i = 0; i < sizeof(control_ids)/sizeof(control_ids[0]) && i < 5; i++) {
        int value = data[10 + i] % 10;
        vpx_codec_control_(&codec, control_ids[i], value);
    }
    
    // Create test image
    vpx_image_t *img = create_test_image(cfg.g_w, cfg.g_h, VPX_IMG_FMT_I420, 8);
    if (!img) {
        cleanup_encoder(&codec, NULL);
        return 0;
    }
    
    // Encode frames using fuzz data
    size_t data_offset = 20;
    int frame_count = 0;
    vpx_codec_pts_t pts = 0;
    
    while (data_offset < size && frame_count < 10) {
        // Use fuzz data to determine frame type
        vpx_enc_frame_flags_t flags = 0;
        if (data[data_offset % size] & 1) {
            // Use VPX_EFLAG_FORCE_KF instead of VP8_EFLAG_FORCE_KF
            flags |= VPX_EFLAG_FORCE_KF;
        }
        
        // Encode frame
        err = vpx_codec_encode(&codec, img, pts, 1, flags, VPX_DL_REALTIME);
        if (err != VPX_CODEC_OK) {
            break;
        }
        
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process different packet types
            switch (pkt->kind) {
                case VPX_CODEC_CX_FRAME_PKT:
                    // Access frame data to trigger potential bugs
                    if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                        volatile uint8_t dummy = *((uint8_t*)pkt->data.frame.buf);
                        (void)dummy;
                    }
                    break;
                case VPX_CODEC_STATS_PKT:
                    if (pkt->data.twopass_stats.buf && pkt->data.twopass_stats.sz > 0) {
                        volatile uint8_t dummy = *((uint8_t*)pkt->data.twopass_stats.buf);
                        (void)dummy;
                    }
                    break;
                default:
                    break;
            }
        }
        
        pts += 30; // Increment PTS
        frame_count++;
        data_offset += 10;
        
        // Occasionally reconfigure encoder
        if (frame_count == 5 && (data[data_offset % size] & 1)) {
            cfg.rc_target_bitrate = 512 + (data[data_offset % size] * 5);
            vpx_codec_enc_config_set(&codec, &cfg);
        }
    }
    
    // Flush encoder
    err = vpx_codec_encode(&codec, NULL, pts, 1, 0, VPX_DL_REALTIME);
    if (err == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Access packet data
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT && pkt->data.frame.buf) {
                volatile uint8_t dummy = *((uint8_t*)pkt->data.frame.buf);
                (void)dummy;
            }
        }
    }
    
    // Test global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        volatile uint8_t dummy = *((uint8_t*)global_headers->buf);
        (void)dummy;
    }
    
    cleanup_encoder(&codec, img);
    return 0;
}