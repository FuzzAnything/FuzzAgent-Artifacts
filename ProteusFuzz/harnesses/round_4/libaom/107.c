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
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

/*
 * Helper function to initialize an encoder context.
 * Returns a valid aom_codec_ctx_t pointer on success, NULL on failure.
 */
static aom_codec_ctx_t *init_encoder_context(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Set some basic configuration to ensure valid initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/*
 * Helper function to cleanup encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize encoder context
    aom_codec_ctx_t *ctx = init_encoder_context();
    if (!ctx) return 0;

    // Interpret input data for fuzzing parameters
    int32_t param_val;
    uint32_t cmd;
    
    // Extract command index from first byte
    cmd = Data[0] % 6; // 0 to 5 for the 6 target functions

    // Extract parameter value from next 4 bytes if available, else use random
    if (Size >= 5) {
        param_val = *(int32_t *)(Data + 1);
    } else {
        param_val = (int32_t)(Data[0] << 8) | (Size << 4);
    }

    aom_codec_err_t err;

    switch (cmd) {
        case 0: // aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE
            // Min partition size: typically 4, 8, 16, 32, 64
            // Clamp to valid range for testing
            if (param_val > 0 && param_val <= 64) {
                err = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, param_val);
            } else {
                // Test with invalid values
                err = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, param_val);
            }
            break;

        case 1: // aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
            // Coeff cost update frequency: usually 0 to some max
            if (param_val >= 0) {
                err = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, param_val);
            } else {
                err = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, param_val);
            }
            break;

        case 2: // aom_codec_control_typechecked_AV1E_SET_TIMING_INFO_TYPE
            // Timing info type: 0, 1, 2
            int timing_type = param_val % 3;
            err = aom_codec_control(ctx, AV1E_SET_TIMING_INFO_TYPE, timing_type);
            break;

        case 3: // aom_codec_control_typechecked_AV1E_SET_MAX_PARTITION_SIZE
            // Max partition size: typically 4, 8, 16, 32, 64
            if (param_val > 0 && param_val <= 64) {
                err = aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, param_val);
            } else {
                err = aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, param_val);
            }
            break;

        case 4: // aom_codec_control_typechecked_AV1E_SET_EXTERNAL_PARTITION
            // This requires a pointer to aom_ext_part_funcs_t with valid function pointers
            // Since we cannot provide real implementations, we skip this command to avoid crashes
            // The library expects non-NULL function pointers for create_model, send_features, etc.
            // To fuzz safely without implementing callbacks, we skip this case.
            break;

        case 5: // aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC
            // RTC external RC: boolean (0 or 1)
            int rtc_external = (param_val != 0) ? 1 : 0;
            err = aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_external);
            break;

        default:
            break;
    }

    // Cleanup
    cleanup_encoder_context(ctx);

    return 0;
}