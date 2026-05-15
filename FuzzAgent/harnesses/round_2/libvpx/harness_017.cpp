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

namespace harness_017 {
/*
 * Fuzzing harness for libvpx targeting advanced VP9 encoder configurations
 * including multi-layer encoding, two-pass rate control, and edge case 
 * configurations to exercise blocked validation logic in validate_config
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_017.cpp based on coverage guidance targeting
 *       advanced VP9 encoder configurations including multi-layer encoding,
 *       two-pass rate control, and edge case configurations to exercise
 *       blocked validation logic. Focus on testing complex encoder settings
 *       to improve function coverage (currently 51.43%) and branch coverage (36.14%).
 * 
 * Specific blocked branches targeted:
 * - validate_config: 293 blocked branches (validation logic for encoder configuration)
 * - vp9_pick_inter_mode: 375 blocked branches (inter-mode selection)
 * Total: 668+ blocked branches
 * 
 * APIs to target as specified in guidance:
 * - vpx_codec_enc_config_default (Required Helper: Get default config)
 * - vpx_codec_enc_init_ver (Target: Test initialization with various configs)
 * - vpx_codec_enc_config_set (Target: Modify config after init)
 * - vpx_codec_encode (Required Helper: Actual encoding)
 * - vpx_codec_get_cx_data (Required Helper: Get encoded data)
 * - vpx_codec_destroy (Required Helper: Cleanup)
 * 
 * Semantic differentiation from previous harnesses:
 * 1. Focuses specifically on two-pass encoding (VPX_RC_LAST_PASS) with proper stats buffer
 * 2. Tests advanced multi-layer configurations with edge cases
 * 3. Systematically tests various rc_end_usage modes with invalid combinations
 * 4. Tests kf_mode with VPX_KF_AUTO and specific kf_min_dist/kf_max_dist relationships
 * 5. Explicitly targets validation error paths in validate_config function
 * 
 * Invocation sequence from guidance:
 * 1. Call vpx_codec_enc_config_default to get default VP9 encoder configuration.
 * 2. Modify configuration parameters to test edge cases:
 *    a. Set g_pass to VPX_RC_LAST_PASS (two-pass encoding)
 *    b. Configure spatial layers (ss_number_layers > 1)
 *    c. Configure temporal layers (ts_number_layers > 1)  
 *    d. Test various rc_end_usage modes (VPX_VBR, VPX_CBR, VPX_CQ, VPX_Q)
 *    e. Set kf_mode to VPX_KF_AUTO with specific kf_min_dist/kf_max_dist
 *    f. Test invalid combinations that should trigger validation errors
 * 3. Call vpx_codec_enc_init_ver with modified configuration (some should fail).
 * 4. For successful initializations, call vpx_codec_encode with test frames.
 * 5. Call vpx_codec_get_cx_data to retrieve encoded packets.
 * 6. Call vpx_codec_destroy to clean up.
 * 
 * Reasoning from guidance: The top branch blockers are vp9_cx_iface.c:validate_config 
 * (293 blocked branches) and vp9_pick_inter_mode (375 blocked branches). These represent 
 * encoder configuration validation and inter-mode selection logic that is not being 
 * exercised. Current harnesses likely test only basic configurations. Testing advanced 
 * features like multi-layer encoding, two-pass rate control, and various encoder 
 * settings will trigger these blocked code paths. The 1284 hits on validate_config 
 * show it's being called, but many validation branches remain untaken due to limited 
 * configuration diversity.
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for complex configuration, stats data, and frame data
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
    
    // Step 2: Configure basic encoder parameters from fuzzer input
    // Use reasonable ranges for video dimensions
    cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 1920) & ~1; // Ensure even width
    cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 1080) & ~1; // Ensure even height
    
    // Set target bitrate and frame rate
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 5000); // kbps
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60); // fps
    
    // Step 3: Test keyframe configuration with VPX_KF_AUTO as per guidance
    cfg.kf_mode = VPX_KF_AUTO;
    
    // Test various kf_min_dist/kf_max_dist relationships including edge cases
    unsigned int kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    unsigned int kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
    
    // Apply different relationship patterns to test validation logic
    uint8_t kf_relationship = fdp.ConsumeIntegral<uint8_t>() % 5;
    switch (kf_relationship) {
        case 0: // Valid: min < max
            if (kf_min_dist >= kf_max_dist && kf_max_dist > 0) {
                kf_min_dist = kf_max_dist - 1;
            }
            break;
        case 1: // Edge case: min > max (should trigger validation error)
            if (kf_min_dist <= kf_max_dist && kf_min_dist > 0) {
                kf_min_dist = kf_max_dist + 1;
            }
            break;
        case 2: // Edge case: min == max
            kf_min_dist = kf_max_dist;
            break;
        case 3: // Edge case: zero values
            kf_min_dist = 0;
            kf_max_dist = 0;
            break;
        case 4: // Edge case: max = 0 but min > 0
            kf_max_dist = 0;
            if (kf_min_dist == 0) {
                kf_min_dist = 1;
            }
            break;
    }
    
    cfg.kf_min_dist = kf_min_dist;
    cfg.kf_max_dist = kf_max_dist;
    
    // Step 4: Configure spatial and temporal layers as per guidance
    // Test multi-layer configurations (ss_number_layers > 1, ts_number_layers > 1)
    cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
    cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
    
    // Set target bitrates for spatial layers if configured
    if (cfg.ss_number_layers > 1) {
        for (unsigned int i = 0; i < cfg.ss_number_layers; i++) {
            cfg.ss_target_bitrate[i] = fdp.ConsumeIntegralInRange<unsigned int>(100, 2000);
        }
    }
    
    // Configure temporal layers with rate decimators
    if (cfg.ts_number_layers > 1) {
        for (unsigned int i = 0; i < cfg.ts_number_layers; i++) {
            cfg.ts_target_bitrate[i] = fdp.ConsumeIntegralInRange<unsigned int>(100, 2000);
            // Use different rate decimator patterns
            uint8_t decimator_pattern = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (decimator_pattern) {
                case 0:
                    cfg.ts_rate_decimator[i] = 1 << (i + 1); // Powers of 2: 2, 4, 8, 16
                    break;
                case 1:
                    cfg.ts_rate_decimator[i] = (i + 1) * 2; // Linear: 2, 4, 6, 8
                    break;
                case 2:
                    cfg.ts_rate_decimator[i] = 1; // All layers at same rate
                    break;
            }
        }
        cfg.ts_periodicity = cfg.ts_number_layers;
    }
    
    // Step 5: Set rate control mode as per guidance (VPX_VBR, VPX_CBR, VPX_CQ, VPX_Q)
    uint8_t rc_mode = fdp.ConsumeIntegral<uint8_t>() % 5; // 5th option for invalid mode testing
    switch (rc_mode) {
        case 0:
            cfg.rc_end_usage = VPX_VBR; // Variable Bit Rate
            break;
        case 1:
            cfg.rc_end_usage = VPX_CBR; // Constant Bit Rate
            // Test invalid CBR parameters
            if (fdp.ConsumeBool()) {
                cfg.rc_target_bitrate = 0; // Invalid: zero target bitrate
            }
            break;
        case 2:
            cfg.rc_end_usage = VPX_CQ;  // Constrained Quality
            cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
            cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
            // Test invalid quantizer range
            if (fdp.ConsumeBool()) {
                cfg.rc_max_quantizer = cfg.rc_min_quantizer - 1; // Invalid: max < min
            }
            break;
        case 3:
            cfg.rc_end_usage = VPX_Q;   // Constant Quality
            break;
        case 4:
            // Test invalid rc_end_usage value (should trigger validation)
            cfg.rc_end_usage = static_cast<vpx_rc_mode>(999);
            break;
    }
    
    // Step 6: Set encoding pass mode - CRITICAL DIFFERENTIATOR: VPX_RC_LAST_PASS
    uint8_t pass_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (pass_mode) {
        case 0:
            cfg.g_pass = VPX_RC_ONE_PASS;
            break;
        case 1:
            cfg.g_pass = VPX_RC_FIRST_PASS;
            // For first pass, we need to set up stats collection
            cfg.rc_twopass_stats_in.buf = nullptr;
            cfg.rc_twopass_stats_in.sz = 0;
            break;
        case 2: {
            cfg.g_pass = VPX_RC_LAST_PASS; // Primary focus as per guidance
            // For last pass, we need to provide stats from first pass
            // Create dummy stats buffer for testing
            size_t stats_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
            if (stats_size > 0 && fdp.remaining_bytes() >= stats_size) {
                std::vector<uint8_t> dummy_stats = fdp.ConsumeBytes<uint8_t>(stats_size);
                cfg.rc_twopass_stats_in.buf = dummy_stats.data();
                cfg.rc_twopass_stats_in.sz = dummy_stats.size();
            } else {
                cfg.rc_twopass_stats_in.buf = nullptr;
                cfg.rc_twopass_stats_in.sz = 0;
            }
            break;
        }
        case 3:
            // Test invalid g_pass value
            cfg.g_pass = static_cast<vpx_enc_pass>(999);
            break;
    }
    // Step 7: Set g_lag_in_frames with edge cases for validation
    uint8_t lag_config = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (lag_config) {
        case 0: // Normal case
            cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
            break;
        case 1: // Edge case: lag > kf_max_dist (should trigger validation)
            cfg.g_lag_in_frames = kf_max_dist + fdp.ConsumeIntegralInRange<unsigned int>(1, 10);
            break;
        case 2: // Edge case: lag = 0 (special case)
            cfg.g_lag_in_frames = 0;
            break;
        case 3: // Edge case: very large lag value
            cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
            break;
    }
    
    // Step 8: Set cpu_used value
    cfg.g_usage = 0; // Required to be zero
    int cpu_used = fdp.ConsumeIntegralInRange<int>(-9, 9);
    
    // Step 9: Allocate image buffer for input frames
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        return 0;
    }
    
    // Fill image buffer with fuzzer data
    size_t y_size = cfg.g_w * cfg.g_h;
    size_t uv_size = ((cfg.g_w + 1) / 2) * ((cfg.g_h + 1) / 2);
    
    if (fdp.remaining_bytes() < y_size + 2 * uv_size) {
        vpx_img_free(img);
        return 0;
    }
    
    // Consume YUV data for the image
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_size);
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_size);
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_size);
    
    memcpy(img->planes[VPX_PLANE_Y], y_data.data(), y_size);
    memcpy(img->planes[VPX_PLANE_U], u_data.data(), uv_size);
    memcpy(img->planes[VPX_PLANE_V], v_data.data(), uv_size);
    
    // Step 10: Initialize encoder with base configuration
    vpx_codec_flags_t flags = 0;
    vpx_codec_err_t init_result = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    
    // We expect some initializations to fail due to invalid configurations
    // This is intentional to test validation error paths
    if (init_result != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0; // Invalid configuration, test complete
    }
    
    // Step 11: Set cpu_used parameter
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    
    // Step 12: Try to apply configuration changes (triggers validate_config)
    // This is a critical test for validation logic
    vpx_codec_err_t config_result = vpx_codec_enc_config_set(&codec, &cfg);
    // Don't fail if config_set fails - invalid configurations are what we want to test
    
    // Step 13: Encode multiple frames to exercise mode selection
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 10);
    for (int i = 0; i < num_frames; i++) {
        vpx_enc_frame_flags_t frame_flags = 0;
        
        // Set frame flags based on input
        if (fdp.ConsumeBool()) {
            frame_flags |= VPX_EFLAG_FORCE_KF;
        }
        
        // Encode the frame
        vpx_codec_err_t encode_result = vpx_codec_encode(&codec, img, i, 1, frame_flags, VPX_DL_GOOD_QUALITY);
        
        // Retrieve encoded packets
        if (encode_result == VPX_CODEC_OK) {
            vpx_codec_iter_t iter = nullptr;
            const vpx_codec_cx_pkt_t* pkt = nullptr;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                // Process packet if needed
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Frame packet received
                }
            }
        }
        
        // Break if we're running out of input data
        if (fdp.remaining_bytes() < 100) {
            break;
        }
    }
    
    // Step 14: Flush encoder
    vpx_codec_encode(&codec, nullptr, -1, 1, 0, VPX_DL_GOOD_QUALITY);
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process any remaining packets
    }
    
    // Step 15: Cleanup
    vpx_codec_destroy(&codec);
    vpx_img_free(img);
    
    return 0;
}

} // namespace harness_017
