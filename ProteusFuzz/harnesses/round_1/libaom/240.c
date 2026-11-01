// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * - aom_codec_control_typechecked_AV1E_SET_QM_MAX
 * - aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY
 * - aom_codec_control_typechecked_AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
 * - aom_codec_control_typechecked_AV1E_SET_S_FRAME_MODE
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
 * - aom_codec_control_typechecked_AOME_SET_TUNING
 *
 * Note: The specific type-checked macros may not exist in all libaom versions.
 * This driver attempts to use them if available, otherwise falls back to standard
 * aom_codec_control calls with the corresponding control IDs.
 */

static void init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }

    // Set some basic config to ensure valid initialization
    cfg->g_w = 32;
    cfg->g_h = 32;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }
}

int LLVMFuzzerTestOneInput_240(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    init_encoder_context(&ctx, &cfg);

    if (!ctx.iface) {
        return 0;
    }

    // Use the first byte to select the control function to test
    int control_id = Data[0] % 6;
    // Use the second byte as a parameter value
    int param_val = Data[1];

    aom_codec_err_t result = AOM_CODEC_ERROR;

    // Map control_id to the specific control command
    // We use the standard aom_codec_control API. 
    // The type-checked macros (e.g., aom_codec_control_typechecked_AV1E_SET_QM_MAX)
    // are typically defined in aom/aomcx.h or similar headers and expand to aom_codec_control
    // with type checking. If the macros are not defined in the build environment,
    // we call aom_codec_control directly with the corresponding enum values.
    
    // Common AV1 encoder control IDs (values may vary slightly by version, but these are standard)
    // AV1E_SET_QM_MAX
    // AV1E_SET_NOISE_SENSITIVITY
    // AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    // AV1E_SET_S_FRAME_MODE
    // AV1E_SET_ENABLE_INTRABC
    // AOME_SET_TUNING

    switch (control_id) {
        case 0: // AV1E_SET_QM_MAX
            // Expected type: int (0-255 usually)
            // If macro exists, use it. Otherwise use aom_codec_control.
            // Since we cannot guarantee the macro exists in the user's environment,
            // we use the standard API with the known enum value if possible, 
            // or rely on the macro if it expands correctly.
            // Assuming the macro aom_codec_control_typechecked_AV1E_SET_QM_MAX exists:
            #ifdef AV1E_SET_QM_MAX
                result = aom_codec_control(&ctx, AV1E_SET_QM_MAX, param_val);
            #else
                // Fallback if macro/enum not directly available in this context
                // This is a heuristic; in a real fuzzing setup, the header should define these.
                // We assume the macro expands to a call with the correct enum.
                // If the macro is missing, this branch might not compile if we try to use the enum name directly without definition.
                // However, the prompt asks to fuzz the "typechecked" functions.
                // We will assume the headers provided in the environment define these macros.
                // If not, the compiler will error out, which is expected if the API is missing.
                // To be safe and compile even if macros are missing, we might need to skip or use a generic approach,
                // but the task is to fuzz these specific functions.
                // Let's assume the macros are defined as per the task description.
                result = aom_codec_control(&ctx, AV1E_SET_QM_MAX, param_val);
            #endif
            break;
        case 1: // AV1E_SET_NOISE_SENSITIVITY
            #ifdef AV1E_SET_NOISE_SENSITIVITY
                result = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, param_val);
            #else
                result = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, param_val);
            #endif
            break;
        case 2: // AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
            #ifdef AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
                result = aom_codec_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, param_val);
            #else
                result = aom_codec_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, param_val);
            #endif
            break;
        case 3: // AV1E_SET_S_FRAME_MODE
            #ifdef AV1E_SET_S_FRAME_MODE
                result = aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, param_val);
            #else
                result = aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, param_val);
            #endif
            break;
        case 4: // AV1E_SET_ENABLE_INTRABC
            #ifdef AV1E_SET_ENABLE_INTRABC
                result = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, param_val);
            #else
                result = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, param_val);
            #endif
            break;
        case 5: // AOME_SET_TUNING
            #ifdef AOME_SET_TUNING
                result = aom_codec_control(&ctx, AOME_SET_TUNING, param_val);
            #else
                result = aom_codec_control(&ctx, AOME_SET_TUNING, param_val);
            #endif
            break;
        default:
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}