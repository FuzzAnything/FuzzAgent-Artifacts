// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP9D_SET_ROW_MT__value at vp8dx.h:216:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int value = *(const int32_t *)Data;
    int mt_threads = (Size > 4) ? *(const int32_t *)(Data + 4) : 1;
    if (mt_threads < 1) mt_threads = 1;
    if (mt_threads > 64) mt_threads = 64;

    // Encoder context
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_iface_t *enc_iface = vpx_codec_vp9_cx();
    if (!enc_iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(enc_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Try VP9E_SET_LOSSLESS
    vpx_codec_control(&enc_ctx, VP9E_SET_LOSSLESS, value);
    // Try VP9E_SET_MAX_GF_INTERVAL
    vpx_codec_control(&enc_ctx, VP9E_SET_MAX_GF_INTERVAL, value);
    // Try VP9E_SET_POSTENCODE_DROP
    vpx_codec_control(&enc_ctx, VP9E_SET_POSTENCODE_DROP, value);
    // Try VP9E_SET_ROW_MT
    vpx_codec_control(&enc_ctx, VP9E_SET_ROW_MT, mt_threads);
    // Try VP9E_SET_FRAME_PERIODIC_BOOST
    vpx_codec_control(&enc_ctx, VP9E_SET_FRAME_PERIODIC_BOOST, value);

    vpx_codec_destroy(&enc_ctx);

    // Decoder context
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    if (!dec_iface) return 0;

    if (vpx_codec_dec_init(&dec_ctx, dec_iface, NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Try VP9D_SET_ROW_MT
    vpx_codec_control(&dec_ctx, VP9D_SET_ROW_MT, mt_threads);

    vpx_codec_destroy(&dec_ctx);

    return 0;
}