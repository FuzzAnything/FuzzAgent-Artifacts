// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int invoke_api_sequence(vpx_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec || !data || size == 0) {
        return -1;
    }

    int disable_loopfilter = 0;
    int row_mt = 0;
    int svc = 0;
    int screen_content_mode = 0;

    if (size > 0) disable_loopfilter = data[0] & 1;
    if (size > 1) row_mt = data[1] & 1;
    if (size > 2) svc = data[2] & 1;
    if (size > 3) screen_content_mode = data[3] & 1;

    if (vpx_codec_control(codec, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter) != 0) {
        return -1;
    }

    if (vpx_codec_control(codec, VP9E_SET_ROW_MT, row_mt) != 0) {
        return -1;
    }

    int row_mt_2 = (size > 4) ? (data[4] & 1) : row_mt;
    if (vpx_codec_control(codec, VP9E_SET_ROW_MT, row_mt_2) != 0) {
        return -1;
    }

    if (vpx_codec_control(codec, VP9E_SET_SVC, svc) != 0) {
        return -1;
    }

    if (vpx_codec_control(codec, VP9E_SET_SVC_PARAMETERS, NULL) != 0) {
        return -1;
    }

    if (vpx_codec_control(codec, VP8E_SET_SCREEN_CONTENT_MODE, screen_content_mode) != 0) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t enc_cfg;

    memset(&enc_cfg, 0, sizeof(enc_cfg));
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.g_error_resilient = 1;
    enc_cfg.rc_target_bitrate = 100;
    enc_cfg.rc_min_quantizer = 0;
    enc_cfg.rc_max_quantizer = 63;

    if (vpx_codec_enc_init(&codec, vpx_codec_vp9_cx(), &enc_cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    invoke_api_sequence(&codec, Data, Size);

    vpx_codec_destroy(&codec);

    return 0;
}