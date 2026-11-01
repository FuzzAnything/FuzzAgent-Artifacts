// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_SCALEMODE__value at vp8cx.h:982:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

static int fuzz_init_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg, uint8_t *data, size_t size) {
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface;

    // Try VP8 first
    iface = vpx_codec_vp8_cx();
    if (iface) {
        res = vpx_codec_enc_init_ver(ctx, iface, cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            return 0;
        }
        vpx_codec_destroy(ctx);
        memset(ctx, 0, sizeof(*ctx));
    }

    // Try VP9 if VP8 failed
    iface = vpx_codec_vp9_cx();
    if (iface) {
        res = vpx_codec_enc_init_ver(ctx, iface, cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            return 0;
        }
        vpx_codec_destroy(ctx);
        memset(ctx, 0, sizeof(*ctx));
    }

    return -1;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    int ret = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize configuration with fuzzed values
    cfg.g_w = (Data[0] % 256) + 16; // Minimum 16x16
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_threads = (Size > 2) ? (Data[2] % 16) + 1 : 1;
    cfg.g_profile = (Size > 3) ? (Data[3] % 4) : 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 4) ? (Data[4] % 60) + 1 : 30;
    cfg.g_error_resilient = 0;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = (Size > 5) ? (Data[5] % 32) : 0;

    cfg.rc_dropframe_thresh = (Size > 6) ? (Data[6] % 100) : 0;
    cfg.rc_resize_allowed = (Size > 7) ? (Data[7] % 2) : 0;
    cfg.rc_scaled_width = cfg.g_w;
    cfg.rc_scaled_height = cfg.g_h;
    cfg.rc_resize_up_thresh = (Size > 8) ? (Data[8] % 100) : 0;
    cfg.rc_resize_down_thresh = (Size > 9) ? (Data[9] % 100) : 0;
    cfg.rc_end_usage = (Size > 10) ? (Data[10] % 4) : VPX_VBR;
    cfg.rc_target_bitrate = (Size > 11) ? (Data[11] % 1000) + 1 : 100;
    cfg.rc_min_quantizer = (Size > 12) ? (Data[12] % 64) : 0;
    cfg.rc_max_quantizer = (Size > 13) ? (Data[13] % 64) + 1 : 63;
    cfg.rc_undershoot_pct = (Size > 14) ? (Data[14] % 100) : 0;
    cfg.rc_overshoot_pct = (Size > 15) ? (Data[15] % 100) : 0;
    cfg.rc_buf_sz = (Size > 16) ? (Data[16] % 1000) + 100 : 1000;
    cfg.rc_buf_initial_sz = (Size > 17) ? (Data[17] % 1000) + 100 : 500;
    cfg.rc_buf_optimal_sz = (Size > 18) ? (Data[18] % 1000) + 100 : 500;
    cfg.rc_2pass_vbr_bias_pct = (Size > 19) ? (Data[19] % 100) : 0;
    cfg.rc_2pass_vbr_minsection_pct = (Size > 20) ? (Data[20] % 100) : 0;
    cfg.rc_2pass_vbr_maxsection_pct = (Size > 21) ? (Data[21] % 100) : 0;
    cfg.kf_mode = (Size > 22) ? (Data[22] % 3) : VPX_KF_AUTO;
    cfg.kf_min_dist = (Size > 23) ? (Data[23] % 120) + 1 : 120;
    cfg.kf_max_dist = (Size > 24) ? (Data[24] % 120) + 1 : 120;

    // Initialize encoder
    if (fuzz_init_encoder(&ctx, &cfg, (uint8_t*)Data, Size) != 0) {
        return 0;
    }

    // Fuzz VP8E_SET_CPUUSED
    if (Size > 25) {
        int cpu_used = Data[25] % 9; // 0-8
        vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
    }

    // Fuzz VP8E_SET_TUNING
    if (Size > 26) {
        int tuning = Data[26] % 8; // 0-7
        vpx_codec_control(&ctx, VP8E_SET_TUNING, tuning);
    }

    // Fuzz VP8E_SET_SCALEMODE
    if (Size > 27) {
        vpx_scaling_mode_t scale_mode;
        scale_mode.h_scaling_mode = (Data[27] % 4);
        scale_mode.v_scaling_mode = (Size > 28) ? (Data[28] % 4) : 0;
        vpx_codec_control(&ctx, VP8E_SET_SCALEMODE, &scale_mode);
    }

    // Fuzz VP9E_SET_TUNE_CONTENT (defined in vpx/vpx_encoder.h via vpx_codec.h)
    if (Size > 29) {
        int content_type = Data[29] % 3; // 0: VP9E_CONTENT_DEFAULT, 1: VP9E_CONTENT_SCREEN, 2: VP9E_CONTENT_FILM
        vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, content_type);
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}