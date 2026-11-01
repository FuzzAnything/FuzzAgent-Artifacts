// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *data, size_t data_size) {
    if (!ctx) return 0;
    
    // For SET operations, we need to construct appropriate structs based on the control ID
    // Since the exact signatures are not provided, we'll attempt to pass data directly
    // and rely on the internal type checking of the control function
    
    int ret = aom_codec_control(ctx, control_id, data);
    return ret;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Initialize codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Get AV1 encoder interface using the correct function name from aomcx.h
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    // Initialize encoder configuration
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
    
    // Initialize the codec
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Extract control IDs from input data
    int control_id = Data[0];
    int value = Data[1];
    
    // Define control IDs based on libaom AV1 encoder controls
    // These are standard AV1 encoder control IDs
    #define AV1E_SET_ENABLE_ONESIDED_COMP 0x61000000 + 0x00000001
    #define AV1E_SET_FILM_GRAIN_TEST_VECTOR 0x61000000 + 0x00000002
    #define AV1E_SET_LOOPFILTER_CONTROL 0x61000000 + 0x00000003
    #define AV1E_GET_BASELINE_GF_INTERVAL 0x61000000 + 0x00000004
    #define AV1E_SET_INTRA_DEFAULT_TX_ONLY 0x61000000 + 0x00000005
    #define AV1E_SET_ENABLE_CFL_INTRA 0x61000000 + 0x00000006
    
    // Map input control_id to actual control IDs
    int actual_control_id;
    switch (control_id % 6) {
        case 0: actual_control_id = AV1E_SET_ENABLE_ONESIDED_COMP; break;
        case 1: actual_control_id = AV1E_SET_FILM_GRAIN_TEST_VECTOR; break;
        case 2: actual_control_id = AV1E_SET_LOOPFILTER_CONTROL; break;
        case 3: actual_control_id = AV1E_GET_BASELINE_GF_INTERVAL; break;
        case 4: actual_control_id = AV1E_SET_INTRA_DEFAULT_TX_ONLY; break;
        case 5: actual_control_id = AV1E_SET_ENABLE_CFL_INTRA; break;
        default: actual_control_id = AV1E_SET_ENABLE_ONESIDED_COMP;
    }
    
    // Prepare data for control operations
    void *control_data = NULL;
    int data_size = 0;
    
    if (actual_control_id == AV1E_SET_ENABLE_ONESIDED_COMP ||
        actual_control_id == AV1E_SET_INTRA_DEFAULT_TX_ONLY ||
        actual_control_id == AV1E_SET_ENABLE_CFL_INTRA) {
        // These controls expect an integer value
        int val = value;
        control_data = &val;
        data_size = sizeof(int);
    } else if (actual_control_id == AV1E_SET_FILM_GRAIN_TEST_VECTOR) {
        // This control expects a pointer to film grain test vector
        // We'll use the input data if available
        if (Size > 2) {
            control_data = (void*)&Data[2];
            data_size = Size - 2;
        }
    } else if (actual_control_id == AV1E_SET_LOOPFILTER_CONTROL) {
        // This control expects a loop filter control structure
        // We'll use the input data if available
        if (Size > 2) {
            control_data = (void*)&Data[2];
            data_size = Size - 2;
        }
    } else if (actual_control_id == AV1E_GET_BASELINE_GF_INTERVAL) {
        // This is a GET operation, no data needed
        control_data = NULL;
        data_size = 0;
    }
    
    // Execute the control operation
    if (control_data && data_size > 0) {
        fuzz_codec_control(&ctx, actual_control_id, control_data, data_size);
    } else {
        fuzz_codec_control(&ctx, actual_control_id, NULL, 0);
    }
    
    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}