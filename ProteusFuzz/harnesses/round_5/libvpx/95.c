// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_CQ_LEVEL__value at vp8cx.h:1008:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "vp8cx.h"
#include "vp8dx.h"

/* 
 * Fuzz driver for libvpx encoder control APIs.
 * Target APIs:
 * VP8E_SET_GF_CBR_BOOST_PCT
 * VP8E_SET_CQ_LEVEL
 * VP8E_SET_STATIC_THRESHOLD
 * VP9E_SET_GF_CBR_BOOST_PCT
 * VP8E_SET_CPUUSED
 * VP8E_SET_ENABLEAUTOALTREF
 */

static int initialize_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg, int is_vp9) {
    vpx_codec_iface_t *codec_iface;
    int err;

    if (is_vp9) {
        codec_iface = vpx_codec_vp9_cx();
    } else {
        codec_iface = vpx_codec_vp8_cx();
    }

    if (!codec_iface) {
        return -1;
    }

    memset(cfg, 0, sizeof(*cfg));
    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 1;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 100;

    err = vpx_codec_enc_config_default(codec_iface, cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    err = vpx_codec_enc_init(ctx, codec_iface, cfg, 0);
    if (err != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret;
    int is_vp9 = 0;
    int value;

    if (Size == 0) {
        return 0;
    }

    /* Use first byte to decide codec (0 for VP8, non-zero for VP9) */
    if (Data[0] & 1) {
        is_vp9 = 1;
    }

    if (initialize_encoder(&ctx, &cfg, is_vp9) != 0) {
        return 0;
    }

    /* Extract a value from the input data for control parameters */
    if (Size >= 2) {
        value = (Data[1] & 0xFF);
    } else {
        value = 0;
    }

    /* VP8E_SET_CPUUSED: 0-8 */
    if (is_vp9 == 0) {
        int cpu_used = value % 9;
        ret = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
        (void)ret;
    }

    /* VP8E_SET_ENABLEAUTOALTREF: 0 or 1 */
    if (is_vp9 == 0) {
        int enable_auto_alt_ref = value % 2;
        ret = vpx_codec_control(&ctx, VP8E_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
        (void)ret;
    }

    /* VP8E_SET_CQ_LEVEL: typically 0-63 or similar, clamp to safe range */
    if (is_vp9 == 0) {
        int cq_level = value % 64;
        ret = vpx_codec_control(&ctx, VP8E_SET_CQ_LEVEL, cq_level);
        (void)ret;
    }

    /* VP8E_SET_GF_CBR_BOOST_PCT: percentage, clamp 0-100 */
    if (is_vp9 == 0) {
        int gf_boost_pct = value % 101;
        ret = vpx_codec_control(&ctx, VP8E_SET_GF_CBR_BOOST_PCT, gf_boost_pct);
        (void)ret;
    }

    /* VP8E_SET_STATIC_THRESHOLD: typically 0-255 or similar */
    if (is_vp9 == 0) {
        int static_thresh = value % 256;
        ret = vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, static_thresh);
        (void)ret;
    }

    /* VP9E_SET_GF_CBR_BOOST_PCT: similar to VP8 */
    if (is_vp9 == 1) {
        int gf_boost_pct = value % 101;
        ret = vpx_codec_control(&ctx, VP9E_SET_GF_CBR_BOOST_PCT, gf_boost_pct);
        (void)ret;
    }

    cleanup_encoder(&ctx);
    return 0;
}