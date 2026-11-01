// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t codec;
static vpx_codec_enc_cfg_t cfg;

static void cleanup_codec(void) {
    vpx_codec_destroy(&codec);
}

static int init_codec(const uint8_t *data, size_t size) {
    int ret;
    vpx_codec_iface_t *iface;

    cleanup_codec();

    iface = vpx_codec_vp9_cx();
    if (!iface) return -1;

    ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return -1;

    ret = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    int ret;
    int val;

    if (Size < 4) return 0;

    val = *(int32_t *)Data;

    if (init_codec(Data, Size) != 0) return 0;

    // VP9E_SET_TILE_ROWS
    ret = vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, val);
    (void)ret;

    // VP9E_SET_COLOR_RANGE
    ret = vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, val);
    (void)ret;

    // VP9E_SET_MAX_GF_INTERVAL
    ret = vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, val);
    (void)ret;

    // VP9E_SET_QUANTIZER_ONE_PASS
    ret = vpx_codec_control(&codec, VP9E_SET_QUANTIZER_ONE_PASS, val);
    (void)ret;

    // VP9E_SET_TARGET_LEVEL
    ret = vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, val);
    (void)ret;

    // VP9E_SET_POSTENCODE_DROP
    ret = vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, val);
    (void)ret;

    cleanup_codec();

    return 0;
}