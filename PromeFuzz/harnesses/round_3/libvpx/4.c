// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Use fuzzer data to set some configuration parameters
    if (size >= sizeof(vpx_codec_enc_cfg_t)) {
        memcpy(cfg, data, sizeof(vpx_codec_enc_cfg_t));
    } else {
        // Set some default values if we don't have enough data
        cfg->g_w = 64;
        cfg->g_h = 64;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 256;
        cfg->g_error_resilient = 0;
        cfg->g_pass = VPX_RC_ONE_PASS;
        cfg->g_lag_in_frames = 0;
        cfg->kf_mode = VPX_KF_AUTO;
        cfg->kf_min_dist = 0;
        cfg->kf_max_dist = 100;
    }
    
    // Ensure reasonable values to avoid crashes
    if (cfg->g_w == 0) cfg->g_w = 64;
    if (cfg->g_h == 0) cfg->g_h = 64;
    if (cfg->g_w > 4096) cfg->g_w = 4096;
    if (cfg->g_h > 4096) cfg->g_h = 4096;
    if (cfg->g_timebase.den == 0) cfg->g_timebase.den = 30;
}

static unsigned char *create_image_buffer(unsigned int width, unsigned int height, 
                                         vpx_img_fmt_t fmt, size_t *buffer_size) {
    // Calculate approximate buffer size needed for the image
    // This is a simplified calculation - real implementation would be more complex
    unsigned int bytes_per_pixel = 1; // Default for 8-bit formats
    if (fmt & VPX_IMG_FMT_HIGHBITDEPTH) {
        bytes_per_pixel = 2;
    }
    
    // For planar formats, we need space for Y, U, and V planes
    if (fmt & VPX_IMG_FMT_PLANAR) {
        // I420 format: Y plane + U plane + V plane
        *buffer_size = width * height * bytes_per_pixel * 3 / 2;
    } else {
        // Packed format
        *buffer_size = width * height * bytes_per_pixel * 3;
    }
    
    if (*buffer_size == 0) *buffer_size = 1024;
    if (*buffer_size > 1024 * 1024 * 16) *buffer_size = 1024 * 1024 * 16; // Limit to 16MB
    
    return (unsigned char *)malloc(*buffer_size);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_ctx_t codec;
    vpx_image_t *img = NULL;
    vpx_codec_err_t res;
    unsigned char *img_data = NULL;
    size_t buffer_size = 0;
    
    // Step 1: Get VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) return 0;
    
    // Step 2: Initialize encoder config with defaults
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return 0;
    
    // Override some config values with fuzzer data
    initialize_encoder_config(&cfg, Data, Size);
    
    // Step 3: Create image wrapper
    img_data = create_image_buffer(cfg.g_w, cfg.g_h, VPX_IMG_FMT_I420, &buffer_size);
    if (!img_data) return 0;
    
    // Fill image buffer with fuzzer data (wrapping around if needed)
    if (Size > 0) {
        for (size_t i = 0; i < buffer_size; i++) {
            img_data[i] = Data[i % Size];
        }
    }
    
    img = vpx_img_wrap(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img_data);
    if (!img) {
        free(img_data);
        return 0;
    }
    
    // Step 4: Initialize encoder (this is the second vpx_codec_vp8_cx usage mentioned)
    // Actually we need to call vpx_codec_enc_init, not vpx_codec_vp8_cx again
    // The description seems to have an error - we should initialize the encoder
    res = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (res == VPX_CODEC_OK) {
        // Try to encode the image
        vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
    }
    
    // Step 5: Destroy the codec
    vpx_codec_destroy(&codec);
    
    // Cleanup image
    if (img) {
        // Free the image descriptor
        vpx_img_free(img);
        // Note: vpx_img_free does NOT free the img_data buffer
        // We need to free it separately since we allocated it
        free(img_data);
    }
    
    return 0;
}