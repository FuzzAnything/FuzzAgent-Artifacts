// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 */

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_282(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Initialize encoder config
    if (aom_codec_enc_config_default("av1", &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure basic parameters
    cfg.g_w = (Data[0] % 256) + 16; // Width
    cfg.g_h = (Data[1] % 256) + 16; // Height
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.rc_target_bitrate = (Data[5] % 1000) + 100;

    // Initialize the codec
    res = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control values based on input data
    int val1 = (Data[6] % 2); // 0 or 1
    int val2 = (Data[7] % 2);
    int val3 = (Data[8] % 2);
    int val4 = (Data[9] % 2);
    int val5 = (Data[10] % 2);
    int val6 = (Data[11] % 2);

    // Attempt to invoke the target control functions.
    // Note: These macros are expected to be defined in aomcx.h or similar headers
    // in the actual build environment. If they are missing, the compiler will error,
    // but the logic assumes they map to aom_codec_control calls.

    // 1. AV1E_SET_ENABLE_CFL_INTRA
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, val1) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 2. AV1E_ENABLE_EXT_TILE_DEBUG
    if (aom_codec_control(&ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val2) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 3. AV1E_SET_ENABLE_RECT_PARTITIONS
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val3) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 4. AV1E_SET_ENABLE_PALETTE
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val4) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 5. AV1_SET_TILE_MODE
    if (aom_codec_control(&ctx, AV1_SET_TILE_MODE, val5) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // 6. AV1E_SET_SINGLE_TILE_DECODING
    if (aom_codec_control(&ctx, AV1E_SET_SINGLE_TILE_DECODING, val6) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Additional exploration: Try with larger values or different ranges if valid
    if (Size > 12) {
        int val_large = Data[12] * 256 + Data[13];
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, val_large) != AOM_CODEC_OK) {}
        if (aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val_large) != AOM_CODEC_OK) {}
    }

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}