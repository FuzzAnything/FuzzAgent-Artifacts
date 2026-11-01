// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

// Helper to safely free global headers
static void free_global_headers(aom_fixed_buf_t *headers) {
    if (headers) {
        if (headers->buf) {
            free(headers->buf);
        }
        free(headers);
    }
}

// Helper to create a dummy image frame for encoding
static aom_image_t *create_dummy_image(unsigned int w, unsigned int h) {
    if (w == 0) w = 16;
    if (h == 0) h = 16;
    
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;
    
    // Fill with simple pattern using correct member names
    for (int i = 0; i < img->h; i++) {
        for (int j = 0; j < img->w; j++) {
            img->planes[0][i * img->stride[0] + j] = (i + j) % 256;
        }
    }
    for (int p = 1; p < 3; p++) {
        int h_p = img->h >> (p - 1);
        int w_p = img->w >> (p - 1);
        for (int i = 0; i < h_p; i++) {
            for (int j = 0; j < w_p; j++) {
                img->planes[p][i * img->stride[p] + j] = 128;
            }
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_204(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t *img;
    aom_fixed_buf_t *headers = NULL;
    aom_codec_err_t err;
    int width = Data[0];
    int height = Data[1];
    
    // Ensure reasonable dimensions
    if (width < 8) width = 16;
    if (height < 8) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_usage = AOM_USAGE_REALTIME;

    // Initialize codec context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Create a dummy image
    img = create_dummy_image(width, height);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz aom_codec_set_option with input data
    if (Size > 4) {
        // Create null-terminated strings from input
        char key[64], value[64];
        size_t key_len = (Size - 4 < 63) ? Size - 4 : 63;
        size_t val_len = (Size - 4 < 63) ? Size - 4 : 63;
        
        memcpy(key, Data + 2, key_len);
        key[key_len] = '\0';
        
        memcpy(value, Data + 2, val_len);
        value[val_len] = '\0';
        
        // Try setting option
        aom_codec_set_option(&ctx, key, value);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ERROR_RESILIENT_MODE
    // This function sets error resilient mode (0 or 1)
    if (Size > 2) {
        int mode = Data[2] % 2;
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, mode);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
    if (Size > 3) {
        int enable = Data[3] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, enable);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_NUM_TG
    if (Size > 4) {
        int num_tg = Data[4] % 8; // Limit to reasonable range
        aom_codec_control(&ctx, AV1E_SET_NUM_TG, num_tg);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_SUPERRES
    if (Size > 5) {
        int enable = Data[5] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, enable);
    }

    // Encode one frame
    // Fixed: aom_codec_encode expects 5 arguments: ctx, img, pts, duration, flags
    err = aom_codec_encode(&ctx, img, 0, 1, 0);
    if (err == AOM_CODEC_OK) {
        // Try to get global headers
        headers = aom_codec_get_global_headers(&ctx);
        
        // If we got headers, try to process them (fuzzing the memory)
        if (headers && headers->buf && headers->sz > 0) {
            // Just ensure we can access the data
            volatile uint8_t dummy = ((uint8_t*)headers->buf)[0];
            (void)dummy;
        }
    }

    // Cleanup
    free_global_headers(headers);
    aom_img_free(img);
    aom_codec_destroy(&ctx);
    
    return 0;
}