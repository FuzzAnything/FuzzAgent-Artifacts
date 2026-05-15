// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static void init_image(vpx_image_t *img, uint32_t width, uint32_t height, 
                       vpx_img_fmt_t fmt, vpx_color_space_t cs, 
                       vpx_color_range_t range, unsigned int bit_depth) {
    memset(img, 0, sizeof(*img));
    img->fmt = fmt;
    img->cs = cs;
    img->range = range;
    img->w = width;
    img->h = height;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    img->bit_depth = bit_depth;
    
    // Set chroma shift based on format
    if (fmt == VPX_IMG_FMT_I420 || fmt == VPX_IMG_FMT_YV12 || fmt == VPX_IMG_FMT_NV12) {
        img->x_chroma_shift = 1;
        img->y_chroma_shift = 1;
    } else if (fmt == VPX_IMG_FMT_I422) {
        img->x_chroma_shift = 1;
        img->y_chroma_shift = 0;
    } else if (fmt == VPX_IMG_FMT_I444) {
        img->x_chroma_shift = 0;
        img->y_chroma_shift = 0;
    }
    
    // Calculate strides and allocate planes
    unsigned int bps = (bit_depth > 8) ? 2 : 1;
    img->stride[VPX_PLANE_Y] = width * bps;
    img->stride[VPX_PLANE_U] = (width >> img->x_chroma_shift) * bps;
    img->stride[VPX_PLANE_V] = (width >> img->x_chroma_shift) * bps;
    img->stride[VPX_PLANE_ALPHA] = width * bps;
    
    // Allocate image data with proper size calculation
    size_t y_size = img->stride[VPX_PLANE_Y] * height;
    size_t u_size = img->stride[VPX_PLANE_U] * (height >> img->y_chroma_shift);
    size_t v_size = img->stride[VPX_PLANE_V] * (height >> img->y_chroma_shift);
    size_t total_size = y_size + u_size + v_size;
    
    // Ensure minimum allocation
    if (total_size < 1024) {
        total_size = 1024;
    }
    
    img->img_data = (unsigned char*)malloc(total_size);
    if (!img->img_data) {
        return;
    }
    img->img_data_owner = 1;
    img->self_allocd = 1;
    
    // Set plane pointers
    img->planes[VPX_PLANE_Y] = img->img_data;
    img->planes[VPX_PLANE_U] = img->img_data + y_size;
    
    if (fmt == VPX_IMG_FMT_NV12) {
        img->planes[VPX_PLANE_V] = img->planes[VPX_PLANE_U] + 1;
    } else {
        img->planes[VPX_PLANE_V] = img->img_data + y_size + u_size;
    }
    
    img->planes[VPX_PLANE_ALPHA] = NULL;
    
    // Fill with pattern, ensuring we don't write beyond allocated memory
    for (size_t i = 0; i < total_size && i < 1024 * 1024; i++) {
        img->img_data[i] = (unsigned char)(i % 256);
    }
}

static void cleanup_image(vpx_image_t *img) {
    if (img && img->img_data_owner && img->img_data) {
        free(img->img_data);
        img->img_data = NULL;
    }
}

static void init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size, 
                        vpx_codec_iface_t *iface) {
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, cfg, 0);
    if (res != VPX_CODEC_OK) {
        cfg->g_w = 64;
        cfg->g_h = 64;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 256;
        cfg->g_error_resilient = 0;
        cfg->g_pass = VPX_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->rc_end_usage = VPX_VBR;
        cfg->kf_mode = VPX_KF_AUTO;
        cfg->kf_min_dist = 0;
        cfg->kf_max_dist = 30;
        cfg->g_profile = 0;
        cfg->g_bit_depth = VPX_BITS_8;
        cfg->g_input_bit_depth = 8;
        cfg->g_threads = 1;
    }
    
    // Modify config based on fuzzer input with bounds checking
    if (size > 0) {
        cfg->g_w = 16 + (data[0] % 128);
    }
    if (size > 1) {
        cfg->g_h = 16 + (data[1] % 128);
    }
    if (size > 2) {
        cfg->rc_target_bitrate = 64 + (data[2] % 1000);
    }
    if (size > 3) {
        cfg->g_lag_in_frames = data[3] % 10;
    }
    if (size > 4) {
        cfg->kf_max_dist = 1 + (data[4] % 100);
    }
    if (size > 5) {
        cfg->g_error_resilient = data[5] & 1;
    }
    if (size > 6) {
        cfg->rc_end_usage = data[6] % 4;
    }
    if (size > 7) {
        cfg->g_profile = data[7] % 4;
    }
    
    // Ensure reasonable values
    if (cfg->g_w < 16) cfg->g_w = 16;
    if (cfg->g_h < 16) cfg->g_h = 16;
    if (cfg->g_w > 4096) cfg->g_w = 4096;
    if (cfg->g_h > 4096) cfg->g_h = 4096;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Choose codec based on first byte
    vpx_codec_iface_t *iface = NULL;
    if ((data[0] & 1) == 0) {
        iface = vpx_codec_vp8_cx();
    } else {
        iface = vpx_codec_vp9_cx();
    }
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    // Initialize configuration
    vpx_codec_enc_cfg_t cfg;
    init_config(&cfg, data, size, iface);
    
    // Initialize encoder
    vpx_codec_err_t res = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test vpx_codec_set_cx_data_buf with random buffer
    vpx_fixed_buf_t output_buf;
    unsigned char buf_data[4096];
    output_buf.buf = buf_data;
    output_buf.sz = sizeof(buf_data);
    
    if (size > 8) {
        unsigned int pad_before = data[8] % 64;
        unsigned int pad_after = data[9] % 64;
        vpx_codec_set_cx_data_buf(&codec, &output_buf, pad_before, pad_after);
    }
    
    // Create and initialize image
    vpx_image_t img;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    vpx_color_space_t cs = VPX_CS_BT_601;
    vpx_color_range_t range = VPX_CR_STUDIO_RANGE;
    unsigned int bit_depth = 8;
    
    if (size > 10) {
        switch (data[10] % 4) {
            case 0: fmt = VPX_IMG_FMT_I420; break;
            case 1: fmt = VPX_IMG_FMT_I422; break;
            case 2: fmt = VPX_IMG_FMT_I444; break;
            case 3: fmt = VPX_IMG_FMT_NV12; break;
        }
    }
    if (size > 11) {
        cs = (data[11] % 8);
    }
    if (size > 12) {
        range = (data[12] % 2);
    }
    if (size > 13) {
        bit_depth = 8 + ((data[13] % 3) * 2);
    }
    
    init_image(&img, cfg.g_w, cfg.g_h, fmt, cs, range, bit_depth);
    
    // Check if image allocation succeeded
    if (!img.img_data) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Encode frames
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
    
    if (size > 14) {
        flags = data[14] & 0xF;
    }
    if (size > 15) {
        switch (data[15] % 3) {
            case 0: deadline = VPX_DL_REALTIME; break;
            case 1: deadline = VPX_DL_GOOD_QUALITY; break;
            case 2: deadline = VPX_DL_BEST_QUALITY; break;
        }
    }
    
    // Encode first frame
    res = vpx_codec_encode(&codec, &img, pts, duration, flags, deadline);
    
    // Test reconfiguration
    if (size > 20 && res == VPX_CODEC_OK) {
        cfg.rc_target_bitrate = 128 + (data[20] % 512);
        if (size > 21) {
            cfg.g_error_resilient = (data[21] & 1);
        }
        vpx_codec_enc_config_set(&codec, &cfg);
    }
    
    // Encode more frames with NULL to flush
    for (int i = 0; i < 2; i++) {
        pts += duration;
        if (size > 22 + i) {
            flags = data[22 + i] & 0xF;
        }
        vpx_codec_encode(&codec, NULL, pts, duration, flags, deadline);
    }
    
    // Get compressed data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packet if needed
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Frame data available
        } else if (pkt->kind == VPX_CODEC_STATS_PKT) {
            // Stats data available
        }
    }
    
    // Test global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
    if (global_headers) {
        // Global headers available
    }
    
    // Clean up
    cleanup_image(&img);
    vpx_codec_destroy(&codec);
    
    return 0;
}