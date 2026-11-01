// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzzer_test_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // Try to set the control with the provided value
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res; // Ignore return value for fuzzing, just want to trigger code paths
    
    return 0;
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16; // Minimum 16
    cfg.g_h = (Data[1] % 256) + 16; // Minimum 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Extract values from input data for control parameters
    int value1 = (Size > 2) ? (Data[2] % 100) + 1 : 1;
    int value2 = (Size > 3) ? (Data[3] % 2) : 0;
    int value3 = (Size > 4) ? (Data[4] % 4) + 1 : 1;
    int value4 = (Size > 5) ? (Data[5] % 4) + 1 : 1;
    int value5 = (Size > 6) ? (Data[6] % 100) + 1 : 1;
    int value6 = (Size > 7) ? (Data[7] % 256) + 16 : 16;
    
    // Test AV1E_SET_MAX_GF_INTERVAL
    fuzzer_test_encoder_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, value1);
    
    // Test AV1E_SET_ERROR_RESILIENT_MODE
    fuzzer_test_encoder_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, value2);
    
    // Test AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    fuzzer_test_encoder_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, value3);
    
    // Test AV1E_SET_GF_MAX_PYRAMID_HEIGHT
    fuzzer_test_encoder_control(&ctx, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, value4);
    
    // Test AV1E_SET_MIN_GF_INTERVAL
    fuzzer_test_encoder_control(&ctx, AV1E_SET_MIN_GF_INTERVAL, value5);
    
    // Test AV1E_SET_MAX_PARTITION_SIZE
    fuzzer_test_encoder_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, value6);
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}