// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    int ret = 0;
    int value;
    
    // Helper to safely read an int from fuzz data
    if (size >= sizeof(int)) {
        memcpy(&value, data, sizeof(int));
        // Normalize to 0 or 1 for boolean flags
        value = value ? 1 : 0;
    } else {
        value = 0;
    }

    // 1. AV1E_SET_ENABLE_ONESIDED_COMP
    if (aom_codec_control(codec, AV1E_SET_ENABLE_ONESIDED_COMP, value) != AOM_CODEC_OK) {
        ret = 1; // Error occurred, but we continue fuzzing
    }

    // 2. AV1E_SET_ENABLE_MASKED_COMP
    if (size >= sizeof(int) + sizeof(int)) {
        memcpy(&value, data + sizeof(int), sizeof(int));
        value = value ? 1 : 0;
    }
    if (aom_codec_control(codec, AV1E_SET_ENABLE_MASKED_COMP, value) != AOM_CODEC_OK) {
        ret = 1;
    }

    // 3. AV1E_SET_ENABLE_SMOOTH_INTRA
    if (size >= sizeof(int) * 2) {
        memcpy(&value, data + sizeof(int) * 2, sizeof(int));
        value = value ? 1 : 0;
    }
    if (aom_codec_control(codec, AV1E_SET_ENABLE_SMOOTH_INTRA, value) != AOM_CODEC_OK) {
        ret = 1;
    }

    // 4. AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // This control expects a pointer to aom_film_grain_params_t or similar.
    // Since the type is not directly available or easily constructible from raw bytes
    // without specific header details in the environment, we pass NULL to test error handling.
    if (aom_codec_control(codec, AV1E_SET_FILM_GRAIN_TEST_VECTOR, NULL) != AOM_CODEC_OK) {
        ret = 1;
    }

    // 5. AV1E_SET_ENABLE_DUAL_FILTER
    if (size >= sizeof(int) * 3) {
        memcpy(&value, data + sizeof(int) * 3, sizeof(int));
        value = value ? 1 : 0;
    }
    if (aom_codec_control(codec, AV1E_SET_ENABLE_DUAL_FILTER, value) != AOM_CODEC_OK) {
        ret = 1;
    }

    // 6. AV1E_SET_ENABLE_ANGLE_DELTA
    if (size >= sizeof(int) * 4) {
        memcpy(&value, data + sizeof(int) * 4, sizeof(int));
        value = value ? 1 : 0;
    }
    if (aom_codec_control(codec, AV1E_SET_ENABLE_ANGLE_DELTA, value) != AOM_CODEC_OK) {
        ret = 1;
    }

    return ret;
}

int LLVMFuzzerTestOneInput_172(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    int ret = 0;

    // Initialize the codec context
    memset(&codec, 0, sizeof(codec));

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration with defaults
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration to ensure valid state
    cfg.g_w = 64;
    cfg.g_h = 64;
    // Fixed typo: g_time_base -> g_timebase
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;

    // Initialize the encoder
    err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the target control functions
    if (Size > 0) {
        fuzz_encoder_controls(&codec, Data, Size);
    }

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}