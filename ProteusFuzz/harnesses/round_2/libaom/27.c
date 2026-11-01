// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t err;
    
    // Cast value to appropriate type based on control
    int32_t int_val = (int32_t)value;
    
    err = aom_codec_control(ctx, control_id, &int_val);
    return (err == AOM_CODEC_OK) ? 1 : 0;
}

static int fuzz_encoder_control_int(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t err;
    
    err = aom_codec_control(ctx, control_id, &value);
    return (err == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Need at least 8 bytes for control_id (4 bytes) and value (4 bytes)
    if (Size < 8) return 0;
    
    // Extract control ID and value from input data safely
    int32_t control_id_raw = 0;
    int32_t value_raw = 0;
    
    memcpy(&control_id_raw, Data, sizeof(int32_t));
    memcpy(&value_raw, Data + 4, sizeof(int32_t));
    
    int control_id = (int)control_id_raw;
    int32_t value = value_raw;
    
    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set some basic configuration
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Map control IDs to actual libaom control constants
    // Using a switch to handle different control types
    int result = 0;
    switch (control_id) {
        case AOME_SET_ENABLEAUTOALTREF:
            result = fuzz_encoder_control(&ctx, AOME_SET_ENABLEAUTOALTREF, value);
            break;
        case AV1E_SET_MAX_REFERENCE_FRAMES:
            result = fuzz_encoder_control_int(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, value);
            break;
        case AV1E_SET_NOISE_SENSITIVITY:
            result = fuzz_encoder_control_int(&ctx, AV1E_SET_NOISE_SENSITIVITY, value);
            break;
        case AOME_SET_ENABLEAUTOBWDREF:
            result = fuzz_encoder_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, value);
            break;
        case AV1E_SET_ENABLE_QM:
            result = fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_QM, value);
            break;
        case AV1E_SET_QM_MAX:
            result = fuzz_encoder_control_int(&ctx, AV1E_SET_QM_MAX, value);
            break;
        default:
            // Try with the raw control ID if it's not a known constant
            // This allows fuzzing to explore unknown control IDs
            result = fuzz_encoder_control(&ctx, control_id, value);
            break;
    }
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}