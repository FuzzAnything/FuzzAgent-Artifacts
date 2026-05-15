// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"

static aom_image_t* create_dummy_image(size_t width, size_t height, aom_img_fmt_t fmt, unsigned int bit_depth) {
    aom_image_t* img = aom_img_alloc(NULL, fmt, width, height, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = width;
    img->d_h = height;
    img->r_w = width;
    img->r_h = height;
    
    // Fill planes with some data
    for (int plane = 0; plane < 3; plane++) {
        if (img->planes[plane]) {
            size_t plane_size = img->stride[plane] * (height >> (plane ? img->y_chroma_shift : 0));
            for (size_t i = 0; i < plane_size; i++) {
                img->planes[plane][i] = (uint8_t)(i % 256);
            }
        }
    }
    
    return img;
}

static void free_dummy_image(aom_image_t* img) {
    if (img) {
        aom_img_free(img);
    }
}

static aom_fixed_buf_t* create_dummy_buffer(size_t size) {
    aom_fixed_buf_t* buf = (aom_fixed_buf_t*)malloc(sizeof(aom_fixed_buf_t));
    if (!buf) return NULL;
    
    buf->buf = malloc(size);
    buf->sz = size;
    
    if (buf->buf) {
        memset(buf->buf, 0, size);
    }
    
    return buf;
}

static void free_dummy_buffer(aom_fixed_buf_t* buf) {
    if (buf) {
        if (buf->buf) free(buf->buf);
        free(buf);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize encoder context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Create default configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t* iface = NULL;
    
    // Try to get encoder interface - different ways to initialize
    if (Size > 0) {
        // Use aom_codec_av1_cx() if available, otherwise try to initialize differently
        // We'll use aom_codec_enc_config_default with a dummy interface pointer
        iface = (aom_codec_iface_t*)&ctx; // Dummy pointer for initialization
        
        // Initialize with default config using 0 as usage
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            // Try with different usage values
            for (int usage = 0; usage < 4; usage++) {
                if (aom_codec_enc_config_default(iface, &cfg, usage) == AOM_CODEC_OK) {
                    break;
                }
            }
        }
    }
    
    // Set some basic configuration from fuzzer data
    if (Size > 4) {
        cfg.g_w = 64 + (Data[0] % 192);  // 64-256 width
        cfg.g_h = 64 + (Data[1] % 192);  // 64-256 height
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        cfg.g_pass = AOM_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        cfg.g_error_resilient = 0;
    }
    
    // Initialize encoder with version check
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        // Try to clean up and return if initialization fails
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    // Test aom_codec_control with various control IDs
    if (Size > 8) {
        // Use fuzzer data to generate control IDs and values
        for (size_t i = 0; i < 8 && i + 4 < Size; i += 4) {
            int ctrl_id = Data[i] | (Data[i+1] << 8);
            int ctrl_value = Data[i+2] | (Data[i+3] << 8);
            
            // Try different control types based on control ID
            if (ctrl_id & 1) {
                aom_codec_control(&ctx, ctrl_id, ctrl_value);
            } else {
                aom_codec_control(&ctx, ctrl_id, (void*)(uintptr_t)ctrl_value);
            }
        }
    }
    
    // Test aom_codec_enc_config_set with modified configuration
    if (Size > 16) {
        // Modify some configuration parameters
        cfg.rc_min_quantizer = Data[8] % 64;
        cfg.rc_max_quantizer = 64 + (Data[9] % 64);
        cfg.g_threads = 1 + (Data[10] % 4);
        
        aom_codec_enc_config_set(&ctx, &cfg);
    }
    
    // Test aom_codec_set_cx_data_buf
    if (Size > 24) {
        size_t buf_size = 1024 + (Data[16] % 8192);
        aom_fixed_buf_t* cx_buf = create_dummy_buffer(buf_size);
        if (cx_buf) {
            unsigned int pad_before = Data[17] % 64;
            unsigned int pad_after = Data[18] % 64;
            aom_codec_set_cx_data_buf(&ctx, cx_buf, pad_before, pad_after);
            free_dummy_buffer(cx_buf);
        }
    }
    
    // Test aom_codec_encode with dummy image
    if (Size > 32) {
        // Create dummy image
        aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
        unsigned int bit_depth = 8;
        
        if (Data[24] & 1) {
            fmt = AOM_IMG_FMT_I42016;
            bit_depth = 10;
        }
        
        aom_image_t* img = create_dummy_image(cfg.g_w, cfg.g_h, fmt, bit_depth);
        if (img) {
            aom_codec_pts_t pts = Data[25] | (Data[26] << 8) | (Data[27] << 16) | (Data[28] << 24);
            unsigned long duration = 1;
            aom_enc_frame_flags_t flags = Data[29] % 4;
            
            aom_codec_encode(&ctx, img, pts, duration, flags);
            
            // Also test flush with NULL image
            aom_codec_encode(&ctx, NULL, 0, 0, 0);
            
            free_dummy_image(img);
        }
    }
    
    // Test aom_codec_get_global_headers
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }
    
    // Clean up
    aom_codec_destroy(&ctx);
    
    return 0;
}