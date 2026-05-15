// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"

static void init_random_image(aom_image_t *img, const uint8_t *data, size_t size) {
    if (size < 100) return;
    
    img->fmt = (aom_img_fmt_t)(data[0] % 16);
    img->cp = (aom_color_primaries_t)(data[1] % 24);
    img->tc = (aom_transfer_characteristics_t)(data[2] % 20);
    img->mc = (aom_matrix_coefficients_t)(data[3] % 16);
    img->monochrome = data[4] & 1;
    img->csp = (aom_chroma_sample_position_t)(data[5] % 4);
    img->range = (aom_color_range_t)(data[6] % 2);
    
    img->w = 16 + (data[7] % 64);
    img->h = 16 + (data[8] % 64);
    img->bit_depth = 8 + ((data[9] % 3) * 2);
    img->d_w = img->w;
    img->d_h = img->h;
    img->r_w = img->w;
    img->r_h = img->h;
    img->x_chroma_shift = (data[10] & 1) + 1;
    img->y_chroma_shift = (data[11] & 1) + 1;
    
    size_t offset = 12;
    for (int i = 0; i < 3; i++) {
        if (offset < size) {
            img->stride[i] = 16 + (data[offset++] % 64);
        } else {
            img->stride[i] = img->w;
        }
    }
    
    img->sz = img->w * img->h * 3;
    img->bps = img->bit_depth;
    img->temporal_id = data[15] % 4;
    img->spatial_id = data[16] % 4;
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;
}

static void init_random_config(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < 100) return;
    
    size_t offset = 0;
    cfg->g_usage = data[offset++] % 10;
    cfg->g_threads = 1 + (data[offset++] % 8);
    cfg->g_profile = data[offset++] % 4;
    cfg->g_w = 16 + (data[offset++] % 128);
    cfg->g_h = 16 + (data[offset++] % 128);
    cfg->g_limit = data[offset++] % 100;
    cfg->g_forced_max_frame_width = data[offset++] % 256;
    cfg->g_forced_max_frame_height = data[offset++] % 256;
    cfg->g_bit_depth = (aom_bit_depth_t)(8 + ((data[offset++] % 3) * 2));
    cfg->g_input_bit_depth = cfg->g_bit_depth;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = data[offset++] & 1;
    cfg->g_pass = (enum aom_enc_pass)(data[offset++] % 4);
    cfg->g_lag_in_frames = data[offset++] % 10;
    
    cfg->rc_dropframe_thresh = data[offset++] % 100;
    cfg->rc_resize_mode = data[offset++] % 4;
    cfg->rc_resize_denominator = 8 + (data[offset++] % 9);
    cfg->rc_resize_kf_denominator = 8 + (data[offset++] % 9);
    cfg->rc_superres_mode = (aom_superres_mode)(data[offset++] % 4);
    cfg->rc_superres_denominator = 8 + (data[offset++] % 9);
    cfg->rc_superres_kf_denominator = 8 + (data[offset++] % 9);
    cfg->rc_superres_qthresh = 1 + (data[offset++] % 63);
    cfg->rc_superres_kf_qthresh = 1 + (data[offset++] % 63);
    cfg->rc_end_usage = (enum aom_rc_mode)(data[offset++] % 4);
    cfg->rc_target_bitrate = 100 + (data[offset++] % 9900);
    
    cfg->rc_min_quantizer = data[offset++] % 64;
    cfg->rc_max_quantizer = 10 + (data[offset++] % 54);
    cfg->rc_undershoot_pct = data[offset++] % 101;
    cfg->rc_overshoot_pct = data[offset++] % 101;
    cfg->rc_buf_sz = data[offset++] % 10000;
    cfg->rc_buf_initial_sz = data[offset++] % 5000;
    cfg->rc_buf_optimal_sz = data[offset++] % 5000;
    cfg->rc_2pass_vbr_bias_pct = data[offset++] % 101;
    cfg->rc_2pass_vbr_minsection_pct = data[offset++] % 101;
    cfg->rc_2pass_vbr_maxsection_pct = 50 + (data[offset++] % 51);
    
    cfg->fwd_kf_enabled = data[offset++] & 1;
    cfg->kf_mode = (enum aom_kf_mode)(data[offset++] % 3);
    cfg->kf_min_dist = data[offset++] % 100;
    cfg->kf_max_dist = 10 + (data[offset++] % 100);
    cfg->sframe_dist = data[offset++] % 100;
    cfg->sframe_mode = 1 + (data[offset++] % 2);
    cfg->large_scale_tile = data[offset++] & 1;
    cfg->monochrome = data[offset++] & 1;
    cfg->full_still_picture_hdr = data[offset++] & 1;
    cfg->save_as_annexb = data[offset++] & 1;
    
    cfg->tile_width_count = data[offset++] % 4;
    cfg->tile_height_count = data[offset++] % 4;
    for (int i = 0; i < cfg->tile_width_count && i < 64; i++) {
        cfg->tile_widths[i] = 8 + (data[offset++] % 56);
    }
    for (int i = 0; i < cfg->tile_height_count && i < 64; i++) {
        cfg->tile_heights[i] = 8 + (data[offset++] % 56);
    }
    
    cfg->use_fixed_qp_offsets = data[offset++] & 1;
    for (int i = 0; i < 5; i++) {
        cfg->fixed_qp_offsets[i] = data[offset++] % 64;
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *data, size_t size) {
    if (size < 10) return 0;
    
    // Test aom_codec_destroy with various contexts
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Test with NULL context
    aom_codec_destroy(NULL);
    
    // Test with uninitialized context
    aom_codec_destroy(&ctx);
    
    // Get encoder interface using the proper method
    extern aom_codec_iface_t aom_codec_av1_cx_algo;
    aom_codec_iface_t *iface = &aom_codec_av1_cx_algo;
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Modify config with fuzzer data
    init_random_config(&cfg, data, size);
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
        // Test aom_codec_set_option
        if (size > 20) {
            char key[32], value[32];
            size_t key_len = (data[0] % 20) + 1;
            size_t val_len = (data[1] % 20) + 1;
            
            if (key_len > 31) key_len = 31;
            if (val_len > 31) val_len = 31;
            
            memcpy(key, data + 2, key_len);
            memcpy(value, data + 2 + key_len, val_len);
            key[key_len] = 0;
            value[val_len] = 0;
            
            aom_codec_set_option(&ctx, key, value);
        }
        
        // Test aom_codec_get_preview_frame
        const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
        (void)preview;
        
        // Test aom_codec_control with various control IDs
        if (size > 50) {
            int ctrl_id = *(int*)(data + 20);
            int ctrl_val = *(int*)(data + 24);
            
            // Test with different control IDs (using generic values since we don't have specific headers)
            aom_codec_control(&ctx, ctrl_id, ctrl_val);
            aom_codec_control(&ctx, 1, data[28] % 10);      // Generic control ID 1
            aom_codec_control(&ctx, 2, data[29] & 1);       // Generic control ID 2
            aom_codec_control(&ctx, 3, data[30] % 5);       // Generic control ID 3
            aom_codec_control(&ctx, 4, data[31] % 7);       // Generic control ID 4
        }
        
        // Test aom_codec_encode
        aom_image_t img;
        memset(&img, 0, sizeof(img));
        
        if (size > 100) {
            init_random_image(&img, data + 40, size - 40);
            
            // Encode with image
            aom_codec_encode(&ctx, &img, 0, 1, 0);
            
            // Encode without image (flush)
            aom_codec_encode(&ctx, NULL, 0, 0, 0);
            
            // Encode with different PTS
            aom_codec_encode(&ctx, &img, 1, 1, 1);  // Using flag value 1
            aom_codec_encode(&ctx, &img, 2, 1, 2);  // Using flag value 2
        }
        
        // Clean up
        aom_codec_destroy(&ctx);
    }
    
    // Test with different initialization flags
    if (size > 60) {
        aom_codec_ctx_t ctx2;
        aom_codec_flags_t flags = 0;
        if (data[50] & 1) {
            flags |= 0x8000;  // Use a generic flag value
        }
        
        if (aom_codec_enc_init_ver(&ctx2, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            aom_codec_destroy(&ctx2);
        }
    }
    
    return 0;
}