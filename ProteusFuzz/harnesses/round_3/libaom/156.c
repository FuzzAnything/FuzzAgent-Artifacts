// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, uint8_t *data, size_t size) {
    int ret;
    int value;

    if (ctx == NULL) return 0;

    // Determine value from input data or use defaults
    if (size >= sizeof(int)) {
        memcpy(&value, data, sizeof(int));
    } else {
        value = 0;
    }

    // Map control_id to actual AOM_CODEC_CTRL_ID
    // Using the correct enum values found in aomcx.h
    switch (control_id) {
        case 0: // AV1E_SET_MIN_GF_INTERVAL
            ret = aom_codec_control(ctx, AV1E_SET_MIN_GF_INTERVAL, value);
            break;
        case 1: // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, value);
            break;
        case 2: // AV1E_SET_FP_MT_UNIT_TEST
            ret = aom_codec_control(ctx, AV1E_SET_FP_MT_UNIT_TEST, value);
            break;
        case 3: // AV1E_SET_SVC_FRAME_DROP_MODE
            ret = aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, value);
            break;
        case 4: // AV1E_SET_FILM_GRAIN_TEST_VECTOR
            // This control might expect a pointer or specific struct, but for fuzzing
            // we try passing an int first. If it expects a pointer, the cast might trigger
            // a crash or error, which is valid for fuzzing.
            ret = aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, value);
            break;
        case 5: // AV1E_SET_MAX_PARTITION_SIZE
            ret = aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, value);
            break;
        default:
            return 0;
    }
    (void)ret; // Suppress unused variable warning, fuzzing cares about execution path
    return 1;
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int control_id = 0;
    uint8_t *payload = NULL;
    size_t payload_size = 0;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    
    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize default config
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to configure encoder parameters
    if (Size > 0) cfg.g_w = (Data[0] % 512) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 512) + 16;
    // Fixed: Changed g_time_base to g_timebase
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 100) + 1;
    if (Size > 4) cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    
    // Initialize encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare payload for control functions
    if (Size > 1) {
        control_id = Data[0] % 6; // 0 to 5 for the 6 target functions
        payload_size = Size - 1;
        payload = (uint8_t *)Data + 1;
    }

    // Invoke the fuzzed control function
    fuzz_encoder_control(&ctx, control_id, payload, payload_size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}