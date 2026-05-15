// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

static void dummy_cleanup(void) {
    // Clean up any global state if needed
}

static aom_codec_ctx_t* create_dummy_codec_ctx(void) {
    static aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.name = "dummy";
    return &ctx;
}

static aom_image_t* create_dummy_image(void) {
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
    img.sz = y_size + 2 * uv_size;
    img.img_data = (unsigned char*)malloc(img.sz);
    if (!img.img_data) return NULL;
    
    img.img_data_owner = 1;
    img.self_allocd = 1;
    
    img.planes[0] = img.img_data;
    img.planes[1] = img.img_data + y_size;
    img.planes[2] = img.img_data + y_size + uv_size;
    
    img.stride[0] = img.w;
    img.stride[1] = img.w >> img.x_chroma_shift;
    img.stride[2] = img.w >> img.x_chroma_shift;
    
    img.bps = 12;
    img.temporal_id = 0;
    img.spatial_id = 0;
    img.user_priv = NULL;
    img.metadata = NULL;
    img.fb_priv = NULL;
    
    return &img;
}

static void free_dummy_image(aom_image_t* img) {
    if (img && img->img_data_owner && img->img_data) {
        free(img->img_data);
        img->img_data = NULL;
    }
}

static aom_codec_enc_cfg_t* create_dummy_enc_cfg(void) {
    static aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
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
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 56;
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

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Initialize dummy contexts
    aom_codec_ctx_t* ctx = create_dummy_codec_ctx();
    aom_image_t* img = create_dummy_image();
    aom_codec_enc_cfg_t* cfg = create_dummy_enc_cfg();
    
    if (!img) {
        dummy_cleanup();
        return 0;
    }
    
    // Fill image data with fuzzer input (safely)
    if (Size > data_offset) {
        size_t copy_size = img->sz;
        if (copy_size > Size - data_offset) {
            copy_size = Size - data_offset;
        }
        memcpy(img->img_data, Data + data_offset, copy_size);
        data_offset += copy_size;
    }
    
    // Test aom_codec_enc_init_ver
    if (selector & 0x01) {
        aom_codec_err_t init_result = aom_codec_enc_init_ver(
            ctx, aom_codec_av1_cx(), cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        // If initialization succeeded, test other functions
        if (init_result == AOM_CODEC_OK) {
            // Test aom_codec_set_option
            if (selector & 0x02) {
                const char* name = "cpu-used";
                const char* value = "1";
                if (Size > data_offset + 2) {
                    // Use fuzzer data for option values if available
                    name = (const char*)(Data + data_offset);
                    value = (const char*)(Data + data_offset + 1);
                }
                aom_codec_set_option(ctx, name, value);
            }
            
            // Test aom_codec_control
            if (selector & 0x04) {
                int ctrl_id = 7; // AOME_SET_CPUUSED
                int value = 1;
                if (Size > data_offset + 8) {
                    ctrl_id = *(const int*)(Data + data_offset);
                    value = *(const int*)(Data + data_offset + 4);
                }
                aom_codec_control(ctx, ctrl_id, value);
            }
            
            // Test aom_codec_encode
            if (selector & 0x08) {
                aom_codec_pts_t pts = 0;
                unsigned long duration = 1;
                aom_enc_frame_flags_t flags = 0;
                
                if (Size > data_offset + 12) {
                    pts = *(const aom_codec_pts_t*)(Data + data_offset);
                    duration = *(const unsigned long*)(Data + data_offset + 8);
                    if (duration > 1000) duration = 1000; // Limit duration
                }
                
                // Encode with image
                aom_codec_encode(ctx, img, pts, duration, flags);
                
                // Encode with NULL to flush
                aom_codec_encode(ctx, NULL, 0, 0, 0);
            }
            
            // Test aom_codec_get_preview_frame
            if (selector & 0x10) {
                const aom_image_t* preview = aom_codec_get_preview_frame(ctx);
                (void)preview; // Preview may be NULL, that's OK
            }
            
            // Test aom_codec_destroy
            if (selector & 0x20) {
                aom_codec_destroy(ctx);
                // After destroy, ctx should not be used
                // Reset for potential reuse
                memset(ctx, 0, sizeof(aom_codec_ctx_t));
                ctx->name = "dummy";
            }
        }
    }
    
    // Clean up
    free_dummy_image(img);
    dummy_cleanup();
    
    return 0;
}