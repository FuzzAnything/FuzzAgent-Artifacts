// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>

/* Dummy callback for frame completion */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* Dummy callback for slice completion */
static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const struct vpx_image_rect *rect, const struct vpx_image_rect *slice_rect) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)slice_rect;
}

/* Helper to initialize encoder config with fuzzed data */
static void init_enc_cfg(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    if (size >= 8) {
        cfg->g_w = *(const uint32_t*)(data);
        cfg->g_h = *(const uint32_t*)(data + 4);
    } else {
        cfg->g_w = 64;
        cfg->g_h = 64;
    }
    
    /* Ensure valid dimensions */
    if (cfg->g_w == 0) cfg->g_w = 64;
    if (cfg->g_h == 0) cfg->g_h = 64;
    
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    
    cfg->rc_dropframe_thresh = 0;
    cfg->rc_resize_allowed = 0;
    cfg->rc_scaled_width = cfg->g_w;
    cfg->rc_scaled_height = cfg->g_h;
    cfg->rc_resize_up_thresh = 0;
    cfg->rc_resize_down_thresh = 0;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_target_bitrate = 100;
    
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    
    cfg->rc_undershoot_pct = 10;
    cfg->rc_overshoot_pct = 10;
    
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 500;
    
    cfg->kf_mode = VPX_KF_DISABLED;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 0;
    
    cfg->ss_number_layers = 1;
    cfg->ts_number_layers = 1;
    cfg->ts_periodicity = 1;
    cfg->ts_layer_id[0] = 0;
    
    cfg->temporal_layering_mode = 0;
    cfg->use_vizier_rc_params = 0;
}

/* Helper to initialize decoder config with fuzzed data */
static void init_dec_cfg(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    
    if (size >= 12) {
        cfg->threads = *(const uint32_t*)(data);
        cfg->w = *(const uint32_t*)(data + 4);
        cfg->h = *(const uint32_t*)(data + 8);
    } else {
        cfg->threads = 1;
        cfg->w = 64;
        cfg->h = 64;
    }
    
    if (cfg->threads == 0) cfg->threads = 1;
    if (cfg->w == 0) cfg->w = 64;
    if (cfg->h == 0) cfg->h = 64;
}

int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *enc_iface = vpx_codec_vp9_cx();
    
    if (!dec_iface && !enc_iface) return 0;

    /* Test 1: vpx_codec_get_caps with NULL and valid interfaces */
    vpx_codec_caps_t caps = vpx_codec_get_caps(NULL);
    (void)caps;
    
    if (dec_iface) {
        caps = vpx_codec_get_caps(dec_iface);
        (void)caps;
    }
    if (enc_iface) {
        caps = vpx_codec_get_caps(enc_iface);
        (void)caps;
    }

    /* Test 2: Decoder Initialization and Callbacks */
    if (dec_iface) {
        vpx_codec_dec_cfg_t dec_cfg;
        init_dec_cfg(&dec_cfg, Data, Size);
        
        vpx_codec_err_t res = vpx_codec_dec_init_ver(&ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        
        if (res == VPX_CODEC_OK) {
            /* Register frame callback */
            res = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, (void*)Data);
            (void)res;
            
            /* Register slice callback */
            res = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void*)Data);
            (void)res;
            
            vpx_codec_destroy(&ctx);
        }
    }

    /* Test 3: Encoder Initialization and Global Headers */
    if (enc_iface) {
        vpx_codec_enc_cfg_t enc_cfg;
        init_enc_cfg(&enc_cfg, Data, Size);
        
        vpx_codec_err_t res = vpx_codec_enc_init_ver(&ctx, enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        if (res == VPX_CODEC_OK) {
            /* Try to get global headers (VP9 specific) */
            vpx_fixed_buf_t *hdrs = vpx_codec_get_global_headers(&ctx);
            (void)hdrs;
            
            vpx_codec_destroy(&ctx);
        }
    }

    /* Test 4: Invalid/Uninitialized Context Calls */
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_err_t res = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, (void*)Data);
    (void)res;
    
    res = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void*)Data);
    (void)res;
    
    vpx_fixed_buf_t *hdrs = vpx_codec_get_global_headers(&ctx);
    (void)hdrs;

    return 0;
}