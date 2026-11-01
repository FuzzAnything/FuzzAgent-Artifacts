// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

/*
 * Note: The functions listed in the task (e.g., aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY)
 * are not standard public API functions in libaom. They appear to be internal or obfuscated names.
 * The standard API for setting encoder options is aom_codec_control().
 * The following fuzz driver attempts to use the standard aom_codec_control() with the
 * standard AV1E_SET_* control IDs. If the target environment has renamed these functions
 * or if the provided names are strict requirements, the compilation will fail unless
 * the environment provides these specific symbols.
 *
 * To maximize compatibility and fuzzing effectiveness, this driver uses the standard
 * aom_codec_control() API with the known AV1E_* control constants.
 */

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_228(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int value;
    int control_id;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure width/height to avoid invalid states
    // Ensure dimensions are valid (multiples of 16 for AV1 usually, but >0 is safe for init)
    if (Size >= 2) {
        cfg.g_w = (Data[0] % 255) + 16;
        cfg.g_h = (Data[1] % 255) + 16;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize the encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Select control ID based on input data
    // Mapping:
    // 0: AV1E_SET_INTRA_DCT_ONLY
    // 1: AV1E_SET_ENABLE_QM
    // 2: AV1E_SET_ENABLE_CDEF
    // 3: AV1E_SET_ENABLE_KEYFRAME_FILTERING
    // 4: AV1E_SET_LOSSLESS
    // 5: AV1E_SET_FORCE_VIDEO_MODE
    int idx = Size % 6;

    switch (idx) {
        case 0:
            control_id = AV1E_SET_INTRA_DCT_ONLY;
            break;
        case 1:
            control_id = AV1E_SET_ENABLE_QM;
            break;
        case 2:
            control_id = AV1E_SET_ENABLE_CDEF;
            break;
        case 3:
            control_id = AV1E_SET_ENABLE_KEYFRAME_FILTERING;
            break;
        case 4:
            control_id = AV1E_SET_LOSSLESS;
            break;
        case 5:
            control_id = AV1E_SET_FORCE_VIDEO_MODE;
            break;
        default:
            control_id = AV1E_SET_INTRA_DCT_ONLY;
            break;
    }

    // Generate a value to pass to the control
    // Most of these controls expect an int (0 or 1), but some might accept ranges.
    // We use the first byte of data modulo 2 for boolean-like flags, 
    // or a small range for others if applicable.
    if (Size > 0) {
        value = Data[0] % 2; 
    } else {
        value = 0;
    }

    // Invoke the control function
    // Note: If the user strictly requires the specific function names provided in the prompt
    // (e.g., aom_codec_control_typechecked_AV1E_SET_INTRA_DCT_ONLY), those symbols
    // do not exist in the standard libaom public headers. This code uses the standard
    // aom_codec_control() which is the correct way to invoke these controls.
    res = aom_codec_control(&ctx, control_id, value);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}