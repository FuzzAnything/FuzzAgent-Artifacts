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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // Try different value ranges for the control
    int test_values[] = {0, 1, -1, 100, 200, 1000, 0x7FFFFFFF, 0x80000000};
    int num_values = sizeof(test_values) / sizeof(test_values[0]);
    
    // Use the provided value if it's within a reasonable range, otherwise use test values
    int val_to_use = value;
    if (value < -1000 || value > 10000) {
        val_to_use = test_values[value % num_values];
    }
    
    aom_codec_err_t result = aom_codec_control(ctx, control_id, val_to_use);
    
    // Also try with the original value if different
    if (val_to_use != value) {
        aom_codec_control(ctx, control_id, value);
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput_202(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Initialize codec context
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;
    
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    // Get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return 0;
    }
    
    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }
    
    // Set basic configuration from input data
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    
    // Use rc_target_bitrate directly instead of g_bitrate
    int bitrate = (Data[4 % Size] % 10000) + 100;
    cfg.rc_target_bitrate = bitrate;
    
    // Initialize the codec
    aom_codec_err_t init_result = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (init_result != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }
    
    // Extract control IDs from input data (using correct identifiers from aomcx.h)
    int control_ids[] = {
        AV1E_SET_ENABLE_ANGLE_DELTA,
        AOME_SET_MAX_INTRA_BITRATE_PCT,
        AV1E_SET_ENABLE_DIST_WTD_COMP,
        AV1E_SET_GF_CBR_BOOST_PCT,
        AV1E_SET_QUANT_B_ADAPT,
        AV1E_SET_DELTALF_MODE
    };
    
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);
    
    // Fuzz each control with different values
    for (size_t i = 0; i < Size && i < num_controls; i++) {
        int control_id = control_ids[i % num_controls];
        int value = (int)(Data[i] % 200) - 100; // Range: -100 to 99
        
        fuzz_codec_control(ctx, control_id, value);
    }
    
    // Try with larger values
    if (Size > 10) {
        int large_value = ((Data[Size-1] << 8) | Data[Size-2]) % 10000;
        for (int i = 0; i < num_controls; i++) {
            fuzz_codec_control(ctx, control_ids[i], large_value);
        }
    }
    
    // Cleanup
    cleanup_codec(ctx);
    
    return 0;
}