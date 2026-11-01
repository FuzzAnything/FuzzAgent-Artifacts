// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>

/*
 * Helper function to initialize the codec context.
 * Returns 0 on success, -1 on failure.
 */
static int init_codec_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    if (!ctx || !data) {
        return -1;
    }

    // Use AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    // Use fuzzing data to configure width/height safely
    if (size >= 4) {
        uint32_t width = data[0] | (data[1] << 8);
        uint32_t height = data[2] | (data[3] << 8);
        // Ensure dimensions are valid and not too large
        if (width > 0 && width <= 8192 && height > 0 && height <= 8192) {
            cfg.g_w = width;
            cfg.g_h = height;
        } else {
            cfg.g_w = 64;
            cfg.g_h = 64;
        }
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

/*
 * Helper function to clean up the codec context.
 */
static void cleanup_codec_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int enable_val;

    if (Size < 1) {
        return 0;
    }

    // Initialize codec context
    if (init_codec_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Derive a boolean value from input data to toggle features
    enable_val = (Data[0] % 2) == 0 ? 1 : 0;

    // 1. AV1E_SET_ENABLE_PALETTE
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_val);
    (void)ret;

    // 2. AV1E_SET_ENABLE_MASKED_COMP
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, enable_val);
    (void)ret;

    // 3. AV1E_SET_ENABLE_SMOOTH_INTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, enable_val);
    (void)ret;

    // 4. AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, enable_val);
    (void)ret;

    // 5. AV1E_SET_ENABLE_OVERLAY
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_val);
    (void)ret;

    // 6. AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, enable_val);
    (void)ret;

    // Cleanup
    cleanup_codec_context(&ctx);

    return 0;
}