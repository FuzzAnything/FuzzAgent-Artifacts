// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static vpx_image_t *create_test_image(int width, int height, vpx_img_fmt_t fmt) {
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    // Fill image planes with dummy data
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            int plane_width = (plane == 0) ? width : (width >> img->x_chroma_shift);
            int plane_height = (plane == 0) ? height : (height >> img->y_chroma_shift);
            size_t plane_size = plane_width * plane_height;
            for (size_t i = 0; i < plane_size; i++) {
                img->planes[plane][i] = (uint8_t)(i % 256);
            }
        }
    }
    return img;
}

static void init_encoder_config(vpx_codec_enc_cfg_t *cfg, vpx_codec_iface_t *iface) {
    vpx_codec_enc_config_default(iface, cfg, 0);
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 30;
}

static void fuzz_control_functions(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    // Extract control ID and value from fuzz data
    int ctrl_id = *(const int *)data;
    int value = *(const int *)(data + 4);
    
    // Try the control function with fuzzed parameters
    vpx_codec_control_(ctx, ctrl_id, value);
}

int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
    if (size < 32) return 0;
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    if (!encoder_iface) return 0;
    
    init_encoder_config(&cfg, encoder_iface);
    
    // Use fuzz data to modify some config parameters
    if (size >= 40) {
        cfg.g_w = 32 + (data[0] % 64);
        cfg.g_h = 32 + (data[1] % 64);
        cfg.rc_target_bitrate = 128 + (data[2] % 1024);
        cfg.rc_min_quantizer = data[3] % 64;
        cfg.rc_max_quantizer = 10 + (data[4] % 54);
    }
    
    vpx_codec_err_t res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) return 0;
    
    // Create test image
    vpx_image_t *img = create_test_image(cfg.g_w, cfg.g_h, VPX_IMG_FMT_I420);
    if (!img) {
        vpx_codec_destroy(&encoder);
        return 0;
    }
    
    // Encode frames using fuzz data to determine parameters
    size_t data_offset = 0;
    vpx_codec_pts_t pts = 0;
    
    for (int i = 0; i < 3; i++) {
        if (data_offset + 8 > size) break;
        
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
        
        // Extract parameters from fuzz data
        if (size - data_offset >= 12) {
            duration = 1 + (*(const uint32_t *)(data + data_offset) % 4);
            data_offset += 4;
            flags = *(const uint32_t *)(data + data_offset) & 0xFF;
            data_offset += 4;
            deadline = (*(const uint32_t *)(data + data_offset) % 3) * 1000000;
            data_offset += 4;
        }
        
        // Alternate between encoding image and flushing
        const vpx_image_t *encode_img = (i < 2) ? img : NULL;
        
        res = vpx_codec_encode(&encoder, encode_img, pts, duration, flags, deadline);
        if (res != VPX_CODEC_OK) {
            // Continue anyway
        }
        
        pts += duration;
        
        // Get encoded data
        const vpx_codec_cx_pkt_t *pkt;
        vpx_codec_iter_t iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            // Process different packet types
            switch (pkt->kind) {
                case VPX_CODEC_CX_FRAME_PKT:
                    // Could decode this frame
                    break;
                case VPX_CODEC_STATS_PKT:
                    // Statistics packet
                    break;
                default:
                    break;
            }
        }
        
        // Try control functions with remaining fuzz data
        if (size - data_offset >= 8) {
            fuzz_control_functions(&encoder, data + data_offset, size - data_offset);
            data_offset += 8;
        }
    }
    
    // Try reconfiguring encoder
    if (size >= 48) {
        cfg.rc_target_bitrate = 512 + (data[5] % 1024);
        cfg.g_error_resilient = data[6] & 1;
        vpx_codec_enc_config_set(&encoder, &cfg);
    }
    
    // Create decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    if (decoder_iface) {
        vpx_codec_dec_init(&decoder, decoder_iface, NULL, 0);
        
        // Encode one more frame to get data for decoding
        res = vpx_codec_encode(&encoder, img, pts, 1, 0, VPX_DL_REALTIME);
        if (res == VPX_CODEC_OK) {
            const vpx_codec_cx_pkt_t *pkt;
            vpx_codec_iter_t iter = NULL;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Decode the frame
                    vpx_codec_decode(&decoder, (const uint8_t *)pkt->data.frame.buf,
                                    pkt->data.frame.sz, NULL, 0);
                    
                    // Flush decoder
                    vpx_codec_decode(&decoder, NULL, 0, NULL, 0);
                }
            }
        }
        
        vpx_codec_destroy(&decoder);
    }
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    
    return 0;
}