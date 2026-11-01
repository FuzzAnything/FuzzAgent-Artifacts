// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void init_enc_cfg(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    cfg->g_w = size > 0 ? data[0] & 0x3F : 16;
    cfg->g_h = size > 1 ? data[1] & 0x3F : 16;
    cfg->g_w = cfg->g_w < 8 ? 8 : cfg->g_w;
    cfg->g_h = cfg->g_h < 8 ? 8 : cfg->g_h;
    cfg->g_threads = size > 2 ? data[2] & 0x03 : 1;
    cfg->g_bit_depth = size > 3 ? (vpx_bit_depth_t)(data[3] & 0x03) : VPX_BITS_8;
    if (cfg->g_bit_depth != VPX_BITS_8 && cfg->g_bit_depth != VPX_BITS_10 && cfg->g_bit_depth != VPX_BITS_12) {
        cfg->g_bit_depth = VPX_BITS_8;
    }
    cfg->g_input_bit_depth = cfg->g_bit_depth;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = size > 4 ? (data[4] & 0x1F) + 1 : 30;
    cfg->g_pass = size > 5 ? (enum vpx_enc_pass)(data[5] & 0x03) : VPX_RC_ONE_PASS;
    cfg->rc_end_usage = size > 6 ? (enum vpx_rc_mode)(data[6] & 0x03) : VPX_VBR;
    cfg->rc_target_bitrate = size > 7 ? (data[7] & 0xFF) + 1 : 100;
    cfg->kf_mode = size > 8 ? (enum vpx_kf_mode)(data[8] & 0x03) : VPX_KF_DISABLED;
    cfg->kf_min_dist = size > 9 ? (data[9] & 0xFF) : 120;
    cfg->kf_max_dist = size > 10 ? (data[10] & 0xFF) : 120;
    cfg->rc_min_quantizer = size > 11 ? (data[11] & 0x3F) : 2;
    cfg->rc_max_quantizer = size > 12 ? (data[12] & 0x3F) : 63;
    cfg->rc_buf_sz = size > 13 ? (data[13] & 0xFF) * 100 : 2000;
    cfg->rc_buf_initial_sz = size > 14 ? (data[14] & 0xFF) * 100 : 1000;
    cfg->rc_buf_optimal_sz = size > 15 ? (data[15] & 0xFF) * 100 : 1500;
}

static void init_dec_cfg(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    cfg->threads = size > 0 ? data[0] & 0x03 : 1;
    cfg->w = size > 1 ? data[1] & 0x3F : 16;
    cfg->h = size > 2 ? data[2] & 0x3F : 16;
    cfg->w = cfg->w < 8 ? 8 : cfg->w;
    cfg->h = cfg->h < 8 ? 8 : cfg->h;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface_enc = NULL;
    vpx_codec_iface_t *iface_dec = NULL;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_err_t res;
    const char *err_detail;

    // Initialize configurations based on input data
    init_enc_cfg(&enc_cfg, Data, Size);
    init_dec_cfg(&dec_cfg, Data, Size);

    // Attempt to get encoder interface
    iface_enc = vpx_codec_vp9_cx();
    // Attempt to get decoder interface
    iface_dec = vpx_codec_vp9_dx();

    // Fuzz Encoder Initialization
    if (iface_enc) {
        res = vpx_codec_enc_init_ver(&ctx, iface_enc, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Try control commands if initialized successfully
            // VP9E_SET_TUNE_CONTENT is a valid control ID
            // Use correct enum values: VP9E_CONTENT_FILM, VP9E_CONTENT_SCREEN
            res = vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_FILM);
            res = vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_SCREEN);
            
            // Try invalid control ID
            res = vpx_codec_control_(&ctx, 99999);

            // Cleanup encoder
            vpx_codec_destroy(&ctx);
        } else {
            // Check error details on failure
            err_detail = vpx_codec_error_detail(&ctx);
            (void)err_detail;
        }
    }

    // Fuzz Decoder Initialization
    if (iface_dec) {
        // Re-initialize context for decoder
        memset(&ctx, 0, sizeof(vpx_codec_ctx_t));
        
        res = vpx_codec_dec_init_ver(&ctx, iface_dec, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Try control commands
            // Try a generic invalid control to trigger error paths
            res = vpx_codec_control_(&ctx, 12345);

            // Cleanup decoder
            vpx_codec_destroy(&ctx);
        } else {
            // Check error details on failure
            err_detail = vpx_codec_error_detail(&ctx);
            (void)err_detail;
        }
    }

    return 0;
}