// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    if (codec == NULL) return 0;

    // Ensure we have enough data to extract parameters
    if (size < 4) return 0;

    int32_t value = *(const int32_t *)data;
    // Clamp or normalize value if necessary for safety, though the API expects specific flags
    // For boolean-like flags, we can try 0 and 1, or the raw value if it's a threshold
    int32_t test_values[] = {0, 1, value, (int32_t)size, -1, 1000};
    int num_test_values = 6;

    // List of control commands to test
    // Note: These macros are typically defined in aom/aomcx.h or aom_encoder.h
    // We cast them to int to pass to aom_codec_control
    int controls[] = {
        AV1E_SET_ENABLE_INTERINTER_WEDGE,
        AV1E_SET_ENABLE_AB_PARTITIONS,
        AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR,
        AV1E_SET_ENABLE_MASKED_COMP,
        AV1E_SET_INTER_DCT_ONLY,
        AV1E_SET_ENABLE_RECT_PARTITIONS
    };
    int num_controls = sizeof(controls) / sizeof(controls[0]);

    for (int i = 0; i < num_controls; i++) {
        for (int j = 0; j < num_test_values; j++) {
            aom_codec_err_t err = aom_codec_control(codec, controls[i], test_values[j]);
            // We don't necessarily fail on error, as we are fuzzing for crashes/hangs
            // However, if initialization failed, further calls might be unsafe
            if (err != AOM_CODEC_OK && err != AOM_CODEC_ERROR) {
                // Unexpected error state, but continue to see if it recovers or crashes
            }
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    // Initialize encoder configuration
    err = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some basic configuration to ensure a valid context can be created
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize the encoder
    err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control commands
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}