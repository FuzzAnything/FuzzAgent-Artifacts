// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_AQ_MODE__value at vp8cx.h:1030:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vp8cx.h"
#include "vp8dx.h"

static int fuzzer_test_one_input(const uint8_t *data, size_t size) {
    if (size < 20) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *codec = NULL;
    int ret;

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(NULL, &cfg, 0) < 0) {
        return 0;
    }

    // Use input data to set configuration parameters
    cfg.g_w = data[0] + 16;
    cfg.g_h = data[1] + 16;
    cfg.g_timebase.num = data[2] + 1;
    cfg.g_timebase.den = data[3] + 1;
    cfg.g_error_resilient = (vpx_codec_er_flags_t)(data[4] % 3);
    cfg.rc_target_bitrate = (data[5] * 256 + data[6]) * 1000;
    cfg.g_lag_in_frames = data[7] % 10;
    cfg.rc_end_usage = data[8] % 3; // VBR, CBR, Q

    // Initialize the codec
    codec = vpx_codec_vp9_cx();
    if (!codec) {
        return 0;
    }

    ret = vpx_codec_enc_init(&ctx, codec, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // 1. VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value
    // Assuming it's a control macro that sets a value (0 or 1)
    int disable_overshoot = data[9] % 2;
    ret = vpx_codec_control(&ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, disable_overshoot);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 2. VP8E_SET_CPUUSED__value
    // CPU usage level from 0 to 8
    int cpu_used = data[10] % 9;
    ret = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 3. VP9E_SET_AQ_MODE__value
    // AQ mode: 0 = OFF, 1 = VARIANCE, 2 = COMPLEXITY, 3 = AUTO
    int aq_mode = data[11] % 4;
    ret = vpx_codec_control(&ctx, VP9E_SET_AQ_MODE, aq_mode);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 4. VP9E_SET_GF_CBR_BOOST_PCT__value
    // Boost percentage, assume 0-100
    int boost_pct = data[12] % 101;
    ret = vpx_codec_control(&ctx, VP9E_SET_GF_CBR_BOOST_PCT, boost_pct);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 5. VP9E_SET_FRAME_PARALLEL_DECODING__value
    // Enable/disable parallel decoding (0 or 1)
    int parallel_decoding = data[13] % 2;
    ret = vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, parallel_decoding);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    return fuzzer_test_one_input(Data, Size);
}