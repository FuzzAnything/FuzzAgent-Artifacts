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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    // Ensure the value is within a reasonable range for boolean flags or enums
    // Most of these controls expect 0 or 1, or specific enum values.
    // We fuzz with 0, 1, and potentially random values if the API allows.
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure the encoder slightly to vary state
    cfg.g_w = (Data[0] % 128) + 16; // Width between 16 and 143
    cfg.g_h = (Data[1] % 128) + 16; // Height between 16 and 143
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize the encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control IDs from the input data to diversify testing
    // We map the input bytes to specific control IDs
    int control_ids[] = {
        AV1E_SET_S_FRAME_MODE,
        AV1E_SET_ENABLE_INTERINTER_WEDGE,
        AV1E_SET_ENABLE_REF_FRAME_MVS,
        AV1E_SET_COLOR_PRIMARIES,
        AV1E_SET_ENABLE_FILTER_INTRA,
        AV1E_SET_ALLOW_WARPED_MOTION
    };
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);

    // Iterate through the input data to trigger different controls
    for (size_t i = 0; i < Size && i < 100; i++) {
        int idx = Data[i] % num_controls;
        int value = (Data[i] >> 4) % 2; // Use upper nibble for value (0 or 1)
        
        // For COLOR_PRIMARIES, we might want a wider range if it accepts enums
        if (control_ids[idx] == AV1E_SET_COLOR_PRIMARIES) {
            value = Data[i] % 256; // Allow a wider range for color primaries
        }

        fuzz_encoder_control(&ctx, control_ids[idx], value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}