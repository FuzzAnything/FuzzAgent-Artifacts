// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_MIN_GF_INTERVAL__value at vp8cx.h:1052:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_LOSSLESS__value at vp8cx.h:1022:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzz_vp9_encoder_controls(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) {
        return 0;
    }

    int ret = 0;
    int val;

    // Helper to extract an integer from the fuzz data
    #define GET_INT(ptr, len) \
        ((len >= sizeof(int)) ? (val = *(const int *)(ptr), ptr += sizeof(int), len -= sizeof(int)) : 0)

    const uint8_t *ptr = data;
    size_t len = size;

    // VP9E_SET_MAX_GF_INTERVAL
    if (len >= sizeof(int)) {
        val = *(const int *)ptr;
        ptr += sizeof(int);
        len -= sizeof(int);
        ret = vpx_codec_control(ctx, VP9E_SET_MAX_GF_INTERVAL, val);
        (void)ret;
    }

    // VP9E_SET_MIN_GF_INTERVAL
    if (len >= sizeof(int)) {
        val = *(const int *)ptr;
        ptr += sizeof(int);
        len -= sizeof(int);
        ret = vpx_codec_control(ctx, VP9E_SET_MIN_GF_INTERVAL, val);
        (void)ret;
    }

    // VP9E_SET_TARGET_LEVEL
    if (len >= sizeof(int)) {
        val = *(const int *)ptr;
        ptr += sizeof(int);
        len -= sizeof(int);
        ret = vpx_codec_control(ctx, VP9E_SET_TARGET_LEVEL, val);
        (void)ret;
    }

    // VP9E_SET_FRAME_PERIODIC_BOOST
    if (len >= sizeof(int)) {
        val = *(const int *)ptr;
        ptr += sizeof(int);
        len -= sizeof(int);
        ret = vpx_codec_control(ctx, VP9E_SET_FRAME_PERIODIC_BOOST, val);
        (void)ret;
    }

    // VP9E_SET_POSTENCODE_DROP
    if (len >= sizeof(int)) {
        val = *(const int *)ptr;
        ptr += sizeof(int);
        len -= sizeof(int);
        ret = vpx_codec_control(ctx, VP9E_SET_POSTENCODE_DROP, val);
        (void)ret;
    }

    // VP9E_SET_LOSSLESS
    if (len >= sizeof(int)) {
        val = *(const int *)ptr;
        ptr += sizeof(int);
        len -= sizeof(int);
        ret = vpx_codec_control(ctx, VP9E_SET_LOSSLESS, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    vpx_codec_iface_t *codec_iface;
    vpx_codec_enc_cfg_t enc_cfg;

    // Initialize encoder config
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_error_resilient = 1;
    enc_cfg.rc_target_bitrate = 100;
    enc_cfg.rc_min_quantizer = 0;
    enc_cfg.rc_max_quantizer = 63;

    // Select VP9 encoder
    codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) {
        return 0;
    }

    res = vpx_codec_enc_config_default(codec_iface, &enc_cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    res = vpx_codec_enc_init(&ctx, codec_iface, &enc_cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_vp9_encoder_controls(&ctx, Data, Size);

    // Cleanup
    vpx_codec_destroy(&ctx);

    return 0;
}