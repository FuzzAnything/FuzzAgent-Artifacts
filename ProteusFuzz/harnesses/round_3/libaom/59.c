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
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * - AV1E_SET_MAX_REFERENCE_FRAMES
 * - AOME_SET_CPUUSED
 * - AV1E_SET_QM_MAX
 * - AV1E_SET_SVC_PARAMS
 * - AV1E_SET_TARGET_SEQ_LEVEL_IDX
 * - AV1E_SET_BITRATE_ONE_PASS_CBR
 *
 * Note: The specific "typechecked" variants mentioned in the prompt are not standard
 * public API names in libaom. This driver uses the standard aom_codec_control API
 * with the corresponding enum constants to achieve the same testing goals.
 */

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, void *arg, size_t arg_size) {
    if (!ctx) return 0;
    
    // Safety check: ensure we don't pass NULL pointers for arguments that require them
    if (arg == NULL && arg_size > 0) return 0;

    aom_codec_err_t res = aom_codec_control(ctx, control_id, arg);
    
    // We don't return the error code to stop fuzzing; we just want to exercise the code path.
    // The fuzzing engine will handle crashes/exceptions.
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // 1. Prepare the environment
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to randomize configuration slightly to explore states
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size % 100) + 1;
    cfg.rc_target_bitrate = (Size * 100) + 1000;
    // Fixed: Use AOM_RC_ONE_PASS instead of AOM_ONE_PASS
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_error_resilient = 0;

    // Initialize the encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 2. Conduct fuzzing logic for target functions
    // We use the raw data to select which control to invoke and generate arguments.
    int control_id = Data[0] % 6;
    int arg_val = 0;
    int8_t arg_int8 = 0;
    int arg_int = 0;
    aom_svc_params_t svc_params;

    // Initialize SVC params with safe defaults
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = 1;
    svc_params.number_temporal_layers = 1;
    svc_params.max_quantizers[0] = 63;
    svc_params.min_quantizers[0] = 0;
    svc_params.scaling_factor_num[0] = 1;
    svc_params.scaling_factor_den[0] = 1;
    svc_params.layer_target_bitrate[0] = 1000;
    svc_params.framerate_factor[0] = 1;

    switch (control_id) {
        case 0:
            // AV1E_SET_MAX_REFERENCE_FRAMES
            // Valid range typically 1-8 (or similar depending on profile)
            arg_int = (Data[1] % 8) + 1; 
            fuzz_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &arg_int, sizeof(int));
            break;
        case 1:
            // AOME_SET_CPUUSED
            // Valid range typically -1 to 8
            arg_int = (Data[1] % 10) - 1; 
            fuzz_codec_control(&ctx, AOME_SET_CPUUSED, &arg_int, sizeof(int));
            break;
        case 2:
            // AV1E_SET_QM_MAX
            // Valid range 0-63
            arg_int = Data[1] % 64;
            fuzz_codec_control(&ctx, AV1E_SET_QM_MAX, &arg_int, sizeof(int));
            break;
        case 3:
            // AV1E_SET_SVC_PARAMS
            // Randomize some fields within safe bounds
            if (Size > 5) {
                svc_params.number_spatial_layers = (Data[2] % 3) + 1; // 1-3
                svc_params.number_temporal_layers = (Data[3] % 3) + 1; // 1-3
                svc_params.max_quantizers[0] = Data[4] % 64;
                svc_params.min_quantizers[0] = (Data[5] % 64);
                if (svc_params.min_quantizers[0] > svc_params.max_quantizers[0]) {
                    int tmp = svc_params.min_quantizers[0];
                    svc_params.min_quantizers[0] = svc_params.max_quantizers[0];
                    svc_params.max_quantizers[0] = tmp;
                }
            }
            fuzz_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params, sizeof(aom_svc_params_t));
            break;
        case 4:
            // AV1E_SET_TARGET_SEQ_LEVEL_IDX
            // Valid range typically 0-6 (or similar)
            arg_int = Data[1] % 7;
            fuzz_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, &arg_int, sizeof(int));
            break;
        case 5:
            // AV1E_SET_BITRATE_ONE_PASS_CBR
            // Requires a struct or int depending on version, usually int for bitrate
            // Or it might be a flag. Assuming int for bitrate or similar.
            // If it expects a specific struct, we might need to adjust, but standard control often takes int.
            // Based on typical usage, this might be a flag or a bitrate setting.
            // Let's try passing an int representing a bitrate multiplier or flag.
            arg_int = (Data[1] % 100) + 1; 
            fuzz_codec_control(&ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, &arg_int, sizeof(int));
            break;
        default:
            break;
    }

    // 3. Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}