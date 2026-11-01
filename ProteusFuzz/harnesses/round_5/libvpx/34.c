// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_MIN_GF_INTERVAL__value at vp8cx.h:1052:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (codec_iface == NULL) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;

    err = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 0;

    err = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    int val = (int)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    int choice = (int)(Data[4] % 6);

    switch (choice) {
        case 0:
            vpx_codec_control(&ctx, VP9E_SET_MAX_GF_INTERVAL, val);
            break;
        case 1:
            vpx_codec_control(&ctx, VP9E_SET_MIN_GF_INTERVAL, val);
            break;
        case 2:
            vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, val);
            break;
        case 3:
            vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, val);
            break;
        case 4:
            vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, val);
            break;
        case 5:
            vpx_codec_control(&ctx, VP9E_SET_LOSSLESS, val);
            break;
        default:
            break;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}