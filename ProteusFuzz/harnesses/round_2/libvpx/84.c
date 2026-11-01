// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP9E_SET_AQ_MODE__value at vp8cx.h:1030:1 in vp8cx.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_encoder.h"

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

static vpx_codec_ctx_t *create_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 160;
    cfg.g_h = 120;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t *ctx = create_encoder(Data, Size);
    if (!ctx) return 0;

    int value = Data[0];
    int ret = 0;

    // Fuzz VP8E_SET_CPUUSED (Range 0-8)
    if (value >= 0 && value <= 8) {
        ret = vpx_codec_control(ctx, VP8E_SET_CPUUSED, value);
        (void)ret;
    }

    // Fuzz VP8E_SET_GF_CBR_BOOST_PCT (Range 0-100)
    int pct_val = Data[0] % 101;
    ret = vpx_codec_control(ctx, VP8E_SET_GF_CBR_BOOST_PCT, pct_val);
    (void)ret;

    // Fuzz VP8E_SET_STATIC_THRESHOLD
    int thresh_val = Data[0] % 100;
    ret = vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, thresh_val);
    (void)ret;

    // Fuzz VP9E_SET_AQ_MODE (Range 0-4 typically)
    // Note: This is a VP9 control on a VP8 encoder, likely returns error, which is fine for fuzzing.
    int aq_mode = Data[0] % 5;
    ret = vpx_codec_control(ctx, VP9E_SET_AQ_MODE, aq_mode);
    (void)ret;

    // Fuzz VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR
    int disable_overshoot = Data[0] % 2;
    ret = vpx_codec_control(ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, disable_overshoot);
    (void)ret;

    // Fuzz VP9E_SET_GF_CBR_BOOST_PCT
    int gf_boost = Data[0] % 101;
    ret = vpx_codec_control(ctx, VP9E_SET_GF_CBR_BOOST_PCT, gf_boost);
    (void)ret;

    cleanup_encoder(ctx);
    return 0;
}