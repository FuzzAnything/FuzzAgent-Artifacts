// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int get_int(const uint8_t **d, size_t *s, int default_val) {
    if (*s < 4) return default_val;
    int val = ((int)(*d)[0] << 24) | ((int)(*d)[1] << 16) | ((int)(*d)[2] << 8) | (int)(*d)[3];
    *d += 4;
    *s -= 4;
    return val;
}

static int get_int8(const uint8_t **d, size_t *s, int default_val) {
    if (*s < 1) return default_val;
    int val = (int)*(*d);
    (*d)++;
    (*s)--;
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    const uint8_t *ptr = data;
    size_t remaining = size;

    // 1. AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    if (remaining >= 1) {
        int mode = get_int8(&ptr, &remaining, 0);
        aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, mode);
    }

    // 2. AV1E_SET_QM_V (Quantization Matrix Vertical)
    if (remaining >= 4) {
        int qm_val = get_int(&ptr, &remaining, 0);
        aom_codec_control(ctx, AV1E_SET_QM_V, qm_val);
    }

    // 3. AV1E_SET_ENABLE_SMOOTH_INTRA
    if (remaining >= 1) {
        int enable = get_int8(&ptr, &remaining, 0);
        aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, enable);
    }

    // 4. AV1E_SET_INTER_DCT_ONLY
    if (remaining >= 1) {
        int dct_only = get_int8(&ptr, &remaining, 0);
        aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, dct_only);
    }

    // 5. AV1E_SET_FILM_GRAIN_TEST_VECTOR
    if (remaining >= 4) {
        int test_vec = get_int(&ptr, &remaining, 0);
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, test_vec);
    }

    // 6. AV1E_SET_ENABLE_ANGLE_DELTA
    if (remaining >= 1) {
        int angle_delta = get_int8(&ptr, &remaining, 0);
        aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, angle_delta);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration to ensure valid initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 0;

    // Get the AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize the encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}