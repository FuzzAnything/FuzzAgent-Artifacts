// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value at vp8cx.h:1093:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP9E_SET_AQ_MODE__value at vp8cx.h:1030:1 in vp8cx.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

// Define the control constants if not present in the included headers
// These are standard libvpx control IDs
#ifndef VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR
#define VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR 0x10000000 + 100
#endif
#ifndef VP8E_SET_CPUUSED
#define VP8E_SET_CPUUSED 0x10000000 + 1
#endif
#ifndef VP9E_SET_AQ_MODE
#define VP9E_SET_AQ_MODE 0x10000000 + 10
#endif
#ifndef VP9E_SET_GF_CBR_BOOST_PCT
#define VP9E_SET_GF_CBR_BOOST_PCT 0x10000000 + 11
#endif
#ifndef VP9E_SET_FRAME_PARALLEL_DECODING
#define VP9E_SET_FRAME_PARALLEL_DECODING 0x10000000 + 12
#endif

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    // Get the VP9 encoder interface
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) {
        return 0;
    }

    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Initialize default configuration
    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Set basic configuration parameters
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_threads = 1;

    // Initialize the encoder
    if (vpx_codec_enc_init(&codec, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Invoke target functions in specified order using vpx_codec_control
    
    // 1. VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR__value
    int disable_overshoot = 0;
    if (size > 0) {
        disable_overshoot = data[0] % 2;
    }
    vpx_codec_control(&codec, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, disable_overshoot);

    // 2. VP8E_SET_CPUUSED__value
    int cpu_used = 0;
    if (size > 1) {
        cpu_used = data[1] % 9; // 0-8
    }
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);

    // 3. VP9E_SET_AQ_MODE__value
    int aq_mode = 0;
    if (size > 2) {
        aq_mode = data[2] % 4; // Assuming 0-3 modes
    }
    vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode);

    // 4. VP9E_SET_GF_CBR_BOOST_PCT__value
    int boost_pct = 0;
    if (size > 3) {
        boost_pct = data[3] % 101; // 0-100
    }
    vpx_codec_control(&codec, VP9E_SET_GF_CBR_BOOST_PCT, boost_pct);

    // 5. VP9E_SET_FRAME_PARALLEL_DECODING__value
    int parallel_decoding = 0;
    if (size > 4) {
        parallel_decoding = data[4] % 2;
    }
    vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, parallel_decoding);

    // Cleanup
    vpx_codec_destroy(&codec);
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    fuzz_vp9_encoder(Data, Size);
    return 0;
}