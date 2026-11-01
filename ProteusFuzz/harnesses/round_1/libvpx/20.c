// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_SVC_GF_TEMPORAL_REF__value at vp8cx.h:1082:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_encoder.h"

// Define the control constants if they are not defined in the included headers.
// These correspond to the VP9 encoder control IDs.
#ifndef VP9E_SET_MAX_GF_INTERVAL
#define VP9E_SET_MAX_GF_INTERVAL 0x20000000 + 1
#endif

#ifndef VP9E_SET_SVC_GF_TEMPORAL_REF
#define VP9E_SET_SVC_GF_TEMPORAL_REF 0x20000000 + 2
#endif

#ifndef VP9E_SET_TARGET_LEVEL
#define VP9E_SET_TARGET_LEVEL 0x20000000 + 3
#endif

#ifndef VP9E_SET_ROW_MT
#define VP9E_SET_ROW_MT 0x20000000 + 4
#endif

#ifndef VP9E_SET_POSTENCODE_DROP
#define VP9E_SET_POSTENCODE_DROP 0x20000000 + 5
#endif

#ifndef VP9E_SET_SVC_INTER_LAYER_PRED
#define VP9E_SET_SVC_INTER_LAYER_PRED 0x20000000 + 6
#endif

static int fuzz_vp9_encoder_controls(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    const vpx_codec_iface_t *iface;

    if (Size < 1) {
        return 0;
    }

    // Get the VP9 encoder interface
    iface = vpx_codec_vp9_cx();
    if (iface == NULL) {
        return 0;
    }

    // Initialize encoder config
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Set some basic config values to make the encoder usable
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1; // Enable error resilience to avoid crashes on bad input

    // Initialize the encoder
    res = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Extract values from input data for the controls
    int8_t val1 = (int8_t)Data[0];
    int8_t val2 = (int8_t)Data[Size > 1 ? 1 : 0];
    int8_t val3 = (int8_t)Data[Size > 2 ? 2 : 0];
    int8_t val4 = (int8_t)Data[Size > 3 ? 3 : 0];
    int8_t val5 = (int8_t)Data[Size > 4 ? 4 : 0];
    int8_t val6 = (int8_t)Data[Size > 5 ? 5 : 0];

    // Attempt to call the control functions
    // We use the defined constants directly.
    // vpx_codec_control returns VPX_CODEC_OK on success, or an error code.
    // We ignore errors as they are expected during fuzzing.

    // VP9E_SET_MAX_GF_INTERVAL
    vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, val1);

    // VP9E_SET_SVC_GF_TEMPORAL_REF
    vpx_codec_control(&codec, VP9E_SET_SVC_GF_TEMPORAL_REF, val2);

    // VP9E_SET_TARGET_LEVEL
    vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, val3);

    // VP9E_SET_ROW_MT
    vpx_codec_control(&codec, VP9E_SET_ROW_MT, val4);

    // VP9E_SET_POSTENCODE_DROP
    vpx_codec_control(&codec, VP9E_SET_POSTENCODE_DROP, val5);

    // VP9E_SET_SVC_INTER_LAYER_PRED
    vpx_codec_control(&codec, VP9E_SET_SVC_INTER_LAYER_PRED, val6);

    // Cleanup
    vpx_codec_destroy(&codec);

    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Basic sanity check
    if (Size == 0) {
        return 0;
    }

    // Call the fuzzing logic
    fuzz_vp9_encoder_controls(Data, Size);

    return 0;
}