// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
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
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8.h>

static int fuzz_vp9_encoder_settings(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *codec_iface;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) {
        return 0;
    }

    ret = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    ret = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    uint8_t *data = (uint8_t *)Data;
    int val1 = data[0];
    int val2 = data[1];
    int val3 = data[2];
    int val4 = data[3];
    int val5 = data[4];

    ret = vpx_codec_control(&ctx, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, val1);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    ret = vpx_codec_control(&ctx, VP8E_SET_CPUUSED, val2);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    ret = vpx_codec_control(&ctx, VP9E_SET_AQ_MODE, val3);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    ret = vpx_codec_control(&ctx, VP9E_SET_GF_CBR_BOOST_PCT, val4);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    ret = vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, val5);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    fuzz_vp9_encoder_settings(Data, Size);
    return 0;
}