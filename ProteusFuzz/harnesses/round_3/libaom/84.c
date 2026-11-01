// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

/*
 * Fuzz driver for libaom encoder control functions.
 * Targets:
 * - AV1E_SET_DENOISE_BLOCK_SIZE
 * - AV1E_SET_DENOISE_NOISE_LEVEL
 * - AV1E_SET_ENABLE_INTRABC
 * - AV1E_SET_SKIP_POSTPROC_FILTERING
 * - AV1E_SET_ENABLE_RESTORATION
 * - AV1E_SET_TILE_ROWS
 */

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (ctx == NULL) return 0;
    
    // aom_codec_control returns AOM_CODEC_OK on success
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    
    // We don't strictly need to check res for fuzzing, but it helps logic flow
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    // We need to initialize an encoder context to use the control functions.
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure the encoder if possible, but keep it valid
    // We'll use a simple heuristic to set width/height from the first few bytes
    if (Size >= 2) {
        cfg.g_w = Data[0] ? Data[0] : 16;
        cfg.g_h = Data[1] ? Data[1] : 16;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    
    // Ensure dimensions are multiples of 16 for AV1
    cfg.g_w = (cfg.g_w / 16) * 16;
    cfg.g_h = (cfg.g_h / 16) * 16;
    if (cfg.g_w < 16) cfg.g_w = 16;
    if (cfg.g_h < 16) cfg.g_h = 16;

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;

    // Initialize the encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 2. Conduct fuzzing logic
    // We will extract values from the input buffer to drive the control functions.
    // We need at least 6 integers for the 6 target functions.
    
    int32_t val1 = 0, val2 = 0, val3 = 0, val4 = 0, val5 = 0, val6 = 0;
    
    if (Size >= 24) {
        memcpy(&val1, Data, sizeof(int32_t));
        memcpy(&val2, Data + 4, sizeof(int32_t));
        memcpy(&val3, Data + 8, sizeof(int32_t));
        memcpy(&val4, Data + 12, sizeof(int32_t));
        memcpy(&val5, Data + 16, sizeof(int32_t));
        memcpy(&val6, Data + 20, sizeof(int32_t));
    } else {
        // If data is too short, use deterministic values based on Size
        val1 = (int32_t)Size;
        val2 = (int32_t)(Size * 2);
        val3 = (int32_t)(Size * 3);
        val4 = (int32_t)(Size * 4);
        val5 = (int32_t)(Size * 5);
        val6 = (int32_t)(Size * 6);
    }

    // Fuzzing the specific control IDs using the correct enum names
    // 1. AV1E_SET_DENOISE_BLOCK_SIZE
    fuzz_encoder_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val1);
    
    // 2. AV1E_SET_DENOISE_NOISE_LEVEL
    fuzz_encoder_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, val2);
    
    // 3. AV1E_SET_ENABLE_INTRABC
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTRABC, val3);
    
    // 4. AV1E_SET_SKIP_POSTPROC_FILTERING
    fuzz_encoder_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val4);
    
    // 5. AV1E_SET_ENABLE_RESTORATION
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_RESTORATION, val5);
    
    // 6. AV1E_SET_TILE_ROWS
    fuzz_encoder_control(&ctx, AV1E_SET_TILE_ROWS, val6);

    // Try to encode a dummy frame to exercise the state changes
    // Create a dummy image
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    
    // Fill with fuzz data if available
    if (Size > 0) {
        // Fix: Use 'planes' instead of 'plane'
        if (img.planes[0] != NULL) {
            size_t copy_size = (Size < (size_t)(img.stride[0] * img.h)) ? Size : (size_t)(img.stride[0] * img.h);
            memcpy(img.planes[0], Data, copy_size);
        }
    }

    // Removed invalid AV1E_SET_QP call.
    // Encode the frame. Fixed argument count: ctx, img, pts, duration, flags
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // 3. Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}