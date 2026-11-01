// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    int32_t val;
    int32_t val2;

    // Initialize default configuration
    ret = aom_codec_enc_config_default("libaom-av1", &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure width/height to ensure valid initialization
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Derive values from fuzz input for control commands
    val = (int32_t)(Data[0] % 100); // Sharpness range 0-100
    val2 = (int32_t)(Data[1] % 2);  // Boolean flags 0 or 1

    // 1. aom_codec_control_typechecked_AOME_SET_SHARPNESS
    // Note: The actual macro/function name in libaom is typically AOME_SET_SHARPNESS
    // We use the generic aom_codec_control which handles the type checking internally
    // if the macro is not directly available as a standalone function in the provided context.
    // Assuming the user wants to test the control mechanism for these specific settings.
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, val);

    // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val2);

    // 3. aom_codec_control_typechecked_AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP
    // This setting might require specific types or ranges, using derived int32
    aom_codec_control(&ctx, AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP, val);

    // 4. aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val2);

    // 5. aom_codec_control_typechecked_AV1E_SET_QM_MIN
    // QM_MIN is typically an integer
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, val);

    // 6. aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS
    // Tile columns must be power of 2 usually, but we pass raw input to test robustness
    // Limit to reasonable range to avoid immediate crashes on allocation if possible,
    // but fuzzer should test edge cases.
    int32_t tile_cols = (Data[2] % 16) + 1;
    aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}