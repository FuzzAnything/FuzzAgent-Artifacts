// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_DELTA_Q_UV__value at vp8cx.h:1091:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    if (size < 2) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Configure basic settings
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 100000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    
    // Initialize codec
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Prepare control value from input
    int32_t val = 0;
    if (size >= 4) {
        val = (int32_t)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    } else {
        val = (int32_t)data[0];
    }

    // Fuzz control parameters
    // VP9E_SET_COLOR_RANGE
    vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, val);
    
    // VP9E_SET_MAX_GF_INTERVAL
    vpx_codec_control(&ctx, VP9E_SET_MAX_GF_INTERVAL, val);
    
    // VP9E_SET_DELTA_Q_UV
    vpx_codec_control(&ctx, VP9E_SET_DELTA_Q_UV, val);
    
    // VP9E_SET_ALT_REF_AQ
    vpx_codec_control(&ctx, VP9E_SET_ALT_REF_AQ, val);
    
    // VP9E_SET_QUANTIZER_ONE_PASS
    vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, val);
    
    // VP9E_SET_TARGET_LEVEL
    vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, val);

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file if needed
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Run the fuzzing logic
    fuzz_vp9_encoder(Data, Size);

    return 0;
}