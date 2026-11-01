// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vp8dx.h"
#include "vp8cx.h"
#include "vpx_codec.h"

static void cleanup_encoder(vpx_codec_ctx_t *codec) {
    if (codec) {
        vpx_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize encoder context
    vpx_codec_ctx_t *codec = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!codec) return 0;
    memset(codec, 0, sizeof(vpx_codec_ctx_t));

    // Initialize the codec with VP9 encoder
    // vpx_codec_vp9_cx() returns a pointer, so we pass it directly, not its address
    if (vpx_codec_enc_init(codec, vpx_codec_vp9_cx(), NULL, 0) != VPX_CODEC_OK) {
        cleanup_encoder(codec);
        return 0;
    }

    // Extract values from fuzz input to drive diverse states
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    int val3 = (Size > 2) ? (int)Data[2] : 0;
    int val4 = (Size > 3) ? (int)Data[3] : 0;
    int val5 = (Size > 4) ? (int)Data[4] : 0;
    int val6 = (Size > 5) ? (int)Data[5] : 0;

    // Fuzz VP9E_SET_MAX_GF_INTERVAL
    vpx_codec_control(codec, VP9E_SET_MAX_GF_INTERVAL, val1);

    // Fuzz VP9E_SET_POSTENCODE_DROP
    vpx_codec_control(codec, VP9E_SET_POSTENCODE_DROP, val2);

    // Fuzz VP9E_SET_ALT_REF_AQ
    vpx_codec_control(codec, VP9E_SET_ALT_REF_AQ, val3);

    // Fuzz VP9E_SET_TARGET_LEVEL
    vpx_codec_control(codec, VP9E_SET_TARGET_LEVEL, val4);

    // Fuzz VP9E_SET_COLOR_RANGE
    vpx_codec_control(codec, VP9E_SET_COLOR_RANGE, val5);

    // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
    vpx_codec_control(codec, VP9E_SET_QUANTIZER_ONE_PASS, val6);

    // Cleanup
    cleanup_encoder(codec);

    return 0;
}