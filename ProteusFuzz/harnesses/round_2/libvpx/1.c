// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static int invoke_control(vpx_codec_ctx_t *enc, int id, int value) {
    if (!enc) return -1;
    return vpx_codec_control_(enc, id, value);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    cfg.g_threads = 1;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (vpx_codec_enc_init(&codec, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    int val1 = (Data[0] % 2);
    int val2 = (Data[1] % 9);
    int val3 = (Data[2] % 4);
    int val4 = (Data[3] % 101);
    int val5 = (Data[4] % 2);

    invoke_control(&codec, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, val1);
    invoke_control(&codec, VP8E_SET_CPUUSED, val2);
    invoke_control(&codec, VP9E_SET_AQ_MODE, val3);
    invoke_control(&codec, VP9E_SET_GF_CBR_BOOST_PCT, val4);
    invoke_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, val5);

    vpx_codec_destroy(&codec);

    return 0;
}