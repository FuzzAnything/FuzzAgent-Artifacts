#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"

namespace harness_021 {
/*
 * Fuzzing harness for libvpx targeting VP9 encoder deep coverage through diverse configuration testing
 * Focus: Deep encoder functions vp9_pick_inter_mode (428 blocked branches), vp9_rd_pick_inter_mode_sb (311 blocked),
 *        and validate_config (305 blocked branches) through comprehensive encoder configuration diversity.
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_021.cpp based on coverage guidance targeting deep encoder functions 
 *       using vpx_codec_control_ API to test diverse encoder configurations and unlock blocked validation paths.
 * 
 * APIs to target as specified in guidance:
 * - vpx_codec_control_ (Primary target: 6/18 undiscovered branches)
 * - vpx_codec_enc_config_default (config initialization)
 * - vpx_codec_enc_config_set (config application)
 * - vpx_codec_enc_init_ver (encoder initialization with varied configurations)
 * - vpx_codec_encode (frame encoding)
 * - vpx_codec_get_cx_data (output retrieval)
 * - vpx_codec_destroy (cleanup)
 * - vpx_img_alloc (image allocation)
 * - vpx_img_free (image cleanup)
 * 
 * Specific blocked branches targeted:
 * - vp9_pick_inter_mode: 428 blocked branches (out of 764)
 * - vp9_rd_pick_inter_mode_sb: 311 blocked branches (out of 462)
 * - validate_config: 305 blocked branches (out of 720) - many validation checks with 0 hits
 * 
 * Configuration diversity strategy:
 * 1. Different g_pass values (VPX_RC_ONE_PASS, VPX_RC_FIRST_PASS, VPX_RC_LAST_PASS)
 * 2. Different rc_end_usage modes (VPX_VBR, VPX_CBR, VPX_CQ, VPX_Q)
 * 3. Different kf_mode settings
 * 4. Multi-layer SVC configurations (spatial/temporal layers)
 * 5. Screen content encoding (VP9E_CONTENT_SCREEN)
 * 6. Alternative reference frame configurations
 * 7. Tile-based encoding (tile_rows, tile_columns)
 * 8. Various quality/rate control parameters
 * 
 * Semantic differentiation from previous harnesses:
 * 1. Specifically focuses on configuration diversity to trigger validate_config function branches
 * 2. Tests all g_pass modes including two-pass encoding (VPX_RC_LAST_PASS) which is currently unexplored
 * 3. Tests Constant Quality mode (VPX_Q) to reach Q-specific branches
 * 4. Tests diverse vpx_codec_control_ calls with different control IDs
 * 5. Focuses on the exact coverage gap: validation logic for edge cases in configuration parameters
 * 
 * Invocation sequence from guidance:
 * 1. Initialize image buffers using vpx_img_alloc
 * 2. Get default configuration using vpx_codec_enc_config_default
 * 3. Modify diverse configuration parameters using vpx_codec_control_ and direct struct modification
 * 4. Apply configurations using vpx_codec_enc_config_set (triggers validate_config)
 * 5. Initialize encoder with vpx_codec_enc_init_ver with varied configurations
 * 6. Encode frames using vpx_codec_encode with test frames
 * 7. Retrieve output using vpx_codec_get_cx_data in loop
 * 8. Exercise vpx_codec_control_ with different control IDs during encoding
 * 9. Cleanup using vpx_codec_destroy and vpx_img_free
 * 
 * Key configurations to test:
 * - Two-pass encoding (g_pass = VPX_RC_LAST_PASS) - currently unexplored
 * - Constant Quality mode (rc_end_usage = VPX_Q) - to reach Q-specific branches
 * - Multi-layer SVC (spatial and temporal layers) - complex validation paths
 * - Screen content encoding (content = VP9E_CONTENT_SCREEN)
 * - Alternative reference frame configurations
 * - Tile-based encoding (tile_rows, tile_columns)
 * - Various bitrate and quality configurations
 * 
 * Reasoning from guidance: The existing harnesses have saturated basic encoding scenarios but lack diversity 
 * in encoder configurations. The validate_config function acts as a gateway to different encoder modes - by 
 * testing diverse configurations through vpx_codec_control_ and config setters, we can reach the deeply 
 * blocked encoder logic that handles specialized encoding modes and optimizations. This represents the 
 * largest remaining coverage gap in VP9 encoder configuration validation and deep encoder function paths.
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for diverse configuration parameters,
    // control IDs, and image data for encoding
    const size_t MIN_SIZE = sizeof(uint8_t) * 2048 + sizeof(int) * 128 + sizeof(unsigned int) * 256;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize encoder interface for VP9
    vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    // Step 1: Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 2: Consume fuzzed data for configuration parameters
    // Basic configuration parameters
    cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 1920);
    cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 1080);
    cfg.g_timebase.num = fdp.ConsumeIntegralInRange<int>(1, 100);
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<int>(1, 1000);
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(0, 8);
    cfg.g_error_resilient = static_cast<vpx_codec_er_flags_t>(fdp.ConsumeIntegral<uint32_t>());
    
    // Step 3: Test different g_pass modes (critical for coverage)
    uint8_t pass_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (pass_mode) {
        case 0:
            cfg.g_pass = VPX_RC_ONE_PASS;
            break;
        case 1:
            cfg.g_pass = VPX_RC_FIRST_PASS;
            break;
        case 2:
            cfg.g_pass = VPX_RC_LAST_PASS;  // Two-pass encoding - currently unexplored
            break;
    }
    
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
    
    // Step 4: Test different rc_end_usage modes
    uint8_t rc_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (rc_mode) {
        case 0:
            cfg.rc_end_usage = VPX_VBR;
            break;
        case 1:
            cfg.rc_end_usage = VPX_CBR;
            break;
        case 2:
            cfg.rc_end_usage = VPX_CQ;
            break;
        case 3:
            cfg.rc_end_usage = VPX_Q;  // Constant Quality mode - to reach Q-specific branches
            break;
    }
    
    // Rate control parameters
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
    cfg.rc_undershoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    cfg.rc_overshoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    cfg.rc_buf_sz = fdp.ConsumeIntegralInRange<unsigned int>(1000, 10000);
    cfg.rc_buf_initial_sz = fdp.ConsumeIntegralInRange<unsigned int>(500, cfg.rc_buf_sz);
    cfg.rc_buf_optimal_sz = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_buf_initial_sz, cfg.rc_buf_sz);
    
    // Keyframe parameters
    cfg.kf_mode = static_cast<vpx_kf_mode>(fdp.ConsumeIntegral<uint8_t>() % 3);
    cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
    cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(cfg.kf_min_dist, 1000);
    
    // Step 5: Allocate test image
    vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        return 0;
    }
    
    // Fill image with fuzzed data
    size_t img_size = cfg.g_w * cfg.g_h * 3 / 2;  // YUV420 size
    if (fdp.remaining_bytes() < img_size) {
        vpx_img_free(img);
        return 0;
    }
    
    std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(img_size);
    if (img_data.size() >= img_size) {
        memcpy(img->planes[0], img_data.data(), cfg.g_w * cfg.g_h);
        memcpy(img->planes[1], img_data.data() + cfg.g_w * cfg.g_h, cfg.g_w * cfg.g_h / 4);
        memcpy(img->planes[2], img_data.data() + cfg.g_w * cfg.g_h * 5 / 4, cfg.g_w * cfg.g_h / 4);
    }
    
    // Step 6: Apply configuration (triggers validate_config)
    if (vpx_codec_enc_config_set(&codec, &cfg) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // Step 7: Initialize encoder with configuration
    vpx_codec_flags_t flags = 0;
    if (fdp.ConsumeBool()) {
        flags |= VPX_CODEC_USE_HIGHBITDEPTH;
    }
    
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // Step 8: Exercise vpx_codec_control_ with diverse control IDs (primary target)
    // This is critical for reaching the 6/18 undiscovered branches in vpx_codec_control_
    
    // Test screen content encoding
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP9E_SET_TUNE_CONTENT, VP9E_CONTENT_SCREEN);
    }
    
    // Test tile-based encoding
    if (fdp.ConsumeBool()) {
        unsigned int tile_columns = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
        unsigned int tile_rows = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
        vpx_codec_control(&codec, VP9E_SET_TILE_ROWS, tile_rows);
    }
    
    // Test frame parallel decoding
    if (fdp.ConsumeBool()) {
        int frame_parallel = fdp.ConsumeBool() ? 1 : 0;
        vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel);
    }
    
    // Test AQ mode
    if (fdp.ConsumeBool()) {
        int aq_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
        vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode);
    }
    
    // Test color space
    if (fdp.ConsumeBool()) {
        int color_space = fdp.ConsumeIntegralInRange<int>(0, 7);
        vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space);
    }
    
    // Test rendering size
    if (fdp.ConsumeBool()) {
        int render_width = fdp.ConsumeIntegralInRange<int>(16, cfg.g_w);
        int render_height = fdp.ConsumeIntegralInRange<int>(16, cfg.g_h);
        vpx_codec_control(&codec, VP9E_SET_RENDER_SIZE, &render_width);
    }
    
    // Test SVC multi-layer configurations if enough bytes remain
    if (fdp.remaining_bytes() > 100) {
        unsigned int svc_params[VPX_MAX_LAYERS] = {0};
        int svc_layers = fdp.ConsumeIntegralInRange<int>(1, 4);
        
        for (int i = 0; i < svc_layers && i < VPX_MAX_LAYERS; i++) {
            svc_params[i] = fdp.ConsumeIntegral<uint32_t>();
        }
        
        vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, svc_params);
        
        // Set layer ID
        vpx_svc_layer_id_t layer_id;
        layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<int>(0, svc_layers - 1);
        layer_id.temporal_layer_id = fdp.ConsumeIntegralInRange<int>(0, 2);
        vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
    }
    
    // Test alternate reference frame configuration
    if (fdp.ConsumeBool()) {
        vpx_svc_ref_frame_config_t ref_config;
        ref_config.lst_fb_idx[0] = fdp.ConsumeIntegralInRange<int>(0, 7);
        ref_config.gld_fb_idx[0] = fdp.ConsumeIntegralInRange<int>(0, 7);
        ref_config.alt_fb_idx[0] = fdp.ConsumeIntegralInRange<int>(0, 7);
        vpx_codec_control(&codec, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
    }
    
    // Step 9: Encode frames
    vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Encode additional frames if we have more image data
    if (fdp.remaining_bytes() > img_size) {
        std::vector<uint8_t> img_data2 = fdp.ConsumeBytes<uint8_t>(img_size);
        if (img_data2.size() >= img_size) {
            memcpy(img->planes[0], img_data2.data(), cfg.g_w * cfg.g_h);
            memcpy(img->planes[1], img_data2.data() + cfg.g_w * cfg.g_h, cfg.g_w * cfg.g_h / 4);
            memcpy(img->planes[2], img_data2.data() + cfg.g_w * cfg.g_h * 5 / 4, cfg.g_w * cfg.g_h / 4);
            // Test different frame flags
            vpx_enc_frame_flags_t frame_flags = 0;
            if (fdp.ConsumeBool()) {
                frame_flags |= VPX_EFLAG_FORCE_KF;
            }
            if (fdp.ConsumeBool()) {
                frame_flags |= VP8_EFLAG_NO_REF_LAST;
            }
            if (fdp.ConsumeBool()) {
                frame_flags |= VP8_EFLAG_NO_REF_GF;
            }
            if (fdp.ConsumeBool()) {
                frame_flags |= VP8_EFLAG_NO_REF_ARF;
            }
            
            vpx_codec_encode(&codec, img, 1, 1, frame_flags, VPX_DL_REALTIME);
        }
    }
    
    // Step 10: Retrieve encoded data
    const vpx_codec_cx_pkt_t* pkt = NULL;
    vpx_codec_iter_t iter = NULL;
    
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packets if needed
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Frame data available
        }
    }
    
    // Step 11: Test more control APIs after encoding
    if (fdp.ConsumeBool()) {
        int cq_level = fdp.ConsumeIntegralInRange<int>(0, 63);
        vpx_codec_control(&codec, VP8E_SET_CQ_LEVEL, cq_level);
    }
    
    if (fdp.ConsumeBool()) {
        int arnr_max_frames = fdp.ConsumeIntegralInRange<int>(0, 15);
        int arnr_strength = fdp.ConsumeIntegralInRange<int>(0, 6);
        vpx_codec_control(&codec, VP8E_SET_ARNR_MAXFRAMES, arnr_max_frames);
        vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    }
    
    if (fdp.ConsumeBool()) {
        int noise_sensitivity = fdp.ConsumeIntegralInRange<int>(0, 6);
        vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    }
    
    // Step 12: Cleanup
    vpx_codec_destroy(&codec);
    vpx_img_free(img);
    
    return 0;
}

} // namespace harness_021
