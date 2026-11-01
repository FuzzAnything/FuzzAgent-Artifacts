// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void initialize_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    /* Safe defaults */
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_usage = 0;
    cfg->g_error_resilient = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    
    /* Rate control */
    cfg->rc_dropframe_thresh = 0;
    cfg->rc_resize_allowed = 0;
    cfg->rc_scaled_width = 0;
    cfg->rc_scaled_height = 0;
    cfg->rc_resize_up_thresh = 0;
    cfg->rc_resize_down_thresh = 0;
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_target_bitrate = 100;
    
    /* Quantizer */
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    
    /* Bitrate tolerance */
    cfg->rc_undershoot_pct = 10;
    cfg->rc_overshoot_pct = 10;
    
    /* Decoder buffer */
    cfg->rc_buf_sz = 5000;
    cfg->rc_buf_initial_sz = 2500;
    cfg->rc_buf_optimal_sz = 3000;
    
    /* Keyframing */
    cfg->kf_mode = VPX_KF_DISABLED;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 0;
    
    /* Scalability (defaults) */
    cfg->ss_number_layers = 0;
    cfg->ts_number_layers = 0;
    cfg->ts_periodicity = 0;
    cfg->temporal_layering_mode = 0;
    cfg->use_vizier_rc_params = 0;
    
    /* Fuzzed values from input data if available */
    if (size > 0) {
        cfg->g_w = (data[0] % 1024) + 16;
    }
    if (size > 1) {
        cfg->g_h = (data[1] % 1024) + 16;
    }
    if (size > 2) {
        cfg->g_threads = (data[2] % 8) + 1;
    }
    if (size > 3) {
        cfg->rc_target_bitrate = (data[3] % 1000) + 10;
    }
    if (size > 4) {
        cfg->rc_min_quantizer = data[4] % 64;
    }
    if (size > 5) {
        cfg->rc_max_quantizer = (data[5] % 63) + 1;
    }
    if (size > 6) {
        cfg->kf_mode = (data[6] % 3);
    }
}

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_fixed_buf_t custom_buf;
    unsigned char *buf_mem = NULL;
    unsigned int pad_before, pad_after;
    vpx_codec_err_t res;
    const vpx_codec_iface_t *iface;

    if (Size == 0) return 0;

    /* Initialize config with fuzzed data */
    initialize_config(&cfg, Data, Size);

    /* Try to initialize encoder context */
    iface = vpx_codec_vp9_cx();
    if (iface == NULL) {
        /* Fallback to VP8 if VP9 is not available */
        iface = vpx_codec_vp8_cx();
    }
    
    if (iface == NULL) {
        return 0;
    }

    /* Initialize context */
    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        /* Initialization failed, try with minimal config */
        memset(&cfg, 0, sizeof(cfg));
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_bit_depth = VPX_BITS_8;
        cfg.g_input_bit_depth = 8;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_end_usage = VPX_VBR;
        cfg.rc_target_bitrate = 100;
        cfg.kf_mode = VPX_KF_DISABLED;
        
        res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != VPX_CODEC_OK) {
            return 0;
        }
    }

    /* Fuzz vpx_codec_enc_config_set */
    /* Re-initialize config with fuzzed data again for reconfiguration */
    initialize_config(&cfg, Data, Size);
    
    /* Ensure valid ranges for reconfig */
    if (cfg.g_w == 0) cfg.g_w = 16;
    if (cfg.g_h == 0) cfg.g_h = 16;
    
    res = vpx_codec_enc_config_set(&ctx, &cfg);
    (void)res; /* Ignore result, just exercising the function */

    /* Fuzz vpx_codec_set_cx_data_buf */
    /* Create a custom buffer if data allows */
    if (Size > 8) {
        size_t buf_size = ((size_t)Data[0] + (size_t)Data[1]) * 100 + 1024;
        if (buf_size > 1024 * 1024) buf_size = 1024 * 1024; /* Cap size */
        
        buf_mem = (unsigned char *)malloc(buf_size);
        if (buf_mem != NULL) {
            custom_buf.buf = buf_mem;
            custom_buf.sz = buf_size;
            
            pad_before = ((unsigned int)Data[2] + (unsigned int)Data[3]) % 256;
            pad_after = ((unsigned int)Data[4] + (unsigned int)Data[5]) % 256;
            
            res = vpx_codec_set_cx_data_buf(&ctx, &custom_buf, pad_before, pad_after);
            (void)res;
            
            /* Try with NULL buffer to restore default */
            res = vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
            (void)res;
        }
    }

    /* Cleanup */
    if (buf_mem != NULL) {
        free(buf_mem);
    }
    
    vpx_codec_destroy(&ctx);
    
    return 0;
}