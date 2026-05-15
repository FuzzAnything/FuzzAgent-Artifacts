// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static aom_image_t* create_test_image(const uint8_t *data, size_t size) {
    static const unsigned int width = 64;
    static const unsigned int height = 64;
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;
    
    // Fill planes with fuzzer data (repeat if needed)
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char *plane_buf = img->planes[plane];
        int stride = img->stride[plane];
        int plane_width = (plane == 0) ? width : (width >> 1);
        int plane_height = (plane == 0) ? height : (height >> 1);
        
        for (int y = 0; y < plane_height; ++y) {
            for (int x = 0; x < plane_width; ++x) {
                size_t offset = (y * plane_width + x) % size;
                plane_buf[y * stride + x] = data[offset];
            }
        }
    }
    
    return img;
}

static void init_encoder_config(aom_codec_enc_cfg_t *cfg, aom_codec_iface_t *iface) {
    aom_codec_enc_config_default(iface, cfg, 0);
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->g_error_resilient = 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Initialize encoder
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    init_encoder_config(&cfg, iface);
    
    // Use fuzzer data to modify config
    if (size >= sizeof(cfg)) {
        memcpy(&cfg, data, sizeof(cfg) < size ? sizeof(cfg) : size);
        // Ensure some basic constraints
        cfg.g_w = cfg.g_w % 256 + 16;
        cfg.g_h = cfg.g_h % 256 + 16;
        cfg.g_threads = cfg.g_threads % 8 + 1;
        cfg.rc_target_bitrate = cfg.rc_target_bitrate % 10000 + 100;
    }
    
    // Initialize encoder with version check
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Test aom_codec_enc_config_set with modified config
    if (size > sizeof(cfg)) {
        aom_codec_enc_cfg_t new_cfg = cfg;
        // Modify some parameters based on fuzzer data
        size_t offset = sizeof(cfg) % size;
        new_cfg.rc_target_bitrate = (data[offset] * 100) % 5000 + 100;
        new_cfg.g_threads = (data[(offset + 1) % size] % 8) + 1;
        aom_codec_enc_config_set(&ctx, &new_cfg);
    }
    
    // Test aom_codec_control with various control IDs
    static const int control_ids[] = {
        AOME_SET_CPUUSED,
        AOME_SET_ENABLEAUTOALTREF,
        AOME_SET_SHARPNESS,
        AOME_SET_STATIC_THRESHOLD,
        AOME_SET_ARNR_MAXFRAMES,
        AOME_SET_ARNR_STRENGTH,
        AV1E_SET_TILE_COLUMNS,
        AV1E_SET_TILE_ROWS,
        AV1E_SET_ROW_MT,
    };
    
    for (size_t i = 0; i < sizeof(control_ids)/sizeof(control_ids[0]) && i < size; i++) {
        int value = data[i] % 256;
        aom_codec_control(&ctx, control_ids[i], value);
    }
    
    // Create test image
    aom_image_t *img = create_test_image(data, size);
    if (img) {
        // Test aom_codec_encode with image
        aom_codec_encode(&ctx, img, 0, 1, 0);
        
        // Test aom_codec_encode with NULL (flush)
        aom_codec_encode(&ctx, NULL, 0, 0, 0);
        
        aom_img_free(img);
    }
    
    // Test aom_codec_set_cx_data_buf
    if (size > 100) {
        aom_fixed_buf_t buf;
        buf.buf = malloc(4096);
        buf.sz = 4096;
        if (buf.buf) {
            unsigned int pad_before = data[0] % 32;
            unsigned int pad_after = data[1] % 32;
            aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
            free(buf.buf);
        }
    }
    
    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // Clean up
    aom_codec_destroy(&ctx);
    
    return 0;
}