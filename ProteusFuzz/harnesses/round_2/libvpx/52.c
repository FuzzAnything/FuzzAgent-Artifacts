// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_DELTA_Q_UV__value at vp8cx.h:1091:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t codec;
static vpx_codec_enc_cfg_t cfg;

static int init_codec(void) {
    if (vpx_codec_enc_init(&codec, vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_codec(void) {
    vpx_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Reset codec state for each input
    cleanup_codec();
    memset(&cfg, 0, sizeof(cfg));
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0);

    if (!init_codec()) {
        return 0;
    }

    // Use input data to drive control parameters
    int val = Data[0];

    // 1. VP9E_SET_TILE_ROWS__value
    // Typically sets number of tile rows (1-16)
    if (vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, val) != VPX_CODEC_OK) {
        // Ignore errors, continue to other controls
    }

    // 2. VP9E_SET_DELTA_Q_UV__value
    // Typically sets delta Q for UV planes
    if (vpx_codec_control(&codec, VP9E_SET_DELTA_Q_UV, val) != VPX_CODEC_OK) {
        // Ignore errors
    }

    // 3. VP9E_SET_COLOR_RANGE__value
    // Typically sets color range (0 for limited, 1 for full)
    if (vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, val) != VPX_CODEC_OK) {
        // Ignore errors
    }

    // 4. VP9E_SET_POSTENCODE_DROP__value
    // Typically sets post-encode drop flag
    if (vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, val) != VPX_CODEC_OK) {
        // Ignore errors
    }

    // 5. VP9E_SET_QUANTIZER_ONE_PASS__value
    // Typically sets quantizer for one-pass mode
    if (vpx_codec_control(&codec, VP9E_SET_QUANTIZER_ONE_PASS, val) != VPX_CODEC_OK) {
        // Ignore errors
    }

    // 6. VP9E_SET_TARGET_LEVEL__value
    // Typically sets target level (e.g., 0-100)
    if (vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, val) != VPX_CODEC_OK) {
        // Ignore errors
    }

    // Cleanup after each test
    cleanup_codec();

    return 0;
}