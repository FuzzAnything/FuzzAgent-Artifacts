// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_image.h>

/*
 * Fuzz driver for libvpx encoder API sequence:
 * vpx_img_alloc -> vpx_codec_iface_name -> vpx_codec_enc_config_default ->
 * vpx_codec_enc_init_ver -> vpx_codec_control_ -> vpx_img_free -> vpx_codec_destroy
 */

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Extract parameters from fuzz input */
    uint32_t width = ((uint32_t)Data[0] << 8) | Data[1];
    uint32_t height = ((uint32_t)Data[2] << 8) | Data[3];
    uint32_t align = (Size > 4) ? Data[4] : 16;
    uint32_t fmt_idx = (Size > 5) ? Data[5] % 10 : 1;

    /* Clamp dimensions to safe limits (max 2^27 as per spec) */
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (align == 0) align = 16;
    if (align > 65536) align = 65536;

    /* Map fmt_idx to a valid vpx_img_fmt_t */
    vpx_img_fmt_t fmt = VPX_IMG_FMT_YV12;
    switch (fmt_idx) {
        case 0: fmt = VPX_IMG_FMT_NONE; break;
        case 1: fmt = VPX_IMG_FMT_YV12; break;
        case 2: fmt = VPX_IMG_FMT_I420; break;
        case 3: fmt = VPX_IMG_FMT_I422; break;
        case 4: fmt = VPX_IMG_FMT_I444; break;
        case 5: fmt = VPX_IMG_FMT_I440; break;
        case 6: fmt = VPX_IMG_FMT_NV12; break;
        case 7: fmt = VPX_IMG_FMT_I42016; break;
        case 8: fmt = VPX_IMG_FMT_I42216; break;
        case 9: fmt = VPX_IMG_FMT_I44416; break;
        default: fmt = VPX_IMG_FMT_YV12; break;
    }

    /* 1. vpx_img_alloc */
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        return 0;
    }

    /* 2. vpx_codec_iface_name */
    /* We use the VP8 encoder interface */
    const vpx_codec_iface_t *iface = &vpx_codec_vp8_cx;
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; /* Use to avoid unused warning */

    /* 3. vpx_codec_enc_config_default */
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    /* Set configuration based on image dimensions */
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;

    cfg.rc_end_usage = VPX_VBR;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 63;
    cfg.rc_buf_sz = 1000;
    cfg.rc_buf_initial_sz = 500;
    cfg.rc_buf_optimal_sz = 750;

    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = 120;
    cfg.kf_max_dist = 240;

    /* 4. vpx_codec_enc_init_ver */
    vpx_codec_ctx_t ctx;
    vpx_codec_flags_t flags = 0;
    /* Use the macro to ensure ABI version is set correctly, but we call the function directly as requested */
    err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }

    /* 5. vpx_codec_control_ */
    /* Try a few control commands to explore state space */
    /* VP8E_SET_CPUUSED */
    int cpu_used = (Size > 6) ? (Data[6] % 8) : 4;
    vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, cpu_used);

    /* VP8E_SET_STATIC_THRESHOLD */
    int static_thresh = (Size > 7) ? (Data[7] % 100) : 50;
    vpx_codec_control_(&ctx, VP8E_SET_STATIC_THRESHOLD, static_thresh);

    /* 6. vpx_img_free */
    vpx_img_free(img);

    /* 7. vpx_codec_destroy */
    vpx_codec_destroy(&ctx);

    return 0;
}