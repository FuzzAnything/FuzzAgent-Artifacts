// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_image_fill(vpx_image_t *img) {
    if (!img) return;
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char *buf = img->planes[plane];
        int stride = img->stride[plane];
        int w = (plane == 0) ? img->d_w : (img->d_w + 1) >> 1;
        int h = (plane == 0) ? img->d_h : (img->d_h + 1) >> 1;
        for (int y = 0; y < h; ++y) {
            memset(buf + y * stride, 128, w);
        }
    }
}

static vpx_image_t *create_dummy_image(unsigned int width, unsigned int height) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
    if (img) {
        dummy_image_fill(img);
    }
    return img;
}

static void parse_fuzz_data(const uint8_t *data, size_t size, 
                           vpx_codec_enc_cfg_t *cfg,
                           int *control_id, int *control_val) {
    if (size < 4) return;
    
    // Use first bytes to influence config
    cfg->g_w = 64 + (data[0] % 256);
    cfg->g_h = 64 + (data[1] % 256);
    cfg->rc_target_bitrate = 100 + (data[2] % 1000);
    cfg->rc_min_quantizer = data[3] % 64;
    cfg->rc_max_quantizer = 10 + (data[3] % 54);
    
    if (size >= 6) {
        *control_id = data[4] % 20;  // Sample control IDs
        *control_val = data[5];
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    if (!iface || Size == 0) return 0;
    
    // Initialize with default config
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify config with fuzz data
    int control_id = 0, control_val = 0;
    parse_fuzz_data(Data, Size, &cfg, &control_id, &control_val);
    
    // Ensure valid dimensions
    cfg.g_w = (cfg.g_w < 16) ? 16 : (cfg.g_w > 4096 ? 4096 : cfg.g_w);
    cfg.g_h = (cfg.g_h < 16) ? 16 : (cfg.g_h > 4096 ? 4096 : cfg.g_h);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Try to reconfigure with potentially different settings
    if (Size > 10) {
        cfg.rc_target_bitrate = 200 + (Data[6] % 800);
        vpx_codec_enc_config_set(&codec, &cfg);
    }
    
    // Try various control operations
    vpx_codec_control_(&codec, VP8E_SET_CPUUSED, 8);
    vpx_codec_control_(&codec, VP8E_SET_ENABLEAUTOALTREF, 0);
    vpx_codec_control_(&codec, VP8E_SET_NOISE_SENSITIVITY, control_val % 4);
    
    // Create dummy image and encode
    vpx_image_t *img = create_dummy_image(cfg.g_w, cfg.g_h);
    if (img) {
        vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet if needed
        }
        
        // Get global headers
        vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
        if (global_headers) {
            // Access the buffer to ensure it's valid
            volatile void *buf = global_headers->buf;
            volatile size_t sz = global_headers->sz;
            (void)buf;
            (void)sz;
        }
        
        vpx_img_free(img);
    }
    
    // Clean up
    vpx_codec_destroy(&codec);
    return 0;
}