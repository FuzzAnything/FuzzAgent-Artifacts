// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_version at vpx_codec.c:24:5 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static void init_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    if (!cfg) return;
    
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    // Use fuzz data to set configuration parameters safely
    if (Size >= 4) {
        cfg->g_w = (Data[0] & 0x3F) * 16 + 16; // Ensure minimum 16x16
        cfg->g_h = (Data[1] & 0x3F) * 16 + 16;
        cfg->g_threads = (Data[2] & 0x0F) + 1;
        cfg->g_profile = Data[3] % 4;
    } else {
        cfg->g_w = 16;
        cfg->g_h = 16;
        cfg->g_threads = 1;
        cfg->g_profile = 0;
    }
    
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
    
    if (Size >= 5) {
        cfg->rc_end_usage = (enum vpx_rc_mode)(Data[4] % 4);
    } else {
        cfg->rc_end_usage = VPX_VBR;
    }
    
    cfg->rc_twopass_stats_in.buf = NULL;
    cfg->rc_twopass_stats_in.sz = 0;
    cfg->rc_firstpass_mb_stats_in.buf = NULL;
    cfg->rc_firstpass_mb_stats_in.sz = 0;
    
    if (Size >= 6) {
        cfg->rc_target_bitrate = (Data[5] % 100) + 100; // 100-199 kbps
    } else {
        cfg->rc_target_bitrate = 100;
    }
    
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    
    cfg->rc_undershoot_pct = 50;
    cfg->rc_overshoot_pct = 50;
    
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 750;
    
    cfg->rc_2pass_vbr_bias_pct = 50;
    cfg->rc_2pass_vbr_minsection_pct = 10;
    cfg->rc_2pass_vbr_maxsection_pct = 100;
    cfg->rc_2pass_vbr_corpus_complexity = 0;
    
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 120;
    
    cfg->ss_number_layers = 1;
    for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
        cfg->ss_enable_auto_alt_ref[i] = 0;
        cfg->ss_target_bitrate[i] = cfg->rc_target_bitrate;
    }
    
    cfg->ts_number_layers = 1;
    for (int i = 0; i < VPX_TS_MAX_LAYERS; i++) {
        cfg->ts_target_bitrate[i] = cfg->rc_target_bitrate;
        cfg->ts_rate_decimator[i] = 1;
    }
    cfg->ts_periodicity = 1;
    for (int i = 0; i < VPX_TS_MAX_PERIODICITY; i++) {
        cfg->ts_layer_id[i] = 0;
    }
    
    for (int i = 0; i < VPX_MAX_LAYERS; i++) {
        cfg->layer_target_bitrate[i] = cfg->rc_target_bitrate;
    }
    
    cfg->temporal_layering_mode = 0;
    cfg->use_vizier_rc_params = 0;
    
    // Initialize rational numbers
    cfg->active_wq_factor.num = 1;
    cfg->active_wq_factor.den = 1;
    cfg->err_per_mb_factor.num = 1;
    cfg->err_per_mb_factor.den = 1;
    cfg->sr_default_decay_limit.num = 1;
    cfg->sr_default_decay_limit.den = 1;
    cfg->sr_diff_factor.num = 1;
    cfg->sr_diff_factor.den = 1;
    cfg->kf_err_per_mb_factor.num = 1;
    cfg->kf_err_per_mb_factor.den = 1;
    cfg->kf_frame_min_boost_factor.num = 1;
    cfg->kf_frame_min_boost_factor.den = 1;
    cfg->kf_frame_max_boost_first_factor.num = 1;
    cfg->kf_frame_max_boost_first_factor.den = 1;
    cfg->kf_frame_max_boost_subs_factor.num = 1;
    cfg->kf_frame_max_boost_subs_factor.den = 1;
    cfg->kf_max_total_boost_factor.num = 1;
    cfg->kf_max_total_boost_factor.den = 1;
    cfg->gf_max_total_boost_factor.num = 1;
    cfg->gf_max_total_boost_factor.den = 1;
    cfg->gf_frame_max_boost_factor.num = 1;
    cfg->gf_frame_max_boost_factor.den = 1;
    cfg->zm_factor.num = 1;
    cfg->zm_factor.den = 1;
    cfg->rd_mult_inter_qp_fac.num = 1;
    cfg->rd_mult_inter_qp_fac.den = 1;
    cfg->rd_mult_arf_qp_fac.num = 1;
    cfg->rd_mult_arf_qp_fac.den = 1;
    cfg->rd_mult_key_qp_fac.num = 1;
    cfg->rd_mult_key_qp_fac.den = 1;
}

static void init_decoder_config(vpx_codec_dec_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    if (!cfg) return;
    
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    
    if (Size >= 2) {
        cfg->threads = (Data[0] & 0x0F) + 1;
        cfg->w = (Data[1] & 0x3F) * 16 + 16;
        cfg->h = (Data[1] >> 4) * 16 + 16;
    } else {
        cfg->threads = 1;
        cfg->w = 16;
        cfg->h = 16;
    }
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test vpx_codec_version
    int version = vpx_codec_version();
    (void)version; // Suppress unused variable warning
    
    // Test vpx_codec_err_to_string with various error codes
    vpx_codec_err_t err_codes[] = {
        VPX_CODEC_OK,
        VPX_CODEC_MEM_ERROR,
        VPX_CODEC_ERROR,
        VPX_CODEC_ABI_MISMATCH,
        VPX_CODEC_INCAPABLE
    };
    
    for (int i = 0; i < 5; i++) {
        const char *err_str = vpx_codec_err_to_string(err_codes[i]);
        (void)err_str; // Suppress unused variable warning
    }
    
    // Test with random error code from fuzz data
    if (Size >= 1) {
        vpx_codec_err_t random_err = (vpx_codec_err_t)Data[0];
        const char *random_err_str = vpx_codec_err_to_string(random_err);
        (void)random_err_str;
    }
    
    // Test encoder initialization
    vpx_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    
    vpx_codec_enc_cfg_t enc_cfg;
    init_encoder_config(&enc_cfg, Data, Size);
    
    vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
    if (enc_iface) {
        vpx_codec_err_t enc_res = vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        if (enc_res != VPX_CODEC_OK) {
            const char *enc_err = vpx_codec_error(&enc_ctx);
            (void)enc_err;
            
            const char *enc_detail = vpx_codec_error_detail(&enc_ctx);
            (void)enc_detail;
        } else {
            // Cleanup on success
            vpx_codec_destroy(&enc_ctx);
        }
    }
    
    // Test decoder initialization
    vpx_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    
    vpx_codec_dec_cfg_t dec_cfg;
    init_decoder_config(&dec_cfg, Data, Size);
    
    vpx_codec_iface_t *dec_iface = vpx_codec_vp8_dx();
    if (dec_iface) {
        vpx_codec_err_t dec_res = vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        
        if (dec_res != VPX_CODEC_OK) {
            const char *dec_err = vpx_codec_error(&dec_ctx);
            (void)dec_err;
            
            const char *dec_detail = vpx_codec_error_detail(&dec_ctx);
            (void)dec_detail;
        } else {
            // Cleanup on success
            vpx_codec_destroy(&dec_ctx);
        }
    }
    
    // Test with NULL context for error functions
    const char *null_err = vpx_codec_error(NULL);
    (void)null_err;
    
    const char *null_detail = vpx_codec_error_detail(NULL);
    (void)null_detail;
    
    // Test with uninitialized context
    vpx_codec_ctx_t uninit_ctx;
    memset(&uninit_ctx, 0, sizeof(uninit_ctx));
    
    const char *uninit_err = vpx_codec_error(&uninit_ctx);
    (void)uninit_err;
    
    const char *uninit_detail = vpx_codec_error_detail(&uninit_ctx);
    (void)uninit_detail;
    
    // Test with NULL config
    if (enc_iface) {
        vpx_codec_err_t null_cfg_enc_res = vpx_codec_enc_init_ver(&enc_ctx, enc_iface, NULL, 0, VPX_ENCODER_ABI_VERSION);
        (void)null_cfg_enc_res;
    }
    
    if (dec_iface) {
        vpx_codec_err_t null_cfg_dec_res = vpx_codec_dec_init_ver(&dec_ctx, dec_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
        (void)null_cfg_dec_res;
    }
    
    // Test with invalid ABI version
    if (enc_iface) {
        vpx_codec_err_t bad_ver_enc_res = vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, 0, 999);
        (void)bad_ver_enc_res;
    }
    
    if (dec_iface) {
        vpx_codec_err_t bad_ver_dec_res = vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, 999);
        (void)bad_ver_dec_res;
    }
    
    return 0;
}