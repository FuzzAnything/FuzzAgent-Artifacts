// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"

// Declare the encoder interface function that's missing from headers
extern aom_codec_iface_t *aom_codec_av1_cx(void);

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;
    
    // Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic configuration from fuzzer data
    cfg.g_w = 64 + (Data[0] % 192);
    cfg.g_h = 64 + (Data[1] % 192);
    cfg.g_threads = 1 + (Data[2] % 4);
    cfg.rc_target_bitrate = 100 + (Data[3] % 1000);
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Try different control IDs based on fuzzer data
    if (Size > 8) {
        int ctrl_id = (int)Data[4];
        int ctrl_value = (int)Data[5];
        aom_codec_control(&ctx, ctrl_id, ctrl_value);
    }

    // Set options from fuzzer data
    if (Size > 20) {
        char key[16];
        char value[16];
        snprintf(key, sizeof(key), "key_%d", Data[6]);
        snprintf(value, sizeof(value), "value_%d", Data[7]);
        aom_codec_set_option(&ctx, key, value);
    }

    // Create a dummy image for encoding
    aom_image_t img;
    unsigned int width = cfg.g_w;
    unsigned int height = cfg.g_h;
    
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1) == NULL) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzzer data
    size_t y_plane_size = width * height;
    size_t uv_plane_size = (width * height) / 4;
    
    if (Size > y_plane_size + uv_plane_size * 2) {
        // Fill Y plane
        size_t y_copy = y_plane_size < Size ? y_plane_size : Size;
        memcpy(img.planes[AOM_PLANE_Y], Data, y_copy);
        
        // Fill U plane
        if (Size > y_copy + uv_plane_size) {
            memcpy(img.planes[AOM_PLANE_U], Data + y_copy, uv_plane_size);
        }
        
        // Fill V plane
        if (Size > y_copy + uv_plane_size * 2) {
            memcpy(img.planes[AOM_PLANE_V], Data + y_copy + uv_plane_size, uv_plane_size);
        }
    }

    // Encode the frame
    aom_codec_encode(&ctx, &img, 0, 1, 0);
    
    // Flush encoder
    aom_codec_encode(&ctx, NULL, 0, 1, 0);

    // Try to get preview frame
    const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
    (void)preview; // Use preview to avoid unused variable warning

    // Clean up
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}