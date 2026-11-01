// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8.h>

/*
 * Fuzz driver for libvpx encoder/decoder initialization and control APIs.
 * Targets:
 *   - vpx_codec_enc_init_ver
 *   - vpx_codec_dec_init_ver
 *   - vpx_codec_control_
 *
 * Note: VP9_GET_REFERENCE__value, VP8_COPY_REFERENCE__value, and VP8_SET_REFERENCE__value
 * are not implemented in the provided API set and are skipped.
 */

static vpx_codec_iface_t *get_encoder_iface(const uint8_t *Data, size_t Size) {
    if (Size > 0 && Data[0] % 2 == 0) {
        return vpx_codec_vp8_cx();
    }
    return vpx_codec_vp9_cx();
}

static vpx_codec_iface_t *get_decoder_iface(const uint8_t *Data, size_t Size) {
    if (Size > 0 && Data[0] % 2 == 0) {
        return vpx_codec_vp8_dx();
    }
    return vpx_codec_vp9_dx();
}

static void init_enc_cfg(vpx_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    cfg->g_w = 16;
    cfg->g_h = 16;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    
    cfg->rc_end_usage = VPX_VBR;
    cfg->rc_target_bitrate = 100;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 63;
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 750;
    
    cfg->kf_mode = VPX_KF_DISABLED;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 0;
    
    if (Size > 0) {
        cfg->g_w = (Data[0] % 100) + 16;
        cfg->g_h = (Data[1 % Size] % 100) + 16;
        cfg->rc_target_bitrate = (Data[2 % Size] % 1000) + 100;
        cfg->rc_min_quantizer = Data[3 % Size] % 64;
        cfg->rc_max_quantizer = (Data[4 % Size] % 63) + cfg->rc_min_quantizer;
    }
}

static void init_dec_cfg(vpx_codec_dec_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    cfg->threads = 1;
    cfg->w = 16;
    cfg->h = 16;
    
    if (Size > 0) {
        cfg->w = (Data[0] % 100) + 16;
        cfg->h = (Data[1 % Size] % 100) + 16;
        cfg->threads = (Data[2 % Size] % 4) + 1;
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* 
     * Scenario 1: Test Encoder Initialization
     */
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_iface_t *iface;
        vpx_codec_err_t res;

        memset(&ctx, 0, sizeof(ctx));
        init_enc_cfg(&cfg, Data, Size);
        
        iface = get_encoder_iface(Data, Size);
        if (!iface) return 0;

        /* Try with valid ABI version */
        res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        /* Cleanup if initialized successfully */
        if (res == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }

        /* Try with invalid ABI version to trigger ABI_MISMATCH */
        memset(&ctx, 0, sizeof(ctx));
        res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION + 1);
        if (res == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }
        
        /* Try with invalid flags */
        memset(&ctx, 0, sizeof(ctx));
        res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, (vpx_codec_flags_t)0xFFFFFFFF, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }
    }

    /* 
     * Scenario 2: Test Decoder Initialization
     */
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_dec_cfg_t cfg;
        vpx_codec_iface_t *iface;
        vpx_codec_err_t res;

        memset(&ctx, 0, sizeof(ctx));
        init_dec_cfg(&cfg, Data, Size);
        
        iface = get_decoder_iface(Data, Size);
        if (!iface) return 0;

        /* Try with valid ABI version */
        res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }

        /* Try with invalid ABI version */
        memset(&ctx, 0, sizeof(ctx));
        res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION + 1);
        if (res == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }
        
        /* Try with NULL config (allowed by API) */
        memset(&ctx, 0, sizeof(ctx));
        res = vpx_codec_dec_init_ver(&ctx, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
        }
    }

    /* 
     * Scenario 3: Test Control API on a valid encoder
     */
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_iface_t *iface;
        vpx_codec_err_t res;

        memset(&ctx, 0, sizeof(ctx));
        init_enc_cfg(&cfg, Data, Size);
        iface = get_encoder_iface(Data, Size);
        if (!iface) return 0;

        res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            /* If init failed, we can't test control, but that's expected for fuzzing */
            return 0;
        }

        /* Try various control IDs, some valid, some invalid */
        int ctrl_ids[] = {
            VP8E_SET_STATIC_THRESHOLD,
            VP9E_SET_TPL,
            0x12345678, /* Invalid ID */
            -1,         /* Negative ID */
        };
        size_t num_ctrls = sizeof(ctrl_ids) / sizeof(ctrl_ids[0]);

        for (size_t i = 0; i < num_ctrls; i++) {
            res = vpx_codec_control_(&ctx, ctrl_ids[i]);
            (void)res; 
        }

        vpx_codec_destroy(&ctx);
    }

    /* 
     * Scenario 4: Test Control API on a valid decoder
     */
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_dec_cfg_t cfg;
        vpx_codec_iface_t *iface;
        vpx_codec_err_t res;

        memset(&ctx, 0, sizeof(ctx));
        init_dec_cfg(&cfg, Data, Size);
        iface = get_decoder_iface(Data, Size);
        if (!iface) return 0;

        res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            return 0;
        }

        /* Try various control IDs */
        int ctrl_ids[] = {
            VP8_SET_REFERENCE,
            0x12345678, /* Invalid ID */
            -1,         /* Negative ID */
        };
        size_t num_ctrls = sizeof(ctrl_ids) / sizeof(ctrl_ids[0]);

        for (size_t i = 0; i < num_ctrls; i++) {
            res = vpx_codec_control_(&ctx, ctrl_ids[i]);
            (void)res;
        }

        vpx_codec_destroy(&ctx);
    }

    return 0;
}