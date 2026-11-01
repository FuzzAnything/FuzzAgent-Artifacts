// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    const vpx_codec_iface_t *encoder = NULL;

    // Try to get VP9 encoder interface
    encoder = vpx_codec_vp9_cx();
    if (encoder == NULL) {
        // Fallback to VP8 if VP9 is not available
        encoder = vpx_codec_vp8_cx();
        if (encoder == NULL) {
            return 0;
        }
    }

    // Initialize encoder configuration
    if (vpx_codec_enc_config_default(encoder, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Extract values from input data
    int cpu_used = Data[0] % 9; // 0-8
    int noise_sensitivity = (int)(Data[1] % 100);
    int static_threshold = (int)(Data[2] % 1000);
    int gf_cbr_boost_pct = (int)(Data[3] % 101); // 0-100
    int postencode_drop = (int)(Data[4] % 2); // 0 or 1

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;
    cfg.g_error_resilient = 1;
    cfg.g_lag_in_frames = 0;

    // Initialize encoder
    res = vpx_codec_enc_init(&ctx, encoder, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Invoke target functions in specified order using the macro expansion directly
    // 1. VP8E_SET_CPUUSED__value
    vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);

    // 2. VP8E_SET_NOISE_SENSITIVITY__value
    vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    // 3. VP8E_SET_STATIC_THRESHOLD__value
    vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, static_threshold);

    // 4. VP8E_SET_GF_CBR_BOOST_PCT__value
    vpx_codec_control(&ctx, VP8E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);

    // 5. VP9E_SET_POSTENCODE_DROP__value
    vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, postencode_drop);

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}