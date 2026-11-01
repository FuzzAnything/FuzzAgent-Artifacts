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
    }
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    uint8_t val = Data[0];
    int32_t val32 = (int32_t)(Data[0] | (Data[1] << 8));

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, val) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // This typically expects a pointer to a film grain test vector structure.
    // We pass a pointer to the input data if size is sufficient, or a dummy.
    const void *fg_ptr = (Size >= 16) ? (const void *)Data : NULL;
    if (aom_codec_control(&codec, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (intptr_t)fg_ptr) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_ENABLE_DIST_WTD_COMP
    if (aom_codec_control(&codec, AV1E_SET_ENABLE_DIST_WTD_COMP, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_GF_CBR_BOOST_PCT
    if (aom_codec_control(&codec, AV1E_SET_GF_CBR_BOOST_PCT, val32) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_MIN_GF_INTERVAL
    if (aom_codec_control(&codec, AV1E_SET_MIN_GF_INTERVAL, val32) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    if (aom_codec_control(&codec, AV1E_SET_ENABLE_CFL_INTRA, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    cleanup_codec(&codec);
    return 0;
}