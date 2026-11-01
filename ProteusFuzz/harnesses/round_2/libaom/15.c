// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdio.h>

#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int test_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    
    // Attempt to set the control with the fuzzed value
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    
    // Return success if the control was accepted (AOM_CODEC_OK)
    // We don't fail the fuzzer on AOM_CODEC_ERROR as invalid controls are expected
    return (ret == AOM_CODEC_OK) ? 1 : 0;
}

static int test_codec_control_bool(aom_codec_ctx_t *ctx, int control_id) {
    if (ctx == NULL) return 0;
    
    // Test with 0 and 1 for boolean-like controls
    int ret0 = aom_codec_control(ctx, control_id, 0);
    int ret1 = aom_codec_control(ctx, control_id, 1);
    
    return (ret0 == AOM_CODEC_OK || ret1 == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices
    if (Size < 6) return 0;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set basic config from fuzz data
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 1000) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 1000;
    cfg.g_error_resilient = 0;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions based on input data
    int control_id = 0;
    int value = 0;
    int state_changes = 0;

    // Map input bytes to control IDs and values
    // Using modulo to select between the known control types
    int selector = Data[4] % 7; // 0-6 for 6 functions + default
    
    switch (selector) {
        case 0: // AV1E_SET_ENABLE_INTERINTRA_COMP
            control_id = AV1E_SET_ENABLE_INTERINTRA_COMP;
            value = (Data[5] % 2);
            if (test_codec_control_bool(&ctx, control_id)) state_changes++;
            break;
        case 1: // AV1E_SET_ENABLE_RECT_PARTITIONS
            control_id = AV1E_SET_ENABLE_RECT_PARTITIONS;
            value = (Data[5] % 2);
            if (test_codec_control_bool(&ctx, control_id)) state_changes++;
            break;
        case 2: // AV1E_SET_LOOPFILTER_CONTROL
            // This control might take more complex arguments, but we try simple int
            control_id = AV1E_SET_LOOPFILTER_CONTROL;
            value = (Data[5] % 100);
            if (test_codec_control(&ctx, control_id, value)) state_changes++;
            break;
        case 3: // AV1E_SET_ENABLE_PALETTE
            control_id = AV1E_SET_ENABLE_PALETTE;
            value = (Data[5] % 2);
            if (test_codec_control_bool(&ctx, control_id)) state_changes++;
            break;
        case 4: // AV1E_SET_INTRA_DCT_ONLY
            control_id = AV1E_SET_INTRA_DCT_ONLY;
            value = (Data[5] % 2);
            if (test_codec_control_bool(&ctx, control_id)) state_changes++;
            break;
        case 5: // AV1E_SET_ENABLE_WARPED_MOTION
            control_id = AV1E_SET_ENABLE_WARPED_MOTION;
            value = (Data[5] % 2);
            if (test_codec_control_bool(&ctx, control_id)) state_changes++;
            break;
        case 6: // Random combination
            // Try multiple controls in sequence
            int controls[] = {
                AV1E_SET_ENABLE_INTERINTRA_COMP,
                AV1E_SET_ENABLE_RECT_PARTITIONS,
                AV1E_SET_ENABLE_PALETTE,
                AV1E_SET_INTRA_DCT_ONLY,
                AV1E_SET_ENABLE_WARPED_MOTION
            };
            // Only access available data
            int max_idx = (Size > 5) ? (Size - 5) : 0;
            int loop_limit = (max_idx < 5) ? max_idx : 5;
            for (int i = 0; i < loop_limit; i++) {
                int val = (Data[5 + i] % 2);
                if (aom_codec_control(&ctx, controls[i], val) == AOM_CODEC_OK) {
                    state_changes++;
                }
            }
            break;
        default:
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return state_changes > 0 ? 1 : 0;
}