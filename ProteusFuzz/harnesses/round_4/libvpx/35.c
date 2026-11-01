// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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

/* Callback function types for fuzzing matching libvpx signatures */
static void dummy_put_frame_cb(void *ctx, const vpx_image_t *frame) {
    (void)ctx;
    (void)frame;
}

static void dummy_put_slice_cb(void *ctx, const vpx_image_t *frame,
                               const vpx_image_rect_t *rect,
                               const vpx_image_rect_t *slice_rect) {
    (void)ctx;
    (void)frame;
    (void)rect;
    (void)slice_rect;
}

/* Helper to safely initialize encoder config with fuzzed data */
static void init_enc_cfg_fuzz(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    /* Check minimum size for width and height (4 bytes each) */
    if (size >= 8) {
        uint32_t w_val = 0;
        uint32_t h_val = 0;
        
        /* Safe byte-by-byte copy to avoid alignment issues */
        memcpy(&w_val, data, sizeof(uint32_t));
        memcpy(&h_val, data + 4, sizeof(uint32_t));
        
        cfg->g_w = w_val;
        cfg->g_h = h_val;
        
        /* Sanity checks */
        if (cfg->g_w > 4096) cfg->g_w = 128;
        if (cfg->g_h > 4096) cfg->g_h = 128;
        if (cfg->g_w == 0) cfg->g_w = 128;
        if (cfg->g_h == 0) cfg->g_h = 128;
    } else {
        cfg->g_w = 128;
        cfg->g_h = 128;
    }

    if (size >= 12) {
        uint8_t bd_val = data[8];
        uint8_t th_val = data[12];
        uint8_t pr_val = data[16];
        
        cfg->g_bit_depth = (vpx_bit_depth_t)(bd_val & 0x3);
        if (cfg->g_bit_depth != VPX_BITS_8 && cfg->g_bit_depth != VPX_BITS_10 && cfg->g_bit_depth != VPX_BITS_12) {
            cfg->g_bit_depth = VPX_BITS_8;
        }
        
        cfg->g_threads = th_val;
        if (cfg->g_threads > 16) cfg->g_threads = 1;
        if (cfg->g_threads == 0) cfg->g_threads = 1;
        
        cfg->g_profile = pr_val;
    } else {
        cfg->g_bit_depth = VPX_BITS_8;
        cfg->g_threads = 1;
    }

    if (size >= 24) {
        uint32_t br_val = 0;
        memcpy(&br_val, data + 20, sizeof(uint32_t));
        cfg->rc_target_bitrate = br_val;
        if (cfg->rc_target_bitrate == 0) cfg->rc_target_bitrate = 100;
    } else {
        cfg->rc_target_bitrate = 100;
    }

    if (size >= 28) {
        cfg->rc_end_usage = (enum vpx_rc_mode)(data[24] & 0x3);
    }

    if (size >= 32) {
        cfg->kf_mode = (enum vpx_kf_mode)(data[28] & 0x3);
    }

    if (size >= 34) {
        cfg->g_timebase.num = data[32];
        cfg->g_timebase.den = data[33];
        if (cfg->g_timebase.den == 0) cfg->g_timebase.den = 1;
    } else {
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
    }
}

/* Helper to safely initialize decoder config with fuzzed data */
static void init_dec_cfg_fuzz(vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    
    if (size >= 1) {
        cfg->threads = data[0];
        if (cfg->threads > 16) cfg->threads = 1;
        if (cfg->threads == 0) cfg->threads = 1;
    } else {
        cfg->threads = 1;
    }

    if (size >= 12) {
        uint32_t w_val = 0;
        uint32_t h_val = 0;
        
        memcpy(&w_val, data + 4, sizeof(uint32_t));
        memcpy(&h_val, data + 8, sizeof(uint32_t));
        
        cfg->w = w_val;
        cfg->h = h_val;
        
        if (cfg->w > 4096) cfg->w = 128;
        if (cfg->h > 4096) cfg->h = 128;
        if (cfg->w == 0) cfg->w = 128;
        if (cfg->h == 0) cfg->h = 128;
    } else {
        cfg->w = 128;
        cfg->h = 128;
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(vpx_codec_ctx_t));

    /* Get codec interfaces */
    const vpx_codec_iface_t *vp8_enc_iface = vpx_codec_vp8_cx();
    const vpx_codec_iface_t *vp8_dec_iface = vpx_codec_vp8_dx();
    const vpx_codec_iface_t *vp9_enc_iface = vpx_codec_vp9_cx();
    const vpx_codec_iface_t *vp9_dec_iface = vpx_codec_vp9_dx();

    /* Test vpx_codec_get_caps with various interfaces */
    vpx_codec_caps_t caps;
    caps = vpx_codec_get_caps(NULL);
    caps = vpx_codec_get_caps(vp8_enc_iface);
    caps = vpx_codec_get_caps(vp8_dec_iface);
    caps = vpx_codec_get_caps(vp9_enc_iface);
    caps = vpx_codec_get_caps(vp9_dec_iface);

    /* Test Encoder Initialization and Global Headers */
    vpx_codec_enc_cfg_t enc_cfg;
    init_enc_cfg_fuzz(&enc_cfg, Data, Size);

    vpx_codec_err_t err;
    err = vpx_codec_enc_init_ver(&ctx, vp8_enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&ctx);
        (void)headers;
        vpx_codec_destroy(&ctx);
    }

    err = vpx_codec_enc_init_ver(&ctx, vp9_enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&ctx);
        (void)headers;
        vpx_codec_destroy(&ctx);
    }

    /* Test Decoder Initialization and Callbacks */
    vpx_codec_dec_cfg_t dec_cfg;
    init_dec_cfg_fuzz(&dec_cfg, Data, Size);

    /* Initialize VP8 Decoder */
    err = vpx_codec_dec_init_ver(&ctx, vp8_dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        /* Test register_put_frame_cb */
        err = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, (void*)Data);
        (void)err;
        
        /* Test register_put_slice_cb */
        err = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void*)Data);
        (void)err;
        
        vpx_codec_destroy(&ctx);
    }

    /* Initialize VP9 Decoder */
    err = vpx_codec_dec_init_ver(&ctx, vp9_dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        /* Test register_put_frame_cb */
        err = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, (void*)Data);
        (void)err;
        
        /* Test register_put_slice_cb */
        err = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void*)Data);
        (void)err;
        
        vpx_codec_destroy(&ctx);
    }

    /* Test error paths: uninitialized context */
    memset(&ctx, 0, sizeof(vpx_codec_ctx_t));
    err = vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, (void*)Data);
    (void)err;
    err = vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void*)Data);
    (void)err;
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&ctx);
    (void)headers;

    /* Test with NULL callbacks */
    vpx_codec_enc_cfg_t enc_cfg2;
    init_enc_cfg_fuzz(&enc_cfg2, Data, Size);
    err = vpx_codec_enc_init_ver(&ctx, vp8_enc_iface, &enc_cfg2, 0, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
    }

    return 0;
}