// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec) return 0;

    int val;
    int idx = 0;

    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    if (idx < size) {
        val = (int)data[idx++];
        aom_codec_control(codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
    }

    // AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST
    if (idx < size) {
        val = (int)data[idx++];
        aom_codec_control(codec, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, val);
    }

    // AV1E_SET_ENABLE_PALETTE
    if (idx < size) {
        val = (int)data[idx++];
        aom_codec_control(codec, AV1E_SET_ENABLE_PALETTE, val);
    }

    // AV1E_SET_SVC_FRAME_DROP_MODE
    if (idx < size) {
        val = (int)data[idx++];
        aom_codec_control(codec, AV1E_SET_SVC_FRAME_DROP_MODE, val);
    }

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR
    if (idx < size) {
        val = (int)data[idx++];
        aom_codec_control(codec, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val);
    }

    // AV1E_SET_FP_MT_UNIT_TEST
    if (idx < size) {
        val = (int)data[idx++];
        aom_codec_control(codec, AV1E_SET_FP_MT_UNIT_TEST, val);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int initialized = 0;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config values from input to vary state
    cfg.g_w = (Data[0] % 4096) + 16;
    cfg.g_h = (Data[1] % 4096) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[0] + Data[1]) * 1000;

    // Initialize encoder context
    ret = aom_codec_enc_init(&codec, aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }
    initialized = 1;

    // Fuzz the control functions
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    if (initialized) {
        aom_codec_destroy(&codec);
    }

    return 0;
}