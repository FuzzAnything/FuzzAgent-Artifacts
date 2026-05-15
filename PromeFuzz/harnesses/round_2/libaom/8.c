// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

static aom_image_t *create_test_image(unsigned int width, unsigned int height, 
                                      aom_img_fmt_t fmt, unsigned int bit_depth) {
    aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    // Fill planes with some data
    for (int plane = 0; plane < 3; ++plane) {
        if (img->planes[plane]) {
            int stride = img->stride[plane];
            int plane_height = (plane == 0) ? height : (height >> img->y_chroma_shift);
            for (int y = 0; y < plane_height; ++y) {
                memset(img->planes[plane] + y * stride, y % 256, stride);
            }
        }
    }
    
    return img;
}

static void parse_fuzz_data(const uint8_t *data, size_t size, 
                           char *key, size_t key_len,
                           char *value, size_t value_len,
                           int *control_id, int *control_val) {
    if (size < 4) return;
    
    // Extract key-value pair (simplified)
    size_t key_size = data[0] % (key_len - 1);
    if (key_size > 0 && key_size < size) {
        memcpy(key, data + 1, key_size);
        key[key_size] = '\0';
    }
    
    size_t value_offset = 1 + key_size;
    if (value_offset < size) {
        size_t value_size = data[value_offset] % (value_len - 1);
        if (value_size > 0 && value_offset + 1 + value_size < size) {
            memcpy(value, data + value_offset + 1, value_size);
            value[value_size] = '\0';
        }
    }
    
    // Extract control parameters
    size_t control_offset = value_offset + 1 + (value_offset < size ? data[value_offset] % (value_len - 1) : 0);
    if (control_offset + 8 <= size) {
        *control_id = *(int*)(data + control_offset);
        *control_val = *(int*)(data + control_offset + 4);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
    if (size < 16) return 0;
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    // Initialize with default config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Modify config with fuzz data
    cfg.g_w = 64 + (data[0] % 192);  // 64-255 width
    cfg.g_h = 64 + (data[1] % 192);  // 64-255 height
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100 + (data[2] % 1900);  // 100-1999 kbps
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = data[3] % 16;
    cfg.g_error_resilient = data[4] % 2;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Parse fuzz data for key-value pairs and control parameters
    char key[256] = {0};
    char value[256] = {0};
    int control_id = 0;
    int control_val = 0;
    
    parse_fuzz_data(data, size, key, sizeof(key), value, sizeof(value), 
                   &control_id, &control_val);
    
    // Test aom_codec_set_option
    if (key[0] != '\0' && value[0] != '\0') {
        aom_codec_set_option(&ctx, key, value);
    }
    
    // Test aom_codec_control with various control IDs
    if (control_id != 0) {
        // Try a few common control IDs from aomcx.h
        int test_controls[] = {AOME_SET_CPUUSED, AOME_SET_ENABLEAUTOALTREF, 
                              AOME_SET_SHARPNESS, AOME_SET_STATIC_THRESHOLD};
        for (int i = 0; i < sizeof(test_controls)/sizeof(test_controls[0]); i++) {
            int val = (control_val + i) % 10;
            aom_codec_control(&ctx, test_controls[i], val);
        }
    }
    
    // Create test image
    aom_image_t *img = create_test_image(cfg.g_w, cfg.g_h, AOM_IMG_FMT_I420, 8);
    if (img) {
        // Test aom_codec_encode with image
        aom_codec_encode(&ctx, img, 0, 1, 0);
        
        // Test aom_codec_encode with NULL (flush)
        aom_codec_encode(&ctx, NULL, 0, 0, 0);
        
        aom_img_free(img);
    }
    
    // Test aom_codec_enc_config_set with modified config
    cfg.rc_target_bitrate = 500 + (data[5] % 1500);
    cfg.g_lag_in_frames = data[6] % 8;
    aom_codec_enc_config_set(&ctx, &cfg);
    
    // Clean up
    aom_codec_destroy(&ctx);
    
    return 0;
}