// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t *codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) {
        return 0;
    }
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(codec);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(codec);
        return 0;
    }

    cfg.g_w = Data[0] ? Data[0] : 16;
    cfg.g_h = Data[1] ? Data[1] : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(codec);
        return 0;
    }

    // Fuzz AV1E_SET_FP_MT_UNIT_TEST
    if (Size > 2) {
        int val1 = Data[2];
        aom_codec_control(codec, AV1E_SET_FP_MT_UNIT_TEST, val1);
    }

    // Fuzz AV1E_ENABLE_SB_QP_SWEEP
    if (Size > 3) {
        int val2 = Data[3];
        aom_codec_control(codec, AV1E_ENABLE_SB_QP_SWEEP, val2);
    }

    // Fuzz AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    if (Size > 4) {
        int val3 = Data[4];
        aom_codec_control(codec, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val3);
    }

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    if (Size > 5) {
        int val4 = Data[5];
        aom_codec_control(codec, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val4);
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (Size > 6) {
        int val5 = Data[6];
        aom_codec_control(codec, AV1E_SET_ENABLE_PALETTE, val5);
    }

    // Fuzz AV1E_SET_SVC_FRAME_DROP_MODE
    if (Size > 7) {
        int val6 = Data[7];
        aom_codec_control(codec, AV1E_SET_SVC_FRAME_DROP_MODE, val6);
    }

    // Cleanup
    cleanup_codec(codec);
    return 0;
}