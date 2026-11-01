// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP8D_GET_LAST_REF_UPDATES__value at vp8dx.h:190:1 in vp8dx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>

static void init_config(vpx_codec_enc_cfg_t *cfg, unsigned int width, unsigned int height) {
    if (!cfg) return;
    memset(cfg, 0, sizeof(*cfg));
    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_threads = 1;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->rc_dropframe_thresh = 0;
    cfg->rc_resize_allowed = 0;
    cfg->rc_scaled_width = 0;
    cfg->rc_scaled_height = 0;
    cfg->rc_resize_up_thresh = 0;
    cfg->rc_resize_down_thresh = 0;
    cfg->rc_end_usage = VPX_CBR;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    cfg->rc_undershoot_pct = 100;
    cfg->rc_overshoot_pct = 100;
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 750;
    cfg->rc_2pass_vbr_bias_pct = 0;
    cfg->rc_2pass_vbr_minsection_pct = 0;
    cfg->rc_2pass_vbr_maxsection_pct = 0;
    cfg->kf_mode = VPX_KF_DISABLED;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 0;
    cfg->ss_number_layers = 1;
    cfg->ts_number_layers = 1;
    cfg->ts_periodicity = 1;
    cfg->ts_layer_id[0] = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;
    int width, height, ctrl_id;
    int tuning_val, cpu_val;

    // Parse input for dimensions and control parameters
    width = Data[0] + 16; // Ensure minimum size
    height = Data[1] + 16;
    ctrl_id = (int)(Data[2] << 8) | Data[3];
    tuning_val = (Size > 4) ? (int)Data[4] : 0;
    cpu_val = (Size > 5) ? (int)Data[5] : 0;

    // Limit dimensions to reasonable values to prevent allocation issues
    if (width > 4096 || height > 4096) return 0;

    // Initialize configuration
    init_config(&cfg, width, height);

    // Try VP8 Encoder
    iface = vpx_codec_vp8_cx();
    if (iface) {
        res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Fuzz VP8E_SET_CPUUSED
            vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_val % 9);
            
            // Fuzz VP8E_SET_TUNING
            vpx_codec_control(&ctx, VP8E_SET_TUNING, tuning_val % 10);

            // Fuzz generic control with random ID (safe range) using the internal function
            // since the macro requires a compile-time constant ID.
            if (ctrl_id > 0 && ctrl_id < 1000) {
                vpx_codec_control_(&ctx, ctrl_id, 0);
            }

            vpx_codec_destroy(&ctx);
        }
    }

    // Try VP8 Decoder (for VP8D_GET_LAST_REF_UPDATES)
    iface = vpx_codec_vp8_dx();
    if (iface) {
        vpx_codec_dec_cfg_t dec_cfg;
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        // Removed allow_highbitdepth as it is not a member of vpx_codec_dec_cfg in this version
        
        res = vpx_codec_dec_init_ver(&ctx, iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Fuzz VP8D_GET_LAST_REF_UPDATES
            int updates[8];
            memset(updates, 0, sizeof(updates));
            vpx_codec_control(&ctx, VP8D_GET_LAST_REF_UPDATES, updates);

            // Fuzz generic control with random ID using internal function
            if (ctrl_id > 0 && ctrl_id < 1000) {
                vpx_codec_control_(&ctx, ctrl_id, 0);
            }

            vpx_codec_destroy(&ctx);
        }
    }

    return 0;
}