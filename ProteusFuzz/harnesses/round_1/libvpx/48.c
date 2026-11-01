// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_DELTA_Q_UV__value at vp8cx.h:1091:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

static vpx_codec_ctx_t codec;
static vpx_codec_enc_cfg_t cfg;
static int codec_initialized = 0;

static void cleanup_codec(void) {
    if (codec_initialized) {
        vpx_codec_destroy(&codec);
        codec_initialized = 0;
    }
}

static int init_codec(void) {
    int ret;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (!iface) {
        return -1;
    }

    memset(&cfg, 0, sizeof(cfg));
    ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = VPX_CBR;
    cfg.rc_target_bitrate = 100;

    ret = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return -1;
    }

    codec_initialized = 1;
    return 0;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cleanup_codec();

    if (init_codec() != 0) {
        return 0;
    }

    int val;
    int ret;

    // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
    if (Size >= 1) {
        val = Data[0];
        ret = vpx_codec_control(&codec, VP9E_SET_QUANTIZER_ONE_PASS, val);
        (void)ret;
    }

    // Fuzz VP9E_SET_DELTA_Q_UV
    if (Size >= 2) {
        val = Data[1];
        ret = vpx_codec_control(&codec, VP9E_SET_DELTA_Q_UV, val);
        (void)ret;
    }

    // Fuzz VP9E_SET_MAX_GF_INTERVAL
    if (Size >= 3) {
        val = Data[2];
        ret = vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, val);
        (void)ret;
    }

    // Fuzz VP9E_SET_TILE_ROWS
    if (Size >= 4) {
        val = Data[3];
        ret = vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, val);
        (void)ret;
    }

    // Fuzz VP9E_SET_TARGET_LEVEL
    if (Size >= 5) {
        val = Data[4];
        ret = vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, val);
        (void)ret;
    }

    // Fuzz VP9E_SET_COLOR_RANGE
    if (Size >= 6) {
        val = Data[5];
        ret = vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, val);
        (void)ret;
    }

    cleanup_codec();
    return 0;
}