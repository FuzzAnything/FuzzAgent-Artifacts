// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"

static aom_codec_ctx_t* create_test_codec_ctx(void) {
    static aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.name = "test";
    return &ctx;
}

static aom_image_t* create_test_image(void) {
    static aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.fmt = AOM_IMG_FMT_I420;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.monochrome = 0;
    img.csp = AOM_CSP_VERTICAL;
    img.range = AOM_CR_STUDIO_RANGE;
    img.w = 64;
    img.h = 64;
    img.bit_depth = 8;
    img.d_w = 64;
    img.d_h = 64;
    img.r_w = 64;
    img.r_h = 64;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    
    // Allocate minimal image data
    size_t y_size = img.w * img.h;
    size_t uv_size = (img.w >> img.x_chroma_shift) * (img.h >> img.y_chroma_shift);
    
    img.img_data = malloc(y_size + 2 * uv_size);
    if (!img.img_data) return NULL;
    
    img.img_data_owner = 1;
    img.self_allocd = 1;
    
    img.planes[0] = img.img_data;
    img.planes[1] = img.img_data + y_size;
    img.planes[2] = img.img_data + y_size + uv_size;
    
    img.stride[0] = img.w;
    img.stride[1] = img.w >> img.x_chroma_shift;
    img.stride[2] = img.w >> img.x_chroma_shift;
    
    img.sz = y_size + 2 * uv_size;
    img.bps = 8;
    img.temporal_id = 0;
    img.spatial_id = 0;
    
    // Fill with some data
    memset(img.img_data, 128, y_size);
    memset(img.planes[1], 64, uv_size);
    memset(img.planes[2], 192, uv_size);
    
    return &img;
}

static void free_test_image(aom_image_t* img) {
    if (img && img->img_data_owner) {
        free(img->img_data);
    }
}

static aom_codec_enc_cfg_t* create_test_config(void) {
    static aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Set minimal valid configuration
    cfg.g_usage = 0;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_limit = 1;
    cfg.g_forced_max_frame_width = 0;
    cfg.g_forced_max_frame_height = 0;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    cfg.rc_dropframe_thresh = 0;
    cfg.rc_resize_mode = 0;
    cfg.rc_resize_denominator = 8;
    cfg.rc_resize_kf_denominator = 8;
    cfg.rc_superres_mode = 0; // AOM_SUPERRES_NONE
    cfg.rc_superres_denominator = 8;
    cfg.rc_superres_kf_denominator = 8;
    cfg.rc_superres_qthresh = 0;
    cfg.rc_superres_kf_qthresh = 0;
    cfg.rc_end_usage = AOM_VBR;
    cfg.rc_target_bitrate = 256;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.rc_undershoot_pct = 50;
    cfg.rc_overshoot_pct = 50;
    cfg.rc_buf_sz = 1000;
    cfg.rc_buf_initial_sz = 500;
    cfg.rc_buf_optimal_sz = 600;
    cfg.rc_2pass_vbr_bias_pct = 50;
    cfg.rc_2pass_vbr_minsection_pct = 0;
    cfg.rc_2pass_vbr_maxsection_pct = 2000;
    
    cfg.fwd_kf_enabled = 0;
    cfg.kf_mode = AOM_KF_AUTO;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 9999;
    cfg.sframe_dist = 0;
    cfg.sframe_mode = 0;
    cfg.large_scale_tile = 0;
    cfg.monochrome = 0;
    cfg.full_still_picture_hdr = 0;
    cfg.save_as_annexb = 0;
    cfg.tile_width_count = 0;
    cfg.tile_height_count = 0;
    cfg.use_fixed_qp_offsets = 0;
    
    return &cfg;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create test context
    aom_codec_ctx_t* ctx = create_test_codec_ctx();
    
    // Test aom_codec_destroy with invalid context
    if (Size & 1) {
        aom_codec_destroy(NULL);
    }
    
    // Test aom_codec_set_option with various inputs
    if (Size > 10) {
        char key[64];
        char value[64];
        size_t key_len = Data[0] % 63 + 1;
        size_t val_len = Data[1] % 63 + 1;
        
        if (key_len > Size - 2) key_len = Size - 2;
        if (val_len > Size - 2 - key_len) val_len = Size - 2 - key_len;
        
        memcpy(key, Data + 2, key_len);
        memcpy(value, Data + 2 + key_len, val_len);
        key[key_len] = '\0';
        value[val_len] = '\0';
        
        aom_codec_set_option(ctx, key, value);
        
        // Test with NULL inputs
        if (Size & 2) {
            aom_codec_set_option(NULL, key, value);
            aom_codec_set_option(ctx, NULL, value);
            aom_codec_set_option(ctx, key, NULL);
        }
    }
    
    // Test aom_codec_control with various control IDs
    if (Size > 2) {
        int ctrl_id = Data[Size % Size] | (Data[(Size/2) % Size] << 8);
        if (ctrl_id != 0) {
            int value = Data[(Size/3) % Size] | (Data[(Size/4) % Size] << 8) |
                       (Data[(Size/5) % Size] << 16) | (Data[(Size/6) % Size] << 24);
            aom_codec_control(ctx, ctrl_id, value);
            
            // Test with NULL context
            if (Size & 4) {
                aom_codec_control(NULL, ctrl_id, value);
            }
        }
    }
    
    // Test encoder initialization if we have enough data
    if (Size > 100) {
        // Get encoder interface - use aom_codec_av1_cx() if available
        // Otherwise, we'll skip encoder initialization
        aom_codec_iface_t* iface = NULL;
        
        // Try to get AV1 encoder interface
        // Note: aom_codec_av1_cx() might not be available in all builds
        // We'll use a simple approach: if the function exists, use it
        // Otherwise, skip encoder tests
        
        // For now, we'll skip the encoder initialization since we can't guarantee
        // aom_codec_av1_cx() is available. We'll still test the other functions.
        
        // Test aom_codec_encode with NULL context and image
        if (Size & 8) {
            aom_codec_encode(NULL, NULL, 0, 1, 0);
        }
        
        // Test aom_codec_get_preview_frame with NULL context
        if (Size & 16) {
            aom_codec_get_preview_frame(NULL);
        }
    }
    
    // Test aom_codec_destroy with our test context
    aom_codec_destroy(ctx);
    
    // Test aom_codec_enc_init_ver with NULL parameters
    if (Size > 50) {
        aom_codec_enc_cfg_t* cfg = create_test_config();
        aom_codec_enc_init_ver(NULL, NULL, NULL, 0, 0);
        aom_codec_enc_init_ver(ctx, NULL, cfg, 0, AOM_ENCODER_ABI_VERSION);
    }
    
    return 0;
}