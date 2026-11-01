// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

#include "vpx_decoder.h"
#include "vpx_codec.h"
#include "vpx_encoder.h"
#include "vp8cx.h"
#include "vp8dx.h"

static void init_encoder_cfg(vpx_codec_enc_cfg_t *cfg, uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    // Use fuzz data to set configuration values safely
    if (size >= 1) cfg->g_threads = data[0] % 16;
    if (size >= 2) cfg->g_profile = data[1] % 4;
    if (size >= 4) cfg->g_w = (data[2] * 256 + data[3]) % 1920 + 16;
    if (size >= 6) cfg->g_h = (data[4] * 256 + data[5]) % 1080 + 16;
    
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
    
    if (size >= 7) cfg->rc_end_usage = (data[6] % 4);
    
    cfg->rc_twopass_stats_in.buf = NULL;
    cfg->rc_twopass_stats_in.sz = 0;
    cfg->rc_firstpass_mb_stats_in.buf = NULL;
    cfg->rc_firstpass_mb_stats_in.sz = 0;
    
    if (size >= 9) cfg->rc_target_bitrate = (data[7] * 256 + data[8]) % 10000 + 100;
    
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    
    cfg->rc_undershoot_pct = 0;
    cfg->rc_overshoot_pct = 0;
    
    cfg->rc_buf_sz = 2000;
    cfg->rc_buf_initial_sz = 1000;
    cfg->rc_buf_optimal_sz = 1500;
    
    cfg->rc_2pass_vbr_bias_pct = 0;
    cfg->rc_2pass_vbr_minsection_pct = 0;
    cfg->rc_2pass_vbr_maxsection_pct = 0;
    cfg->rc_2pass_vbr_corpus_complexity = 0;
    
    cfg->kf_mode = VPX_KF_AUTO;
    cfg->kf_min_dist = 120;
    cfg->kf_max_dist = 240;
    
    cfg->ss_number_layers = 1;
    memset(cfg->ss_enable_auto_alt_ref, 0, sizeof(cfg->ss_enable_auto_alt_ref));
    memset(cfg->ss_target_bitrate, 0, sizeof(cfg->ss_target_bitrate));
    
    cfg->ts_number_layers = 1;
    memset(cfg->ts_target_bitrate, 0, sizeof(cfg->ts_target_bitrate));
    memset(cfg->ts_rate_decimator, 0, sizeof(cfg->ts_rate_decimator));
    cfg->ts_periodicity = 1;
    memset(cfg->ts_layer_id, 0, sizeof(cfg->ts_layer_id));
    
    memset(cfg->layer_target_bitrate, 0, sizeof(cfg->layer_target_bitrate));
    
    cfg->temporal_layering_mode = 0;
    cfg->use_vizier_rc_params = 0;
}

static void init_decoder_cfg(vpx_codec_dec_cfg_t *cfg, uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    
    if (size >= 1) cfg->threads = data[0] % 16;
    if (size >= 3) cfg->w = (data[1] * 256 + data[2]) % 1920 + 16;
    if (size >= 5) cfg->h = (data[3] * 256 + data[4]) % 1080 + 16;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_iface_t *enc_iface = NULL;
    vpx_codec_iface_t *dec_iface = NULL;
    
    // Try to get encoder interface
    enc_iface = vpx_codec_vp8_cx();
    if (!enc_iface) enc_iface = vpx_codec_vp9_cx();
    
    // Try to get decoder interface
    dec_iface = vpx_codec_vp8_dx();
    if (!dec_iface) dec_iface = vpx_codec_vp9_dx();
    
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_dec_cfg_t dec_cfg;
    
    init_encoder_cfg(&enc_cfg, (uint8_t *)Data, Size);
    init_decoder_cfg(&dec_cfg, (uint8_t *)Data, Size);
    
    vpx_codec_flags_t flags = 0;
    if (Size >= 10) flags = Data[9] % 10;
    
    // Test vpx_codec_enc_init_ver
    if (enc_iface) {
        vpx_codec_err_t res = vpx_codec_enc_init_ver(&ctx, enc_iface, &enc_cfg, flags, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Test vpx_codec_control_ with safe control IDs
            // VP8E_SET_SCALEMODE is a valid control ID for VP8 encoder
            vpx_scaling_mode_t scale_mode;
            scale_mode.h_scaling_mode = (Size >= 11) ? (VPX_SCALING_MODE)(Data[10] % 4) : VP8E_NORMAL;
            scale_mode.v_scaling_mode = (Size >= 12) ? (VPX_SCALING_MODE)(Data[11] % 4) : VP8E_NORMAL;
            
            vpx_codec_control_(&ctx, VP8E_SET_SCALEMODE, &scale_mode);
            
            // Check if we are using VP9 by looking at the context name instead of the opaque interface
            // The interface structure is opaque, so we cannot access enc_iface->name directly.
            // However, after init, ctx.name should reflect the codec used.
            if (ctx.name && strstr(ctx.name, "vp9")) {
                int tune_content = (Size >= 13) ? (Data[12] % 3) : 0;
                vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, &tune_content);
            }
            
            vpx_codec_destroy(&ctx);
        } else {
            // Check error details
            const char *err_detail = vpx_codec_error_detail(&ctx);
            (void)err_detail; // Suppress unused warning
        }
    }
    
    // Reset context for decoder test
    memset(&ctx, 0, sizeof(ctx));
    
    // Test vpx_codec_dec_init_ver
    if (dec_iface) {
        vpx_codec_err_t res = vpx_codec_dec_init_ver(&ctx, dec_iface, &dec_cfg, flags, VPX_DECODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // For decoder, we should not call control commands that need frame data immediately
            // Just test error retrieval
            const char *err_detail = vpx_codec_error_detail(&ctx);
            (void)err_detail;
            
            vpx_codec_destroy(&ctx);
        } else {
            const char *err_detail = vpx_codec_error_detail(&ctx);
            (void)err_detail;
        }
    }
    
    return 0;
}