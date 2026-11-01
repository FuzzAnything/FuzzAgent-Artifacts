// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

/* Helper functions to read data from fuzz input */
static uint32_t read_u32(const uint8_t **d, size_t *s) {
    if (*s < 4) return 0;
    uint32_t val = (*d)[0] | ((*d)[1] << 8) | ((*d)[2] << 16) | ((*d)[3] << 24);
    *d += 4;
    *s -= 4;
    return val;
}

static uint8_t read_u8(const uint8_t **d, size_t *s) {
    if (*s < 1) return 0;
    uint8_t val = (*d)[0];
    *d += 1;
    *s -= 1;
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec || !data || size == 0) return 0;

    const uint8_t *ptr = data;
    size_t remaining = size;

    // 1. AV1E_SET_ENABLE_SMOOTH_INTRA (int32_t)
    int32_t smooth_intra = read_u8(&ptr, &remaining);
    if (aom_codec_control(codec, AV1E_SET_ENABLE_SMOOTH_INTRA, smooth_intra) != AOM_CODEC_OK) {
        // Ignore errors for fuzzing
    }

    // 2. AV1E_SET_MAX_PARTITION_SIZE (int32_t)
    int32_t max_part_size = read_u32(&ptr, &remaining);
    if (aom_codec_control(codec, AV1E_SET_MAX_PARTITION_SIZE, max_part_size) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 3. AV1E_SET_POSTENCODE_DROP_RTC (int32_t)
    // Only attempt if the macro is defined
    #ifdef AV1E_SET_POSTENCODE_DROP_RTC
    int32_t drop_rtc = read_u8(&ptr, &remaining);
    if (aom_codec_control(codec, AV1E_SET_POSTENCODE_DROP_RTC, drop_rtc) != AOM_CODEC_OK) {
        // Ignore errors
    }
    #endif

    // 4. AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Skipping complex struct construction for safety in this context.

    // 5. AV1E_SET_FP_MT_UNIT_TEST (int32_t)
    #ifdef AV1E_SET_FP_MT_UNIT_TEST
    int32_t fp_mt_test = read_u8(&ptr, &remaining);
    if (aom_codec_control(codec, AV1E_SET_FP_MT_UNIT_TEST, fp_mt_test) != AOM_CODEC_OK) {
        // Ignore errors
    }
    #endif

    // 6. AV1E_SET_INTRA_DEFAULT_TX_ONLY (int32_t)
    int32_t intra_tx_only = read_u8(&ptr, &remaining);
    if (aom_codec_control(codec, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_tx_only) != AOM_CODEC_OK) {
        // Ignore errors
    }

    return 0;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    memset(&codec, 0, sizeof(codec));

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    res = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_controls(&codec, Data, Size);

    aom_codec_destroy(&codec);

    return 0;
}