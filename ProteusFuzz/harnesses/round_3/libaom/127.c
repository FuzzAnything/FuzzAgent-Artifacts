// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include "aom/aom.h"
#include "aom/aomcx.h"

// Helper to safely initialize encoder context
static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzz data to vary configuration slightly within safe bounds
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1] % 256) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.g_threads = (data[4] % 8) + 1;
        cfg.rc_target_bitrate = (data[5] % 1000) + 100;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 100;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to cleanup encoder context
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz various control commands using data from input
    // We map different parts of the input to different control IDs and values

    // Command 1: AV1E_SET_INTER_DCT_ONLY (if available via macro)
    #ifdef AV1E_SET_INTER_DCT_ONLY
    {
        int val = (Size > 0) ? (Data[0] % 2) : 0;
        aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
    }
    #endif

    // Command 2: AV1E_SET_ENABLE_1TO4_PARTITIONS
    #ifdef AV1E_SET_ENABLE_1TO4_PARTITIONS
    {
        int val = (Size > 1) ? (Data[1] % 2) : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val);
    }
    #endif

    // Command 3: AV1E_SET_ENABLE_MASKED_COMP
    #ifdef AV1E_SET_ENABLE_MASKED_COMP
    {
        int val = (Size > 2) ? (Data[2] % 2) : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_MASKED_COMP, val);
    }
    #endif

    // Command 4: AV1E_SET_ENABLE_AB_PARTITIONS
    #ifdef AV1E_SET_ENABLE_AB_PARTITIONS
    {
        int val = (Size > 3) ? (Data[3] % 2) : 0;
        aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);
    }
    #endif

    // Command 5: AV1E_SET_MIN_PARTITION_SIZE
    #ifdef AV1E_SET_MIN_PARTITION_SIZE
    {
        int val = (Size > 4) ? (Data[4] % 16) + 1 : 1; // Min partition size typically 1-16
        aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, val);
    }
    #endif

    // Command 6: AV1E_SET_REDUCED_TX_TYPE_SET
    #ifdef AV1E_SET_REDUCED_TX_TYPE_SET
    {
        int val = (Size > 5) ? (Data[5] % 2) : 0;
        aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    }
    #endif

    // Try to encode a dummy frame to trigger internal logic
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    
    // Fixed member name: planes instead of plane
    memset(img.planes[0], 128, 16 * 16);
    memset(img.planes[1], 128, 8 * 8);
    memset(img.planes[2], 128, 8 * 8);

    // Fixed function call: removed extra argument (flags)
    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_encoder_context(ctx);

    return 0;
}