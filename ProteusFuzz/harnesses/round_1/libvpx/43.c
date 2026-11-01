// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>

/* Helper to safely initialize encoder config with fuzz data */
static void init_enc_cfg_fuzz(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    if (size >= 8) {
        cfg->g_w = *(uint32_t*)(data + 0);
        cfg->g_h = *(uint32_t*)(data + 4);
        if (cfg->g_w > 4096) cfg->g_w = 4096;
        if (cfg->g_h > 4096) cfg->g_h = 4096;
        if (cfg->g_w == 0) cfg->g_w = 16;
        if (cfg->g_h == 0) cfg->g_h = 16;
    } else {
        cfg->g_w = 16;
        cfg->g_h = 16;
    }

    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    
    if (size >= 16) {
        cfg->g_timebase.num = *(int*)(data + 8);
        cfg->g_timebase.den = *(int*)(data + 12);
        if (cfg->g_timebase.den == 0) cfg->g_timebase.den = 1;
        if (cfg->g_timebase.num == 0) cfg->g_timebase.num = 1;
    } else {
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
    }

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
    if (size >= 16) {
        uint32_t br = *(uint32_t*)(data + 16);
        if (br > 0 && br < 1000000) cfg->rc_target_bitrate = br;
    }

    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    
    cfg->rc_undershoot_pct = 100;
    cfg->rc_overshoot_pct = 100;
    
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 500;

    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 120;

    cfg->ss_number_layers = 1;
    cfg->ts_number_layers = 1;
    cfg->ts_periodicity = 1;
    cfg->ts_layer_id[0] = 0;
    
    cfg->use_vizier_rc_params = 0;
}

/* Helper to safely initialize decoder config with fuzz data */
static void init_dec_cfg_fuzz(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    cfg->threads = 1;
    if (size >= 8) {
        cfg->w = *(uint32_t*)(data + 0);
        cfg->h = *(uint32_t*)(data + 4);
        if (cfg->w > 4096) cfg->w = 4096;
        if (cfg->h > 4096) cfg->h = 4096;
        if (cfg->w == 0) cfg->w = 16;
        if (cfg->h == 0) cfg->h = 16;
    } else {
        cfg->w = 16;
        cfg->h = 16;
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    /* 1. vpx_codec_version */
    int version = vpx_codec_version();
    /* Manually extract version components as macros might be missing in some builds */
    int major = version >> 16;
    int minor = (version >> 8) & 0xFF;
    int patch = version & 0xFF;
    (void)major; (void)minor; (void)patch;

    /* 2. vpx_codec_control_ (Requires a valid context, so we test inside init blocks) */

    /* 3. vpx_codec_dec_init_ver */
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    init_dec_cfg_fuzz(&dec_cfg, Data, Size);
    
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    if (dec_iface) {
        vpx_codec_err_t res = vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            /* Try to invoke control on a valid decoder context */
            /* Using a dummy control ID to explore error paths */
            vpx_codec_control_(&dec_ctx, 0x12345678); 
            
            vpx_codec_destroy(&dec_ctx);
        }
    }

    /* 4. VP9E_SET_TUNE_CONTENT (Enum, used via control) */
    /* 5. VP8E_SET_TUNING (Enum, used via control) */
    /* These are tested via vpx_codec_enc_init_ver and control calls below */

    /* 6. vpx_codec_enc_init_ver */
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    init_enc_cfg_fuzz(&enc_cfg, Data, Size);

    /* Try VP8 Encoder */
    vpx_codec_iface_t *enc_iface_vp8 = vpx_codec_vp8_cx();
    if (enc_iface_vp8) {
        vpx_codec_err_t res = vpx_codec_enc_init_ver(&enc_ctx, enc_iface_vp8, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            /* Test control with valid context */
            vpx_codec_control_(&enc_ctx, VP8E_SET_TUNING, 0);
            vpx_codec_control_(&enc_ctx, VP8E_SET_TUNING, 100);
            vpx_codec_control_(&enc_ctx, VP8E_SET_TUNING, -1);
            vpx_codec_control_(&enc_ctx, VP8E_SET_CPUUSED, 0);
            vpx_codec_control_(&enc_ctx, VP8E_SET_CPUUSED, 8);
            vpx_codec_control_(&enc_ctx, VP8E_SET_CPUUSED, -1);
            
            vpx_codec_destroy(&enc_ctx);
        }
    }

    /* Try VP9 Encoder */
    vpx_codec_iface_t *enc_iface_vp9 = vpx_codec_vp9_cx();
    if (enc_iface_vp9) {
        vpx_codec_err_t res = vpx_codec_enc_init_ver(&enc_ctx, enc_iface_vp9, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            /* Test control with valid context */
            vpx_codec_control_(&enc_ctx, VP9E_SET_TUNE_CONTENT, 0);
            vpx_codec_control_(&enc_ctx, VP9E_SET_TUNE_CONTENT, 1);
            vpx_codec_control_(&enc_ctx, VP9E_SET_TUNE_CONTENT, 2);
            vpx_codec_control_(&enc_ctx, VP9E_SET_TUNE_CONTENT, 100);
            /* VP9 does not have VP9E_SET_CPUUSED in older headers, using generic control or skipping */
            /* Testing with a generic invalid control ID to trigger error paths if needed */
            vpx_codec_control_(&enc_ctx, 0x99999999); 
            
            vpx_codec_destroy(&enc_ctx);
        }
    }

    /* Test invalid control calls on uninitialized context */
    vpx_codec_control_(&dec_ctx, 0x12345678);
    vpx_codec_control_(&enc_ctx, 0x12345678);

    return 0;
}