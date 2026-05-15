#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"

namespace harness_014 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation error paths
 * targeting 337 blocked branches in validate_config function (single largest coverage blocker).
 * 
 * Target APIs: vpx_codec_enc_config_default → vpx_codec_enc_init_ver/vpx_codec_enc_init_multi_ver → 
 *              vpx_codec_enc_config_set → vpx_codec_control_ → vpx_codec_destroy
 * 
 * Focus: Error path validation and edge case configuration testing specifically targeting:
 * 1. Invalid dimension ranges (g_w, g_h outside 1-65536)
 * 2. Profile validation (g_profile > 3)
 * 3. Quantizer range checks (rc_min_quantizer > rc_max_quantizer)
 * 4. Lag-in-frames constraints with max_gf_interval
 * 5. Multi-layer configuration validation (ss_number_layers * ts_number_layers > VPX_MAX_LAYERS)
 * 6. Rate control parameter validation (undershoot/overshoot percentages)
 * 7. Keyframe mode and distance validation
 * 8. Resize configuration validation when rc_resize_allowed=1
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_013.cpp: Complete encoder lifecycle testing with normal operation
 * - harness_014.cpp: Focused on error path validation, testing invalid/edge case configurations
 * 
 * Strategy: Generate intentionally invalid configurations to trigger validation failures,
 * test error handling paths in validate_config function, and ensure proper error returns.
 */



// Define constants from libvpx internal headers
#define MAX_LAG_BUFFERS 25        // From vp9/encoder/vp9_lookahead.h
#define VPX_MAX_LAYERS 12         // From vpx/vpx_encoder.h
#define VPX_SS_MAX_LAYERS 5       // From vpx/vpx_encoder.h
#define VPX_TS_MAX_LAYERS 5       // From vpx/vpx_encoder.h

// Test case categories for validation targeting
enum TestCategory {
    TEST_DIMENSIONS = 0,          // Invalid dimension ranges
    TEST_PROFILE,                 // Profile validation
    TEST_QUANTIZER,               // Quantizer range checks
    TEST_LAG_FRAMES,              // Lag-in-frames constraints
    TEST_MULTI_LAYER,             // Multi-layer configuration
    TEST_RATE_CONTROL,            // Rate control parameters
    TEST_KEYFRAME,                // Keyframe mode validation
    TEST_RESIZE,                  // Resize configuration
    TEST_COMPLEX_COMBINATIONS,    // Multiple validation failures
    TEST_CATEGORY_COUNT
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least some data for test selection
    if (size < 4) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Select test category based on input
    TestCategory category = static_cast<TestCategory>(fdp.ConsumeIntegralInRange<uint8_t>(0, TEST_CATEGORY_COUNT - 1));
    
    // Get default VP9 encoder configuration
    vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
    if (!iface) return 0;
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify configuration based on test category to trigger validation errors
    switch (category) {
        case TEST_DIMENSIONS: {
            // Test invalid dimension ranges (g_w, g_h outside 1-65536)
            bool test_zero = fdp.ConsumeBool();
            bool test_overflow = fdp.ConsumeBool();
            
            if (test_zero) {
                // Test zero dimensions (invalid)
                cfg.g_w = 0;
                cfg.g_h = 0;
            } else if (test_overflow) {
                // Test overflow dimensions (> 65536)
                cfg.g_w = 65537 + fdp.ConsumeIntegralInRange<uint32_t>(0, 1000);
                cfg.g_h = 65537 + fdp.ConsumeIntegralInRange<uint32_t>(0, 1000);
            } else {
                // Test mixed invalid combinations
                cfg.g_w = fdp.ConsumeBool() ? 0 : 65537;
                cfg.g_h = fdp.ConsumeBool() ? 0 : 65537;
            }
            
            // Also test timebase validation
            cfg.g_timebase.num = fdp.ConsumeBool() ? 0 : 1000000001;
            cfg.g_timebase.den = fdp.ConsumeBool() ? 0 : 1000000001;
            break;
        }
        
        case TEST_PROFILE: {
            // Test profile validation (g_profile > 3)
            cfg.g_profile = 4 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Also test profile-bit-depth combination validation
            if (fdp.ConsumeBool()) {
                cfg.g_bit_depth = static_cast<vpx_bit_depth_t>(VPX_BITS_12);
                cfg.g_input_bit_depth = 12;
            }
            break;
        }
        
        case TEST_QUANTIZER: {
            // Test quantizer range checks (rc_min_quantizer > rc_max_quantizer)
            uint8_t min_q = fdp.ConsumeIntegralInRange<uint8_t>(0, 63);
            uint8_t max_q = fdp.ConsumeIntegralInRange<uint8_t>(0, 63);
            
            if (min_q > max_q) {
                // This should trigger validation error: min > max
                cfg.rc_min_quantizer = min_q;
                cfg.rc_max_quantizer = max_q;
            } else {
                // Ensure min <= max but set values > 63 to trigger range error
                cfg.rc_min_quantizer = fdp.ConsumeBool() ? 64 : min_q;
                cfg.rc_max_quantizer = fdp.ConsumeBool() ? 64 : max_q;
            }
            
            // Test max quantizer > 63
            if (fdp.ConsumeBool()) {
                cfg.rc_max_quantizer = 64 + fdp.ConsumeIntegralInRange<uint8_t>(0, 100);
            }
            break;
        }
        
        case TEST_LAG_FRAMES: {
            // Test lag-in-frames constraints with max_gf_interval
            cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<uint32_t>(0, MAX_LAG_BUFFERS + 100);
            
            // Create extra config for max_gf_interval validation
            vpx_codec_ctx_t ctx;
            memset(&ctx, 0, sizeof(ctx));
            
            // Set max_gf_interval that would conflict with lag_in_frames
            // Note: This would need vpx_codec_control to set extra config
            // For now, we'll test lag_in_frames > MAX_LAG_BUFFERS
            if (fdp.ConsumeBool()) {
                cfg.g_lag_in_frames = MAX_LAG_BUFFERS + 1 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            }
            
            // Test g_threads > MAX_NUM_THREADS (indirect from validate_config)
            cfg.g_threads = 65 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100); // MAX_NUM_THREADS is 64
            break;
        }
        
        case TEST_MULTI_LAYER: {
            // Test multi-layer configuration validation
            // Set ss_number_layers * ts_number_layers > VPX_MAX_LAYERS (12)
            cfg.ss_number_layers = fdp.ConsumeIntegralInRange<uint32_t>(4, VPX_SS_MAX_LAYERS);
            cfg.ts_number_layers = fdp.ConsumeIntegralInRange<uint32_t>(4, VPX_TS_MAX_LAYERS);
            
            // Ensure product > VPX_MAX_LAYERS
            if (cfg.ss_number_layers * cfg.ts_number_layers <= VPX_MAX_LAYERS) {
                // Adjust to guarantee overflow
                cfg.ss_number_layers = VPX_SS_MAX_LAYERS;
                cfg.ts_number_layers = VPX_TS_MAX_LAYERS;
            }
            
            // Test layer_target_bitrate validation for temporal scalability
            if (cfg.ts_number_layers > 1 && fdp.ConsumeBool()) {
                // Set decreasing bitrates (invalid)
                for (unsigned int i = 0; i < cfg.ts_number_layers; ++i) {
                    cfg.layer_target_bitrate[i] = 100 * (cfg.ts_number_layers - i);
                }
            }
            
            // Test ts_rate_decimator validation
            if (cfg.ts_number_layers > 1 && fdp.ConsumeBool()) {
                // Set invalid ts_rate_decimator values (not powers of 2)
                cfg.ts_rate_decimator[cfg.ts_number_layers - 1] = fdp.ConsumeBool() ? 2 : 3;
                for (unsigned int i = 0; i < cfg.ts_number_layers - 1; ++i) {
                    cfg.ts_rate_decimator[i] = fdp.ConsumeIntegralInRange<uint32_t>(1, 10);
                }
            }
            break;
        }
        
        case TEST_RATE_CONTROL: {
            // Test rate control parameter validation
            // Set undershoot/overshoot percentages > 100
            cfg.rc_undershoot_pct = 101 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            cfg.rc_overshoot_pct = 101 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Test rc_2pass_vbr_bias_pct > 100
            cfg.rc_2pass_vbr_bias_pct = 101 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Test rc_2pass_vbr_corpus_complexity > 10000
            cfg.rc_2pass_vbr_corpus_complexity = 10001 + fdp.ConsumeIntegralInRange<uint32_t>(0, 1000);
            
            // Test rc_end_usage invalid value
            cfg.rc_end_usage = static_cast<vpx_rc_mode>(VPX_Q + 1 + fdp.ConsumeIntegralInRange<uint32_t>(0, 10));
            
            // Test rc_dropframe_thresh > 100
            cfg.rc_dropframe_thresh = 101 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            break;
        }
        
        case TEST_KEYFRAME: {
            // Test keyframe mode and distance validation
            // Set invalid kf_mode
            cfg.kf_mode = static_cast<vpx_kf_mode>(VPX_KF_DISABLED + 3 + fdp.ConsumeIntegralInRange<uint32_t>(0, 10));
            
            // Test kf_min_dist > 0 when kf_mode != VPX_KF_DISABLED
            if (fdp.ConsumeBool()) {
                cfg.kf_mode = VPX_KF_AUTO;
                cfg.kf_min_dist = 1 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
                cfg.kf_max_dist = cfg.kf_min_dist + 10;
            }
            
            // Test kf_min_dist != kf_max_dist when kf_min_dist > 0
            if (fdp.ConsumeBool()) {
                cfg.kf_mode = VPX_KF_AUTO;
                cfg.kf_min_dist = 10;
                cfg.kf_max_dist = 20;
            }
            break;
        }
        
        case TEST_RESIZE: {
            // Test resize configuration validation when rc_resize_allowed=1
            cfg.rc_resize_allowed = 1;
            
            // Set invalid scaled dimensions (greater than original)
            cfg.rc_scaled_width = cfg.g_w + 1 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            cfg.rc_scaled_height = cfg.g_h + 1 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Test resize thresholds > 100
            cfg.rc_resize_up_thresh = 101 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            cfg.rc_resize_down_thresh = 101 + fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Test boolean validation for rc_resize_allowed
            if (fdp.ConsumeBool()) {
                cfg.rc_resize_allowed = 2; // Not a boolean (0 or 1)
            }
            break;
        }
        
        case TEST_COMPLEX_COMBINATIONS: {
            // Test multiple validation failures simultaneously
            cfg.g_w = 0;  // Invalid dimension
            cfg.g_profile = 5;  // Invalid profile
            cfg.rc_min_quantizer = 50;
            cfg.rc_max_quantizer = 40;  // min > max
            cfg.g_lag_in_frames = MAX_LAG_BUFFERS + 10;
            cfg.ss_number_layers = 4;
            cfg.ts_number_layers = 4;  // 4*4=16 > VPX_MAX_LAYERS(12)
            cfg.rc_undershoot_pct = 150;
            cfg.rc_resize_allowed = 1;
            cfg.rc_scaled_width = cfg.g_w + 100;
            break;
        }
        
        default:
            // Should not reach here
            break;
    }
    
    // Now test the configuration with encoder initialization APIs
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    // Test vpx_codec_enc_init_ver with potentially invalid configuration
    vpx_codec_err_t init_result = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // The expectation is that invalid configurations should fail initialization
    // But we need to handle both success and failure cases properly
    if (init_result == VPX_CODEC_OK) {
        // Configuration passed validation (might happen for some edge cases)
        // Test vpx_codec_enc_config_set with further modifications
        vpx_codec_enc_cfg_t new_cfg = cfg;
        
        // Modify configuration to potentially trigger validation on set
        if (fdp.ConsumeBool()) {
            new_cfg.g_w = 0;  // Make it invalid
            vpx_codec_enc_config_set(&codec, &new_cfg);
        }
        
        // Test vpx_codec_control with specific control parameters that might affect validation
        // Use control IDs from vp8cx.h
        if (fdp.ConsumeBool()) {
            int ctrl_value = fdp.ConsumeIntegral<int>();
            vpx_codec_control(&codec, VP8E_SET_CPUUSED, ctrl_value);
        }
        
        if (fdp.ConsumeBool()) {
            unsigned int ctrl_value = fdp.ConsumeIntegral<unsigned int>();
            vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, ctrl_value);
        }
        
        if (fdp.ConsumeBool()) {
            unsigned int ctrl_value = fdp.ConsumeIntegral<unsigned int>();
            vpx_codec_control(&codec, VP8E_SET_ARNR_MAXFRAMES, ctrl_value);
        }
        
        if (fdp.ConsumeBool()) {
            unsigned int ctrl_value = fdp.ConsumeIntegral<unsigned int>();
            vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, ctrl_value);
        }
        
        if (fdp.ConsumeBool()) {
            unsigned int ctrl_value = fdp.ConsumeIntegral<unsigned int>();
            vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, ctrl_value);
        }
        
        if (fdp.ConsumeBool()) {
            unsigned int ctrl_value = fdp.ConsumeIntegral<unsigned int>();
            vpx_codec_control(&codec, VP8E_SET_SHARPNESS, ctrl_value);
        }
        
        // Test vpx_codec_enc_init_multi_ver if multi-encoder initialization makes sense
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
            const int num_encoders = fdp.ConsumeIntegralInRange<int>(1, 4);
            std::vector<vpx_codec_ctx_t> encoders(num_encoders);
            std::vector<vpx_codec_enc_cfg_t> configs(num_encoders, cfg);
            
            for (int i = 0; i < num_encoders; ++i) {
                // Modify each config slightly
                if (i > 0 && fdp.ConsumeBool()) {
                    configs[i].g_w += fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
                }
                vpx_codec_enc_init_multi_ver(&encoders[i], iface, &configs[i], num_encoders, 0, nullptr, VPX_ENCODER_ABI_VERSION);
                
                // Clean up if initialization succeeded
                if (encoders[i].err == VPX_CODEC_OK) {
                    vpx_codec_destroy(&encoders[i]);
                }
            }
        }
        
        // Clean up
        vpx_codec_destroy(&codec);
    } else {
        // Initialization failed due to validation errors - this is expected for invalid configs
        // The error code should be VPX_CODEC_INVALID_PARAM for validation failures
        // We don't need to do anything else for failed initialization
    }
    
    return 0;
}

} // namespace harness_014
