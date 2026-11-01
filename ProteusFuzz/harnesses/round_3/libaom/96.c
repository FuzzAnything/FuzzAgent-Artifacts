// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // Ensure the value is within a reasonable range for boolean-like flags
    if (value < 0 || value > 1) {
        // Fuzz with random values occasionally to test edge cases
        value = (value % 2);
    }

    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    if (ret != AOM_CODEC_OK) {
        // Non-critical error, just return to continue fuzzing
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(NULL, &cfg, 0);

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[0] + Data[1]) * 1000 + 1000;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control IDs from input data
    int control_ids[] = {
        AV1E_SET_NOISE_SENSITIVITY,
        AV1E_SET_ENABLE_DIRECTIONAL_INTRA,
        AV1E_SET_ENABLE_DIAGONAL_INTRA,
        AV1E_SET_ENABLE_INTRABC,
        AV1E_SET_ENABLE_FLIP_IDTX,
        AV1E_SET_ENABLE_PAETH_INTRA
    };
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);

    // Fuzz each control with different values
    for (size_t i = 0; i < Size; i++) {
        int control_id = control_ids[i % num_controls];
        int value = Data[i] % 2; // Boolean-like value
        fuzz_encoder_control(&ctx, control_id, value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}