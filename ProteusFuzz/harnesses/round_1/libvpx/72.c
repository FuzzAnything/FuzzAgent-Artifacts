// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

// Callback function for frame completion
// Signature: void cb(void *user_priv, const vpx_image_t *img)
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

// Callback function for slice completion
// Signature: void cb(void *user_priv, const vpx_image_t *img,
//                    const vpx_image_rect_t *rect, const vpx_image_rect_t *rect_out)
static void put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *rect, const vpx_image_rect_t *rect_out) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect_out;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Get decoder and encoder interfaces
    vpx_codec_iface_t *dec_iface = vpx_codec_vp8_dx();
    vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();

    if (!dec_iface || !enc_iface) {
        return 0;
    }

    // Test vpx_codec_get_caps
    vpx_codec_caps_t dec_caps = vpx_codec_get_caps(dec_iface);
    vpx_codec_caps_t enc_caps = vpx_codec_get_caps(enc_iface);
    (void)dec_caps;
    (void)enc_caps;

    // Initialize decoder context
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;

    if (vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize encoder context
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    enc_cfg.g_w = 128;
    enc_cfg.g_h = 128;
    enc_cfg.g_threads = 1;
    enc_cfg.g_profile = 0;
    enc_cfg.g_bit_depth = VPX_BITS_8;
    enc_cfg.g_input_bit_depth = 8;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_error_resilient = 0;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.rc_dropframe_thresh = 0;
    enc_cfg.rc_resize_allowed = 0;
    enc_cfg.rc_scaled_width = 0;
    enc_cfg.rc_scaled_height = 0;
    enc_cfg.rc_resize_up_thresh = 0;
    enc_cfg.rc_resize_down_thresh = 0;
    enc_cfg.rc_end_usage = VPX_VBR;
    enc_cfg.rc_target_bitrate = 100;
    enc_cfg.rc_min_quantizer = 2;
    enc_cfg.rc_max_quantizer = 63;
    enc_cfg.rc_undershoot_pct = 10;
    enc_cfg.rc_overshoot_pct = 10;
    enc_cfg.rc_buf_sz = 1000;
    enc_cfg.rc_buf_initial_sz = 500;
    enc_cfg.rc_buf_optimal_sz = 750;
    enc_cfg.rc_2pass_vbr_bias_pct = 0;
    enc_cfg.rc_2pass_vbr_minsection_pct = 0;
    enc_cfg.rc_2pass_vbr_maxsection_pct = 0;
    enc_cfg.kf_mode = VPX_KF_DISABLED;
    enc_cfg.kf_min_dist = 0;
    enc_cfg.kf_max_dist = 0;
    enc_cfg.ss_number_layers = 0;
    enc_cfg.ts_number_layers = 0;
    enc_cfg.ts_periodicity = 0;
    enc_cfg.temporal_layering_mode = 0;
    enc_cfg.use_vizier_rc_params = 0;

    if (vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&dec_ctx);
        return 0;
    }

    // Test vpx_codec_register_put_frame_cb
    vpx_codec_register_put_frame_cb(&dec_ctx, put_frame_cb, (void *)Data);
    vpx_codec_register_put_frame_cb(&enc_ctx, put_frame_cb, (void *)Data);

    // Test vpx_codec_register_put_slice_cb
    vpx_codec_register_put_slice_cb(&dec_ctx, put_slice_cb, (void *)Data);
    vpx_codec_register_put_slice_cb(&enc_ctx, put_slice_cb, (void *)Data);

    // Test vpx_codec_get_global_headers (only valid for VP9, but we test with VP8 context)
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&enc_ctx);
    (void)global_headers;

    // Cleanup
    vpx_codec_destroy(&dec_ctx);
    vpx_codec_destroy(&enc_ctx);

    return 0;
}