// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_config(vpx_codec_enc_cfg_t *cfg, vpx_codec_iface_t *iface) {
    // Set some reasonable defaults for fuzzing
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->g_error_resilient = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
}

static vpx_image_t* create_test_image(const uint8_t *data, size_t size) {
    // Use I420 format (planar YUV) which is commonly supported
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int stride_align = 1;
    
    // Calculate required buffer size for I420
    // Y plane: width * height
    // U plane: (width/2) * (height/2)
    // V plane: (width/2) * (height/2)
    size_t y_size = width * height;
    size_t uv_size = (width/2) * (height/2);
    size_t total_size = y_size + 2 * uv_size;
    
    // Allocate buffer for image data
    unsigned char *img_data = (unsigned char*)malloc(total_size);
    if (!img_data) return NULL;
    
    // Fill with fuzzer data, repeating if needed
    for (size_t i = 0; i < total_size; i++) {
        img_data[i] = data[i % size];
    }
    
    // Create image descriptor on heap
    vpx_image_t *img = (vpx_image_t*)malloc(sizeof(vpx_image_t));
    if (!img) {
        free(img_data);
        return NULL;
    }
    
    // Wrap the existing buffer
    vpx_image_t *result = vpx_img_wrap(img, fmt, width, height, stride_align, img_data);
    if (!result) {
        free(img);
        free(img_data);
        return NULL;
    }
    
    return result;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Step 1: Get VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;
    
    // Step 2: Initialize encoder configuration with defaults
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) return 0;
    
    // Customize config with fuzzer-influenced values
    initialize_config(&cfg, iface);
    
    // Use fuzzer data to influence some parameters
    if (Size > 0) {
        cfg.g_w = 16 + (Data[0] % 128);  // Width between 16-143
        cfg.g_h = 16 + (Data[0] % 128);  // Height between 16-143
    }
    if (Size > 1) {
        cfg.rc_target_bitrate = 64 + (Data[1] % 1024);  // Bitrate between 64-1087
    }
    if (Size > 2) {
        cfg.g_error_resilient = Data[2] & 1;
    }
    
    // Step 3: Create test image using fuzzer data
    vpx_image_t *img = create_test_image(Data, Size);
    if (!img) return 0;
    
    // Step 4: Get VP8 encoder interface again (as per specification)
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    (void)iface2;  // Mark as used
    
    // Create encoder context
    vpx_codec_ctx_t codec;
    err = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        free(img->img_data);
        free(img);
        return 0;
    }
    
    // Encode the image (minimal encoding to exercise the codec)
    err = vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Get compressed data (even if encoding failed)
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packets if needed
        (void)pkt;
    }
    
    // Step 5: Destroy the codec instance
    err = vpx_codec_destroy(&codec);
    
    // Clean up image resources
    vpx_img_free(img);
    free(img->img_data);
    free(img);
    
    return 0;
}