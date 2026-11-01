// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// VP9D_SET_ROW_MT__value at vp8dx.h:216:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>

static int fuzzer_test_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    int ret;
    int lossless;
    int mt;
    int max_gf;
    int boost;
    int drop;

    if (size < 1) return 0;

    iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;

    ret = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != 0) return 0;

    if (size >= 1) lossless = (data[0] % 2);
    if (size >= 2) mt = (data[1] % 2);
    if (size >= 3) max_gf = data[2] % 100;
    if (size >= 4) boost = data[3] % 2;
    if (size >= 5) drop = data[4] % 2;

    if (vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf) != 0) {
        // Ignore errors
    }

    if (vpx_codec_control(&codec, VP9E_SET_LOSSLESS, lossless) != 0) {
        // Ignore errors
    }

    if (vpx_codec_control(&codec, VP9E_SET_ROW_MT, mt) != 0) {
        // Ignore errors
    }

    if (vpx_codec_control(&codec, VP9E_SET_FRAME_PERIODIC_BOOST, boost) != 0) {
        // Ignore errors
    }

    if (vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, drop) != 0) {
        // Ignore errors
    }

    vpx_codec_destroy(&codec);
    return 0;
}

static int fuzzer_test_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface;
    int ret;
    int mt;

    if (size < 1) return 0;

    iface = vpx_codec_vp9_dx();
    if (!iface) return 0;

    ret = vpx_codec_dec_init(&codec, iface, NULL, 0);
    if (ret != 0) return 0;

    mt = data[0] % 2;

    if (vpx_codec_control(&codec, VP9D_SET_ROW_MT, mt) != 0) {
        // Ignore errors
    }

    vpx_codec_destroy(&codec);
    return 0;
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzzer_test_encoder(Data, Size);
    fuzzer_test_decoder(Data, Size);

    return 0;
}