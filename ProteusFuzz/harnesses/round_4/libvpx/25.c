// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_COLOR_SPACE__value at vp8cx.h:1050:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int test_vp9_encoder_controls(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    int ret;
    int val;

    if (!iface) return 0;

    ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    ret = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    if (size > 0) val = data[0]; else val = 0;
    vpx_codec_control(&ctx, VP9E_SET_TILE_ROWS, val);

    if (size > 1) val = data[1]; else val = 0;
    vpx_codec_control(&ctx, VP9E_SET_COLOR_SPACE, val);

    if (size > 2) val = data[2]; else val = 0;
    vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, val);

    if (size > 3) val = data[3]; else val = 0;
    vpx_codec_control(&ctx, VP9E_SET_ALT_REF_AQ, val);

    if (size > 4) val = data[4]; else val = 0;
    vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, val);

    if (size > 5) val = data[5]; else val = 0;
    vpx_codec_control(&ctx, VP8E_SET_CPUUSED, val);

    vpx_codec_destroy(&ctx);
    return 1;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    test_vp9_encoder_controls(Data, Size);
    return 0;
}