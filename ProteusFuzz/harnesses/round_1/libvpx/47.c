// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_ARNR_TYPE__value at vp8cx.h:1004:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

static int test_encoder_controls(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return -1;

    // Extract values from fuzz input to diversify test cases
    int arnr_type = (size > 0) ? (int)data[0] % 3 : 0; // Assuming 0-2 range for ARNR type
    int enable_auto_altref = (size > 1) ? (int)data[1] % 2 : 1;
    int static_threshold = (size > 2) ? (int)data[2] % 100 : 50;
    int cpu_used = (size > 3) ? (int)data[3] % 9 : 4; // 0-8 range
    int noise_sensitivity = (size > 4) ? (int)data[4] % 100 : 50;
    int gf_cbr_boost_pct = (size > 5) ? (int)data[5] % 101 : 50;

    // Apply encoder controls
    vpx_codec_err_t ret;

    ret = vpx_codec_control(ctx, VP8E_SET_ARNR_TYPE, arnr_type);
    if (ret != VPX_CODEC_OK) return -1;

    ret = vpx_codec_control(ctx, VP8E_SET_ENABLEAUTOALTREF, enable_auto_altref);
    if (ret != VPX_CODEC_OK) return -1;

    ret = vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, static_threshold);
    if (ret != VPX_CODEC_OK) return -1;

    ret = vpx_codec_control(ctx, VP8E_SET_CPUUSED, cpu_used);
    if (ret != VPX_CODEC_OK) return -1;

    ret = vpx_codec_control(ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    if (ret != VPX_CODEC_OK) return -1;

    ret = vpx_codec_control(ctx, VP8E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);
    if (ret != VPX_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0; // Need at least 6 bytes to generate diverse inputs

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_lag_in_frames = 0;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Test encoder controls with fuzzed data
    if (test_encoder_controls(&ctx, Data, Size) != 0) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    return 0;
}