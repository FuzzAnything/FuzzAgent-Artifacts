// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->priv) return 0;

    // Helper to extract integer values from fuzz data safely
    int val = 0;
    if (size > 0) {
        val = data[0];
        if (size > 1) val |= (data[1] << 8);
        if (size > 2) val |= (data[2] << 16);
        if (size > 3) val |= (data[3] << 24);
    }

    switch (control_id) {
        case AV1E_SET_MATRIX_COEFFICIENTS:
            return aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, val);
        case AV1E_SET_COLOR_PRIMARIES:
            return aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, val);
        case AV1E_SET_ALLOW_WARPED_MOTION:
            return aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val & 1);
        case AV1E_SET_FILM_GRAIN_TEST_VECTOR:
            // AV1E_SET_FILM_GRAIN_TEST_VECTOR expects a pointer to aom_film_grain_params_t.
            // Since the struct definition is not available in the provided headers and might be internal,
            // we cannot safely construct it. We skip this specific control to avoid compilation errors
            // or undefined behavior, as we cannot guarantee the memory layout.
            // In a real fuzzing scenario, if the struct was exposed, we would cast 'data' to it.
            // For now, we return 0 to indicate no action taken for this specific control in this driver.
            return 0;
        case AV1E_SET_COLOR_RANGE:
            return aom_codec_control(ctx, AV1E_SET_COLOR_RANGE, val);
        case AV1E_SET_REDUCED_REFERENCE_SET:
            return aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val & 1);
        default:
            return 0;
    }
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely read indices 0 to 4
    if (Size < 5) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Configure basic settings from fuzz data to vary states
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[4] % 100) + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Determine which control to fuzz based on data
    int control_id = Data[4] % 6;
    const uint8_t *control_data = &Data[5];
    size_t control_size = Size > 5 ? Size - 5 : 0;

    // Map 0-5 to the specific control IDs
    int mapped_controls[] = {
        AV1E_SET_MATRIX_COEFFICIENTS,
        AV1E_SET_COLOR_PRIMARIES,
        AV1E_SET_ALLOW_WARPED_MOTION,
        AV1E_SET_FILM_GRAIN_TEST_VECTOR,
        AV1E_SET_COLOR_RANGE,
        AV1E_SET_REDUCED_REFERENCE_SET
    };

    int actual_control = mapped_controls[control_id];

    // Invoke the target function
    fuzz_encoder_control(&ctx, actual_control, control_data, control_size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}