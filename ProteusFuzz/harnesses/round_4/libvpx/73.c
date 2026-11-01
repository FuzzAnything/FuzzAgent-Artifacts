// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

/*
 * Fuzz driver for libvpx VP9 encoder control commands.
 */

static int is_valid_ctrl_id(int ctrl_id) {
    /* List of known VP9 encoder control IDs */
    switch (ctrl_id) {
        case VP9E_SET_DISABLE_LOOPFILTER:
        case VP9E_SET_FRAME_PARALLEL_DECODING:
        case VP9E_GET_LEVEL:
        case VP9E_SET_TARGET_LEVEL:
        case VP9E_SET_NOISE_SENSITIVITY:
            return 1;
        default:
            return 0;
    }
}

static int init_codec_context(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    /* Initialize configuration */
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Use fuzz data to influence configuration slightly, but keep it valid */
    /* Ensure we have enough data before accessing indices */
    if (size >= 7) {
        cfg.g_w = 16 + (data[0] % 1024);
        cfg.g_h = 16 + (data[1] % 1024);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30 + (data[2] % 60);
        /* Use VPX_ERROR_RESILIENT_DEFAULT which is the standard define in vpx_codec.h */
        cfg.g_error_resilient = (data[3] & 0x01) ? VPX_ERROR_RESILIENT_DEFAULT : 0;
        cfg.rc_target_bitrate = 100 + (data[4] % 1000);
        cfg.rc_min_quantizer = 2 + (data[5] % 30);
        cfg.rc_max_quantizer = 30 + (data[6] % 30);
    } else if (size >= 2) {
        /* Minimal valid config if data is short but not empty */
        cfg.g_w = 16 + (data[0] % 1024);
        cfg.g_h = 16 + (data[1] % 1024);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_error_resilient = 0;
        cfg.rc_target_bitrate = 100;
        cfg.rc_min_quantizer = 2;
        cfg.rc_max_quantizer = 30;
    } else {
        /* If size is 0 or 1, just use defaults */
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_error_resilient = 0;
        cfg.rc_target_bitrate = 100;
        cfg.rc_min_quantizer = 2;
        cfg.rc_max_quantizer = 30;
    }

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    int ctrl_id;
    int value;
    int level;

    /* Basic sanity check */
    if (Size < 2) {
        return 0;
    }

    /* Initialize codec context */
    if (!init_codec_context(&ctx, Data, Size)) {
        return 0;
    }

    /* Extract control ID and value from input data */
    ctrl_id = (int)Data[0];
    value = (int)Data[1];

    /* Try to exercise various control commands */
    /* Case 1: Use extracted ctrl_id if valid */
    if (is_valid_ctrl_id(ctrl_id)) {
        res = vpx_codec_control_(&ctx, ctrl_id, value);
        (void)res;
    }

    /* Case 2: Explicitly test known control IDs with fuzzed values */
    /* VP9E_SET_DISABLE_LOOPFILTER */
    res = vpx_codec_control_(&ctx, VP9E_SET_DISABLE_LOOPFILTER, value & 0x01);
    (void)res;

    /* VP9E_SET_FRAME_PARALLEL_DECODING */
    res = vpx_codec_control_(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, (value >> 1) & 0x01);
    (void)res;

    /* VP9E_SET_NOISE_SENSITIVITY */
    res = vpx_codec_control_(&ctx, VP9E_SET_NOISE_SENSITIVITY, value & 0xFF);
    (void)res;

    /* VP9E_SET_TARGET_LEVEL */
    /* Level is usually 0-6, clamp it */
    int target_level = value & 0x07;
    res = vpx_codec_control_(&ctx, VP9E_SET_TARGET_LEVEL, target_level);
    (void)res;

    /* VP9E_GET_LEVEL */
    /* This command usually returns a value via the argument */
    level = 0;
    res = vpx_codec_control_(&ctx, VP9E_GET_LEVEL, &level);
    (void)res;
    (void)level;

    /* Cleanup */
    vpx_codec_destroy(&ctx);

    return 0;
}