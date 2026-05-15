#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"

namespace harness_011 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation paths
 * Targets: Validation error paths in vp9_cx_iface.c validate_config() function
 * 
 * Coverage analysis identified 220+ blocked branches in VP9 encoder configuration
 * validation paths. This harness specifically targets validation failure scenarios
 * and error handling paths that existing harnesses haven't covered.
 * 
 * Testing strategy:
 * 1. Initialize VP9 encoder with default configuration
 * 2. Generate various invalid configurations to trigger validation errors
 * 3. Test vpx_codec_enc_config_set() with invalid parameters
 * 4. Focus on boundary conditions and invalid parameter combinations
 * 5. Test error handling and cleanup after validation failures
 * 
 * Key validation targets (from coverage analysis):
 * - RANGE_CHECK failures for various configuration fields
 * - Layer configuration validation errors
 * - Temporal/spatial scalability validation
 * - Bitrate allocation validation
 * - Frame rate and resolution validation
 */




// Local definitions for constants that would normally come from headers
const unsigned int MAX_NUM_THREADS = 64;
const unsigned int MAX_LAG_BUFFERS = 25;
const unsigned int PROFILE_0 = 0;
const unsigned int PROFILE_1 = 1;
const unsigned int PROFILE_2 = 2;
const unsigned int PROFILE_3 = 3;
const unsigned int MAX_WIDTH = 16384;
const unsigned int MAX_HEIGHT = 16384;
const unsigned int MAX_THREADS = 64;
const unsigned int MIN_INPUT_SIZE = 10;

// Helper function to create intentionally invalid VP9 encoder configuration
static void create_invalid_vp9_config(vpx_codec_enc_cfg_t* cfg, 
                                     FuzzedDataProvider* fdp,
                                     uint8_t invalid_config_type) {
    if (!cfg || !fdp) return;
    
    // Start with default configuration
    const vpx_codec_iface_t* vp9_iface = vpx_codec_vp9_cx();
    if (!vp9_iface) return;
    
    if (vpx_codec_enc_config_default(vp9_iface, cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Apply specific invalid configurations based on type
    switch (invalid_config_type % 20) {
        case 0: {
            // Invalid width/height (out of range)
            cfg->g_w = fdp->ConsumeIntegralInRange<unsigned int>(MAX_WIDTH + 1, MAX_WIDTH * 2);
            cfg->g_h = fdp->ConsumeIntegralInRange<unsigned int>(MAX_HEIGHT + 1, MAX_HEIGHT * 2);
            break;
        }
        
        case 1: {
            // Invalid width/height (zero or negative)
            cfg->g_w = 0;
            cfg->g_h = 0;
            break;
        }
        
        case 2: {
            // Invalid timebase (zero denominator)
            cfg->g_timebase.num = fdp->ConsumeIntegralInRange<int>(1, 100);
            cfg->g_timebase.den = 0;
            break;
        }
        
        case 3: {
            // Invalid timebase (zero numerator)
            cfg->g_timebase.num = 0;
            cfg->g_timebase.den = fdp->ConsumeIntegralInRange<int>(1, 100);
            break;
        }
        
        case 4: {
            // Invalid quantizer range (min > max)
            cfg->rc_min_quantizer = fdp->ConsumeIntegralInRange<unsigned int>(32, 63);
            cfg->rc_max_quantizer = fdp->ConsumeIntegralInRange<unsigned int>(0, cfg->rc_min_quantizer - 1);
            break;
        }
        
        case 5: {
            // Invalid quantizer values (> 63)
            cfg->rc_min_quantizer = fdp->ConsumeIntegralInRange<unsigned int>(64, 100);
            cfg->rc_max_quantizer = fdp->ConsumeIntegralInRange<unsigned int>(64, 100);
            break;
        }
        
        case 6: {
            // Invalid profile (> 3)
            cfg->g_profile = fdp->ConsumeIntegralInRange<unsigned int>(4, 10);
            break;
        }
        
        case 7: {
            // Invalid thread count (exceeds MAX_NUM_THREADS)
            cfg->g_threads = fdp->ConsumeIntegralInRange<unsigned int>(MAX_THREADS + 1, MAX_THREADS * 2);
            break;
        }
        
        case 8: {
            // Invalid lag in frames (exceeds MAX_LAG_BUFFERS)
            cfg->g_lag_in_frames = fdp->ConsumeIntegralInRange<unsigned int>(256, 512);
            break;
        }
        
        case 9: {
            // Invalid rc_end_usage value
            cfg->rc_end_usage = static_cast<vpx_rc_mode>(fdp->ConsumeIntegralInRange<unsigned int>(VPX_Q + 1, VPX_Q + 10));
            break;
        }
        
        case 10: {
            // Invalid percentage values (> 100)
            cfg->rc_undershoot_pct = fdp->ConsumeIntegralInRange<unsigned int>(101, 200);
            cfg->rc_overshoot_pct = fdp->ConsumeIntegralInRange<unsigned int>(101, 200);
            break;
        }
        
        case 11: {
            // Invalid kf_mode
            cfg->kf_mode = static_cast<vpx_kf_mode>(fdp->ConsumeIntegralInRange<unsigned int>(VPX_KF_AUTO + 1, VPX_KF_AUTO + 10));
            break;
        }
        
        case 12: {
            // Invalid spatial/temporal layer configuration
            cfg->ss_number_layers = fdp->ConsumeIntegralInRange<unsigned int>(VPX_SS_MAX_LAYERS + 1, VPX_SS_MAX_LAYERS * 2);
            cfg->ts_number_layers = fdp->ConsumeIntegralInRange<unsigned int>(VPX_TS_MAX_LAYERS + 1, VPX_TS_MAX_LAYERS * 2);
            break;
        }
        
        case 13: {
            // Invalid layer product (ss * ts > VPX_MAX_LAYERS)
            cfg->ss_number_layers = VPX_SS_MAX_LAYERS;
            cfg->ts_number_layers = VPX_TS_MAX_LAYERS;
            // This should trigger: ss_number_layers * ts_number_layers > VPX_MAX_LAYERS
            break;
        }
        
        case 14: {
            // Non-increasing layer target bitrates
            cfg->ss_number_layers = 2;
            cfg->ts_number_layers = 2;
            // Set decreasing bitrates to trigger validation error
            cfg->layer_target_bitrate[0] = 1000;
            cfg->layer_target_bitrate[1] = 500;  // Decreasing - should trigger error
            cfg->layer_target_bitrate[2] = 800;
            cfg->layer_target_bitrate[3] = 1200;
            break;
        }
        
        case 15: {
            // Invalid ts_rate_decimator factors (not powers of 2)
            cfg->ts_number_layers = 3;
            cfg->ts_rate_decimator[0] = 4;
            cfg->ts_rate_decimator[1] = 3;  // Not 2*4=8 - should trigger error
            cfg->ts_rate_decimator[2] = 1;
            break;
        }
        
        case 16: {
            // Invalid kf_min_dist in auto mode
            cfg->kf_mode = VPX_KF_AUTO;
            cfg->kf_min_dist = fdp->ConsumeIntegralInRange<unsigned int>(1, 100);
            cfg->kf_max_dist = cfg->kf_min_dist * 2;
            // Should trigger: kf_min_dist not supported in auto mode
            break;
        }
        
        case 17: {
            // Invalid bit depth/profile combination
            cfg->g_profile = 0;  // Profile 0
            cfg->g_bit_depth = VPX_BITS_10;  // 10-bit not supported with profile 0
            break;
        }
        
        case 18: {
            // Invalid resize configuration
            cfg->rc_resize_allowed = 1;
            cfg->rc_scaled_width = cfg->g_w + 100;  // Scaled width > original width
            cfg->rc_scaled_height = cfg->g_h + 100; // Scaled height > original height
            break;
        }
        
        case 19: {
            // Mixed invalid configurations
            cfg->g_w = 0;
            cfg->rc_min_quantizer = 70;
            cfg->rc_max_quantizer = 20;
            cfg->g_profile = 10;
            cfg->ss_number_layers = 10;
            break;
        }
        
        default:
            break;
    }
}

// Test specific validation scenarios for temporal scalability
static void test_temporal_scalability_validation(vpx_codec_ctx_t* enc, 
                                                vpx_codec_enc_cfg_t* cfg,
                                                FuzzedDataProvider* fdp) {
    if (!enc || !cfg || !fdp) return;
    
    uint8_t test_case = fdp->ConsumeIntegral<uint8_t>() % 4;
    
    // Start with valid default config
    const vpx_codec_iface_t* vp9_iface = vpx_codec_vp9_cx();
    if (!vp9_iface) return;
    
    if (vpx_codec_enc_config_default(vp9_iface, cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    switch (test_case) {
        case 0: {
            // Test invalid temporal layer bitrate ordering
            cfg->ts_number_layers = 3;
            cfg->ts_target_bitrate[0] = 500;
            cfg->ts_target_bitrate[1] = 300;  // Decreasing - should be invalid
            cfg->ts_target_bitrate[2] = 800;
            break;
        }
        
        case 1: {
            // Test invalid rate decimator sequence
            cfg->ts_number_layers = 4;
            cfg->ts_rate_decimator[0] = 8;
            cfg->ts_rate_decimator[1] = 4;
            cfg->ts_rate_decimator[2] = 3;  // Not 2 - should be invalid
            cfg->ts_rate_decimator[3] = 1;
            break;
        }
        
        case 2: {
            // Test invalid layer count
            cfg->ts_number_layers = 0;  // Should be >= 1
            break;
        }
        
        case 3: {
            // Test mixed spatial/temporal layer validation
            cfg->ss_number_layers = 3;
            cfg->ts_number_layers = 3;
            // Set up invalid bitrate progression across combined layers
            for (int i = 0; i < 9; i++) {
                cfg->layer_target_bitrate[i] = (i % 2 == 0) ? 1000 : 500;  // Alternating
            }
            break;
        }
    }
    
    // Try to apply the invalid configuration
    vpx_codec_err_t err = vpx_codec_enc_config_set(enc, cfg);
    // Error expected - don't check result, just exercise validation path
    (void)err;
}

// Test configuration validation with NULL/invalid pointers
static void test_null_pointer_validation(FuzzedDataProvider* fdp) {
    if (!fdp) return;
    
    uint8_t test_type = fdp->ConsumeIntegral<uint8_t>() % 4;
    
    vpx_codec_ctx_t enc;
    memset(&enc, 0, sizeof(enc));
    
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    const vpx_codec_iface_t* vp9_iface = vpx_codec_vp9_cx();
    if (!vp9_iface) return;
    
    switch (test_type) {
        case 0: {
            // Test with NULL encoder context
            vpx_codec_enc_config_set(nullptr, &cfg);
            break;
        }
        
        case 1: {
            // Test with NULL config
            vpx_codec_enc_config_set(&enc, nullptr);
            break;
        }
        
        case 2: {
            // Test with uninitialized encoder context
            vpx_codec_enc_config_set(&enc, &cfg);
            break;
        }
        
        case 3: {
            // Test with partially initialized config
            vpx_codec_enc_config_set(&enc, &cfg);
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection from fuzzer input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t invalid_config_type = fdp.ConsumeIntegral<uint8_t>() % 20;
    
    // Initialize VP9 encoder with default configuration
    const vpx_codec_iface_t* vp9_iface = vpx_codec_vp9_cx();
    if (!vp9_iface) {
        return 0;
    }
    
    vpx_codec_ctx_t enc;
    vpx_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (vpx_codec_enc_config_default(vp9_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set reasonable default values for testing
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder with valid configuration first
    if (vpx_codec_enc_init(&enc, vp9_iface, &cfg, 0) != VPX_CODEC_OK) {
        // If initialization fails, try error handling tests
        test_null_pointer_validation(&fdp);
        return 0;
    }
    
    // Execute selected test scenario
    switch (test_scenario) {
        case 0: {
            // Test invalid configuration validation
            create_invalid_vp9_config(&cfg, &fdp, invalid_config_type);
            vpx_codec_err_t err = vpx_codec_enc_config_set(&enc, &cfg);
            // Don't check error - we're testing error paths
            (void)err;
            break;
        }
        
        case 1: {
            // Test temporal scalability validation
            test_temporal_scalability_validation(&enc, &cfg, &fdp);
            break;
        }
        
        case 2: {
            // Test multiple sequential invalid configurations
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                create_invalid_vp9_config(&cfg, &fdp, (invalid_config_type + i) % 20);
                vpx_codec_err_t err = vpx_codec_enc_config_set(&enc, &cfg);
                (void)err;
                
                // Also test error handling APIs
                const char* error_str = vpx_codec_error(&enc);
                const char* error_detail = vpx_codec_error_detail(&enc);
                (void)error_str;
                (void)error_detail;
            }
            break;
        }
        
        case 3: {
            // Test boundary condition validations
            // Create configs at exact boundaries
            cfg.g_w = 65536;  // Maximum allowed
            cfg.g_h = 65536;  // Maximum allowed
            cfg.rc_min_quantizer = 63;  // Maximum allowed
            cfg.rc_max_quantizer = 63;  // Maximum allowed
            cfg.g_profile = 3;  // Maximum allowed
            
            vpx_codec_err_t err = vpx_codec_enc_config_set(&enc, &cfg);
            (void)err;
            
            // Now test just beyond boundaries
            create_invalid_vp9_config(&cfg, &fdp, invalid_config_type);
            err = vpx_codec_enc_config_set(&enc, &cfg);
            (void)err;
            break;
        }
    }
    
    // Clean up encoder context
    vpx_codec_destroy(&enc);
    
    return 0;
}

} // namespace harness_011
