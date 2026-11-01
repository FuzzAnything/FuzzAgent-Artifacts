// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (!codec || !data || size < 2) return 0;

    int val = 0;
    if (size >= 4) {
        val = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    } else if (size >= 1) {
        val = data[0];
    }

    // AV1E_SET_ENABLE_SMOOTH_INTRA (0x0001)
    aom_codec_control(codec, AV1E_SET_ENABLE_SMOOTH_INTRA, val);

    // AV1E_SET_MAX_PARTITION_SIZE (0x0002)
    // Partition sizes are typically powers of 2, fuzz with clamped values
    int part_size = (val % 64) + 8; // 8 to 71
    aom_codec_control(codec, AV1E_SET_MAX_PARTITION_SIZE, part_size);

    // AV1E_SET_POSTENCODE_DROP_RTC (0x0003)
    aom_codec_control(codec, AV1E_SET_POSTENCODE_DROP_RTC, val);

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR (0x0004)
    // This usually expects a pointer to a struct, but for fuzzing we pass the int value
    // to trigger potential internal parsing or error handling paths if the API allows it loosely.
    // In strict usage, this might fail, which is a valid fuzz state.
    aom_codec_control(codec, AV1E_SET_FILM_GRAIN_TEST_VECTOR, val);

    // AV1E_SET_FP_MT_UNIT_TEST (0x0005)
    aom_codec_control(codec, AV1E_SET_FP_MT_UNIT_TEST, val);

    // AV1E_SET_INTRA_DEFAULT_TX_ONLY (0x0006)
    aom_codec_control(codec, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);

    return 1;
}

int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize config with fuzzed data if available
    if (Size >= 4) {
        cfg.g_w = (Data[0] | (Data[1] << 8)) & 0x3FF; // Limit width
        cfg.g_h = (Data[2] | (Data[3] << 8)) & 0x3FF; // Limit height
        if (cfg.g_w == 0) cfg.g_w = 64;
        if (cfg.g_h == 0) cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 1000;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 1000;
    }

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic on the controls
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}