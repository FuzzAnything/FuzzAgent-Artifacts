// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP9E_SET_MAX_INTER_BITRATE_PCT__value at vp8cx.h:1014:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int is_valid_cpu_used(int val) {
    return val >= 0 && val <= 8;
}

static int is_valid_pct(int val) {
    return val >= 0 && val <= 100;
}

static int is_valid_bool(int val) {
    return val == 0 || val == 1;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t ecfg;
    int ret;

    // Initialize encoder config
    memset(&ecfg, 0, sizeof(ecfg));
    ecfg.g_w = 128;
    ecfg.g_h = 128;
    ecfg.g_timebase.num = 1;
    ecfg.g_timebase.den = 30;
    ecfg.rc_target_bitrate = 500;
    ecfg.rc_min_quantizer = 0;
    ecfg.rc_max_quantizer = 63;
    ecfg.g_lag_in_frames = 0;
    ecfg.g_error_resilient = 0;
    ecfg.g_pass = 0;
    ecfg.g_threads = 1;

    // Try to initialize VP8 encoder
    ret = vpx_codec_enc_init(&codec, vpx_codec_vp8_cx(), &ecfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    uint8_t op = Data[0] % 6;
    int32_t value = 0;

    if (Size >= 4) {
        value = (int32_t)(Data[1] | (Data[2] << 8) | (Data[3] << 16) | (Data[4] << 24));
    } else if (Size >= 1) {
        value = (int32_t)Data[1];
    }

    switch (op) {
        case 0: // VP8E_SET_GF_CBR_BOOST_PCT
            if (is_valid_pct(value)) {
                ret = vpx_codec_control(&codec, VP8E_SET_GF_CBR_BOOST_PCT, value);
                (void)ret;
            }
            break;
        case 1: // VP9E_SET_MAX_INTER_BITRATE_PCT
            // Note: This codec is VP8, so this control may fail, but we try it for fuzzing
            if (is_valid_pct(value)) {
                ret = vpx_codec_control(&codec, VP9E_SET_MAX_INTER_BITRATE_PCT, value);
                (void)ret;
            }
            break;
        case 2: // VP8E_SET_CPUUSED
            if (is_valid_cpu_used(value)) {
                ret = vpx_codec_control(&codec, VP8E_SET_CPUUSED, value);
                (void)ret;
            }
            break;
        case 3: // VP8E_SET_MAX_INTRA_BITRATE_PCT
            if (is_valid_pct(value)) {
                ret = vpx_codec_control(&codec, VP8E_SET_MAX_INTRA_BITRATE_PCT, value);
                (void)ret;
            }
            break;
        case 4: // VP9E_SET_POSTENCODE_DROP
            if (is_valid_bool(value)) {
                ret = vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, value);
                (void)ret;
            }
            break;
        case 5: // VP8E_SET_ENABLEAUTOALTREF
            if (is_valid_bool(value)) {
                ret = vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, value);
                (void)ret;
            }
            break;
        default:
            break;
    }

    vpx_codec_destroy(&codec);
    return 0;
}