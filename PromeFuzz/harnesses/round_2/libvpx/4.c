// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
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
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Use fuzzer data to initialize some configuration parameters
    if (size >= 4) {
        cfg->g_w = (data[0] << 8) | data[1];
        cfg->g_h = (data[2] << 8) | data[3];
        // Limit dimensions to reasonable values to avoid excessive memory usage
        if (cfg->g_w == 0) cfg->g_w = 1;
        if (cfg->g_h == 0) cfg->g_h = 1;
        if (cfg->g_w > 4096) cfg->g_w = 4096;
        if (cfg->g_h > 4096) cfg->g_h = 4096;
    } else {
        cfg->g_w = 320;
        cfg->g_h = 240;
    }
    
    cfg->rc_target_bitrate = 1000;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 56;
    cfg->rc_end_usage = VPX_VBR;
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_ctx_t codec;
    vpx_image_t *img = NULL;
    vpx_codec_err_t res;
    
    // Handle empty input to avoid division by zero
    if (Size == 0) {
        return 0;
    }
    
    // 1. Get VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }
    
    // 2. Get default encoder configuration
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Initialize configuration with fuzzer data
    fuzz_init_config(&cfg, Data, Size);
    
    // 3. Create image wrapper
    // Calculate buffer size for I420 format
    unsigned int width = cfg.g_w;
    unsigned int height = cfg.g_h;
    size_t y_plane = width * height;
    size_t uv_plane = ((width + 1) / 2) * ((height + 1) / 2);
    size_t total_size = y_plane + 2 * uv_plane;
    
    // Allocate image buffer
    unsigned char *img_data = (unsigned char *)malloc(total_size);
    if (!img_data) {
        return 0;
    }
    
    // Initialize with fuzzer data (wrap around if needed)
    // Size is guaranteed to be > 0 here
    for (size_t i = 0; i < total_size; i++) {
        img_data[i] = Data[i % Size];
    }
    
    // Create image descriptor
    img = vpx_img_wrap(NULL, VPX_IMG_FMT_I420, width, height, 1, img_data);
    if (!img) {
        free(img_data);
        return 0;
    }
    
    // 4. Get VP8 encoder interface again (as specified in order)
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        vpx_img_free(img);
        free(img_data);
        return 0;
    }
    
    // Initialize encoder
    res = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (res == VPX_CODEC_OK) {
        // Try to encode the image
        res = vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // 5. Destroy codec instance
        vpx_codec_destroy(&codec);
    }
    
    // Cleanup
    vpx_img_free(img);
    free(img_data);
    
    return 0;
}