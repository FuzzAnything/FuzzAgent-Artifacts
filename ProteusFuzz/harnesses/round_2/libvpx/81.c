// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

static void init_enc_cfg(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (cfg == NULL) return;
    
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Fuzz configuration parameters based on input data
    if (size > 0) cfg->g_w = (data[0] % 1024) + 16;
    if (size > 1) cfg->g_h = (data[1] % 1024) + 16;
    if (size > 2) cfg->g_threads = (data[2] % 8) + 1;
    if (size > 3) cfg->rc_target_bitrate = (data[3] % 10000) + 100;
    if (size > 4) cfg->g_timebase.num = (data[4] % 100) + 1;
    if (size > 5) cfg->g_timebase.den = (data[5] % 1000) + 1;
    if (size > 6) cfg->rc_min_quantizer = data[6] % 64;
    if (size > 7) cfg->rc_max_quantizer = (data[7] % 63) + 1;
    if (size > 8) cfg->kf_min_dist = (data[8] % 300) + 1;
    if (size > 9) cfg->kf_max_dist = (data[9] % 300) + 1;
    
    // Ensure width and height are multiples of 2 for VP8
    cfg->g_w = (cfg->g_w / 2) * 2;
    cfg->g_h = (cfg->g_h / 2) * 2;
    if (cfg->g_w < 16) cfg->g_w = 16;
    if (cfg->g_h < 16) cfg->g_h = 16;
}

static vpx_image_t *create_fuzz_image(const uint8_t *data, size_t size, unsigned int w, unsigned int h) {
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, w, h, 1);
    if (img == NULL) return NULL;

    size_t plane_size = w * h;
    size_t uv_size = (w / 2) * (h / 2);
    
    // Fill Y plane
    size_t y_offset = 0;
    for (unsigned int i = 0; i < h; i++) {
        size_t copy_len = (size - y_offset < plane_size) ? (size - y_offset) : plane_size;
        memcpy(img->planes[0] + i * img->stride[0], data + y_offset, copy_len);
        y_offset += copy_len;
        if (y_offset >= size) y_offset = 0;
    }
    
    // Fill U plane
    size_t u_offset = 0;
    for (unsigned int i = 0; i < h / 2; i++) {
        size_t copy_len = (size - u_offset < uv_size) ? (size - u_offset) : uv_size;
        memcpy(img->planes[1] + i * img->stride[1], data + u_offset, copy_len);
        u_offset += copy_len;
        if (u_offset >= size) u_offset = 0;
    }
    
    // Fill V plane
    size_t v_offset = 0;
    for (unsigned int i = 0; i < h / 2; i++) {
        size_t copy_len = (size - v_offset < uv_size) ? (size - v_offset) : uv_size;
        memcpy(img->planes[2] + i * img->stride[2], data + v_offset, copy_len);
        v_offset += copy_len;
        if (v_offset >= size) v_offset = 0;
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (iface == NULL) return 0;

    // Test vpx_codec_get_caps
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps; // Suppress unused warning

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    init_enc_cfg(&cfg, Data, Size);

    // Test vpx_codec_enc_init_ver
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Initialization failed, try with different flags or just return
        // We can also try to init with flags
        err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, VPX_CODEC_USE_PSNR, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }

    // Test vpx_codec_enc_config_set
    vpx_codec_enc_cfg_t new_cfg;
    init_enc_cfg(&new_cfg, Data, Size);
    err = vpx_codec_enc_config_set(&ctx, &new_cfg);
    (void)err;

    // Create a test image
    unsigned int w = cfg.g_w;
    unsigned int h = cfg.g_h;
    vpx_image_t *img = create_fuzz_image(Data, Size, w, h);
    
    if (img != NULL) {
        // Test vpx_codec_encode with valid image
        err = vpx_codec_encode(&ctx, img, 0, 1, 0, VPX_DL_REALTIME);
        (void)err;
        
        // Encode a few more frames to exercise state
        for (int i = 1; i < 5 && i < Size; i++) {
            vpx_codec_encode(&ctx, img, i, 1, 0, VPX_DL_REALTIME);
        }
        
        vpx_img_free(img);
    }

    // Flush the encoder
    vpx_codec_encode(&ctx, NULL, 0, 0, 0, VPX_DL_REALTIME);

    // Cleanup
    vpx_codec_destroy(&ctx);

    // Test multi-encoder path if size allows
    if (Size > 10) {
        int num_enc = (Data[10] % 4) + 1; // 1 to 4 encoders
        vpx_codec_ctx_t *ctxs = malloc(num_enc * sizeof(vpx_codec_ctx_t));
        vpx_codec_enc_cfg_t *cfgs = malloc(num_enc * sizeof(vpx_codec_enc_cfg_t));
        vpx_rational_t *dsf = malloc(num_enc * sizeof(vpx_rational_t));
        
        if (ctxs && cfgs && dsf) {
            for (int i = 0; i < num_enc; i++) {
                init_enc_cfg(&cfgs[i], Data, Size);
                dsf[i].num = 1;
                dsf[i].den = 1;
            }
            
            err = vpx_codec_enc_init_multi_ver(ctxs, iface, cfgs, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION);
            (void)err;
            
            // Cleanup multi-encoder contexts if init succeeded
            if (err == VPX_CODEC_OK) {
                for (int i = 0; i < num_enc; i++) {
                    vpx_codec_destroy(&ctxs[i]);
                }
            }
            
            free(ctxs);
            free(cfgs);
            free(dsf);
        }
    }

    return 0;
}