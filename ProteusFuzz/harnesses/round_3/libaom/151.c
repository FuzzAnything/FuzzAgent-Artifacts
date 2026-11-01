// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aom.h"

// Helper function to initialize the encoder context
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    const aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_image_t *img;

    // Get the AV1 encoder interface using the correct function name
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    // Initialize configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    // Use fuzzing data to set some configuration parameters safely
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16; // Width between 16 and 1039
        cfg.g_h = (data[1] % 1024) + 16; // Height between 16 and 1039
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (data[2] % 60) + 1; // Framerate between 1 and 60
        cfg.rc_target_bitrate = (data[3] % 1000) + 100; // Bitrate
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
    }

    // Align dimensions to macroblock size (8x8)
    cfg.g_w = (cfg.g_w + 7) & ~7;
    cfg.g_h = (cfg.g_h + 7) & ~7;

    // Initialize the codec context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    // Create a dummy input image
    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        aom_codec_destroy(&ctx);
        return NULL;
    }

    // Fill image with fuzzing data if available
    if (size > 0) {
        size_t bytes_per_frame = (cfg.g_w * cfg.g_h * 3) / 2;
        size_t copy_size = (size < bytes_per_frame) ? size : bytes_per_frame;
        memcpy(img->planes[0], data, copy_size);
    }

    // Store the image pointer in the private data for later use
    // We cast to void* to avoid accessing private struct members directly
    ctx->priv = (aom_codec_priv_t *)img;

    return ctx;
}

// Helper function to cleanup the encoder context
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        // Retrieve the image pointer we stored
        aom_image_t *img = (aom_image_t *)ctx->priv;
        aom_codec_destroy(&ctx);
        if (img) {
            aom_img_free(img);
        }
    }
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) {
        return 0;
    }

    // Extract parameters from fuzzing data
    int enable_directional_intra = (Data[0] % 2);
    int error_resilient_mode = (Data[1] % 3); // 0, 1, or 2
    int enable_diagonal_intra = (Data[2] % 2);
    int enable_paeth_intra = (Data[3] % 2);
    int min_partition_size = (Data[4 % Size] % 4) + 1; // 1 to 4

    // Prepare SVC ref frame config
    aom_svc_ref_frame_config_t svc_config;
    memset(&svc_config, 0, sizeof(svc_config));
    // Set some default values for SVC config
    for (int i = 0; i < 7; i++) {
        svc_config.reference[i] = (Data[(5 + i) % Size] % 2);
        svc_config.ref_idx[i] = (Data[(12 + i) % Size] % 8);
    }
    for (int i = 0; i < 8; i++) {
        svc_config.refresh[i] = (Data[(19 + i) % Size] % 2);
    }

    // Call the target API functions using the correct control IDs
    // Note: The specific control IDs are defined in aom_encoder.h or aomcx.h
    // We use the standard aom_codec_control interface.
    
    // AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_directional_intra);
    
    // AV1E_SET_ERROR_RESILIENT_MODE
    aom_codec_control(ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);
    
    // AV1E_SET_ENABLE_DIAGONAL_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, enable_diagonal_intra);
    
    // AV1E_SET_ENABLE_PAETH_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth_intra);
    
    // AV1E_SET_MIN_PARTITION_SIZE
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, min_partition_size);
    
    // AV1E_SET_SVC_REF_FRAME_CONFIG
    aom_codec_control(ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_config);

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}