// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_GET_LAST_QUANTIZER_SVC_LAYERS__value at vp8cx.h:1103:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    int val1, val2, val3, val4;
    int out_val = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize encoder configuration
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;
    cfg.rc_end_usage = VPX_CBR;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize encoder using the VP9 encoder ID
    res = vpx_codec_enc_init_ver(&ctx, vpx_codec_vp9_cx(), &cfg, 0, VPX_CODEC_USE_POSTPROC);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Extract values from input data
    val1 = (int)Data[0];
    val2 = (int)Data[1];
    val3 = (int)Data[2];
    val4 = (int)Data[3];

    // Fuzz VP9E_SET_MAX_GF_INTERVAL
    vpx_codec_control(&ctx, VP9E_SET_MAX_GF_INTERVAL, val1);

    // Fuzz VP9E_SET_POSTENCODE_DROP
    vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, val2);

    // Fuzz VP9E_SET_ROW_MT
    vpx_codec_control(&ctx, VP9E_SET_ROW_MT, val3);

    // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
    vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, val4);

    // Fuzz VP9E_SET_FRAME_PERIODIC_BOOST
    vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, val1);

    // Fuzz VP9E_GET_LAST_QUANTIZER_SVC_LAYERS
    vpx_codec_control(&ctx, VP9E_GET_LAST_QUANTIZER_SVC_LAYERS, &out_val);

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}