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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int test_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // Try to set the control with the provided value
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, value);
    if (ret != AOM_CODEC_OK) {
        // Return 0 on error to continue fuzzing other paths
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_279(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to configure some parameters
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.g_threads = (Data[4] % 4) + 1;
    cfg.g_lag_in_frames = (Data[5] % 25) + 1;
    cfg.rc_target_bitrate = (Data[6] % 1000) + 100;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input to use as control parameters
    int value = Data[Size - 1] % 2; // 0 or 1 for boolean-like controls

    // Test various control IDs using correct AV1E_ prefixes
    // Note: AV1E_SET_ENABLE_DIST_8X8 was removed/renamed in newer libaom versions.
    // We skip it or replace with a valid one if needed. For now, we omit it to avoid compilation errors.
    test_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, value);
    test_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, value);
    test_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, value);
    test_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, value);
    test_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, value);
    // AV1E_SET_ENABLE_DIST_8X8 is not available in current headers, skipping to avoid compile error

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}