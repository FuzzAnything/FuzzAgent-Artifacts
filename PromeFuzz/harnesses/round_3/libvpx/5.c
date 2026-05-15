// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

static void initialize_encoder_config(vpx_codec_enc_cfg_t *cfg, 
                                     vpx_codec_iface_t *iface,
                                     const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Get default configuration
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, cfg, 0);
    if (res != VPX_CODEC_OK) return;
    
    // Use fuzz data to modify some parameters (within reasonable bounds)
    if (size >= 4) {
        cfg->g_w = 64 + (data[0] % 128);  // Width between 64-191
        cfg->g_h = 64 + (data[1] % 128);  // Height between 64-191
        cfg->rc_target_bitrate = 100 + (data[2] % 900);  // 100-999 kbps
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->g_threads = 1 + (data[3] % 4);  // 1-4 threads
    }
    
    // Set some reasonable defaults for fuzzing
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
}

static vpx_image_t *create_test_image(const uint8_t *data, size_t size,
                                     unsigned int width, unsigned int height) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;
    
    // Fill image planes with fuzz data (or zeros if not enough)
    size_t y_plane_size = width * height;
    size_t uv_plane_size = y_plane_size / 4;
    
    // Y plane
    size_t copy_size = size > y_plane_size ? y_plane_size : size;
    if (copy_size > 0) {
        memcpy(img->planes[VPX_PLANE_Y], data, copy_size);
    }
    
    // U plane (use data offset or zeros)
    if (size > y_plane_size) {
        copy_size = (size - y_plane_size) > uv_plane_size ? uv_plane_size : (size - y_plane_size);
        memcpy(img->planes[VPX_PLANE_U], data + y_plane_size, copy_size);
    }
    
    // V plane (use more data or zeros)
    if (size > y_plane_size + uv_plane_size) {
        copy_size = (size - y_plane_size - uv_plane_size) > uv_plane_size ? 
                   uv_plane_size : (size - y_plane_size - uv_plane_size);
        memcpy(img->planes[VPX_PLANE_V], data + y_plane_size + uv_plane_size, copy_size);
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Choose codec interface based on first byte
    vpx_codec_iface_t *iface = (data[0] & 1) ? 
                              vpx_codec_vp9_cx() : vpx_codec_vp8_cx();
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Initialize configuration
    initialize_encoder_config(&cfg, iface, data, size);
    
    // Initialize encoder
    vpx_codec_err_t res = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, 
                                                VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test vpx_codec_get_global_headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
    (void)global_headers; // May be NULL for VP8
    
    // Create test image
    vpx_image_t *img = create_test_image(data + 1, size > 1 ? size - 1 : 0,
                                        cfg.g_w, cfg.g_h);
    
    // Test vpx_codec_encode with the image
    if (img) {
        vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Test vpx_codec_get_cx_data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet (just for fuzzing coverage)
            (void)pkt;
        }
        
        vpx_img_free(img);
    }
    
    // Test vpx_codec_encode with NULL to flush
    vpx_codec_encode(&codec, NULL, 0, 0, 0, VPX_DL_REALTIME);
    
    // Test vpx_codec_get_cx_data after flush
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        (void)pkt;
    }
    
    // Test vpx_codec_control_ with some common control IDs
    // Use the macro wrapper as recommended
    if (iface == vpx_codec_vp8_cx()) {
        // VP8 controls - use available control IDs
        vpx_codec_control_(&codec, VP8E_SET_NOISE_SENSITIVITY, data[0] % 4);
        vpx_codec_control_(&codec, VP8E_SET_CPUUSED, 8);
    }
    // For VP9, we don't have the control IDs defined in the headers we're allowed to use
    
    // Test vpx_codec_enc_config_set with modified configuration
    if (size >= 2) {
        cfg.rc_target_bitrate = 200 + (data[1] % 800);
        vpx_codec_enc_config_set(&codec, &cfg);
    }
    
    // Clean up
    vpx_codec_destroy(&codec);
    
    return 0;
}