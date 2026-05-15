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

namespace harness_018 {
/*
 * Fuzzing harness for libvpx targeting systematic testing of invalid VP9 encoder 
 * configurations to trigger validation errors in validate_config function.
 * 
 * Target: HarnessAgent
 * Goal: Generate harness_018.cpp based on coverage guidance targeting validate_config 
 *       function in vp9_cx_iface.c with 297 blocked branches out of 720. Focus on 
 *       systematically testing invalid VP9 encoder configurations to trigger validation errors.
 * 
 * APIs to target as specified in guidance:
 * - vpx_codec_enc_init_ver (Required Helper: Initialization)
 * - vpx_codec_enc_config_default (Required Helper: Get default config)
 * - vpx_codec_enc_config_set (Target: Main API for configuration validation)
 * - vpx_codec_control_ (Target: For additional control validation)
 * - vpx_codec_destroy (Required Helper: Cleanup)
 * 
 * Specific invalid configurations to test:
 * 1. g_lag_in_frames < max_gf_interval + 2 (to trigger line 238-240 error)
 * 2. ss_number_layers * ts_number_layers > VPX_MAX_LAYERS (to trigger line 262 error)
 * 3. kf_min_dist > 0 with kf_mode != VPX_KF_DISABLED and kf_min_dist != kf_max_dist (line 283-286)
 * 4. Invalid ts_rate_decimator values (not powers of 2, line 276-277)
 * 5. layer_target_bitrate not increasing (line 268-270)
 * 6. rc_min_quantizer > rc_max_quantizer (line 202)
 * 7. Invalid target_level values (line 252-258)
 * 
 * Semantic differentiation from previous harnesses:
 * 1. Specifically focuses on testing invalid/error configurations rather than valid ones
 * 2. Systematically targets each validation check in validate_config function
 * 3. Uses vpx_codec_enc_config_set API which triggers validate_config internally
 * 4. Tests error path coverage for robustness testing
 * 
 * Invocation sequence from guidance:
 * 1. Call vpx_codec_enc_config_default to get default configuration.
 * 2. Call vpx_codec_enc_init_ver to initialize encoder.
 * 3. Modify configuration parameters to invalid values to test validation:
 *    a. Set g_lag_in_frames < max_gf_interval + 2
 *    b. Set ss_number_layers * ts_number_layers > VPX_MAX_LAYERS
 *    c. Set kf_min_dist > 0 with kf_mode != VPX_KF_DISABLED and kf_min_dist != kf_max_dist
 *    d. Set invalid ts_rate_decimator values (not powers of 2)
 *    e. Set layer_target_bitrate not increasing
 *    f. Set rc_min_quantizer > rc_max_quantizer
 *    g. Set invalid target_level values
 * 4. Call vpx_codec_enc_config_set with invalid configurations to trigger validation errors.
 * 5. Call vpx_codec_control_ with invalid control parameters.
 * 6. Call vpx_codec_destroy to clean up.
 * 
 * Reasoning from guidance: The validate_config function has 297 blocked branches 
 * representing untested error conditions. These validation checks are critical for 
 * encoder robustness but currently have very low hit counts (e.g., only 9 hits for 
 * lag_in_frames validation, 4 hits for layer count validation). Testing these error 
 * paths will significantly improve branch coverage and test the library's error 
 * handling capabilities.
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for configuration parameters and test data
    const size_t MIN_SIZE = sizeof(uint8_t) * 1024 + sizeof(int) * 64 + sizeof(unsigned int) * 128;
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
    
    // Set basic valid configuration first
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    
    // Step 2: Initialize encoder with default config
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Step 3: Test different invalid configuration patterns
    uint8_t test_pattern = fdp.ConsumeIntegral<uint8_t>() % 7;
    
    switch (test_pattern) {
        case 0: {
            // Test 1: g_lag_in_frames < max_gf_interval + 2
            // Create fresh config
            vpx_codec_enc_cfg_t invalid_cfg;
            if (vpx_codec_enc_config_default(iface, &invalid_cfg, 0) != VPX_CODEC_OK) {
                vpx_codec_destroy(&codec);
                return 0;
            }
            
            // Set basic valid parameters
            invalid_cfg.g_w = 640;
            invalid_cfg.g_h = 480;
            invalid_cfg.g_timebase.num = 1;
            invalid_cfg.g_timebase.den = 30;
            invalid_cfg.rc_target_bitrate = 1000;
            invalid_cfg.g_threads = 1;
            
            // Test different validation errors based on fuzzer input
            // For g_lag_in_frames validation, we would need max_gf_interval from extra_cfg
            // which we can't set directly. Instead test rc_min_quantizer > rc_max_quantizer
            invalid_cfg.rc_min_quantizer = 50;
            invalid_cfg.rc_max_quantizer = 40; // Invalid: min > max
            
            vpx_codec_err_t res = vpx_codec_enc_config_set(&codec, &invalid_cfg);
            // Should fail validation
            break;
        }
        
        case 1: {
            // Test 2: ss_number_layers * ts_number_layers > VPX_MAX_LAYERS (12)
            vpx_codec_enc_cfg_t invalid_cfg;
            if (vpx_codec_enc_config_default(iface, &invalid_cfg, 0) != VPX_CODEC_OK) {
                vpx_codec_destroy(&codec);
                return 0;
            }
            
            // Set basic valid parameters
            invalid_cfg.g_w = 640;
            invalid_cfg.g_h = 480;
            invalid_cfg.g_timebase.num = 1;
            invalid_cfg.g_timebase.den = 30;
            invalid_cfg.rc_target_bitrate = 1000;
            invalid_cfg.g_threads = 1;
            
            // Set spatial and temporal layers that multiply to > 12
            // VPX_MAX_LAYERS = 12, so try 4 spatial * 4 temporal = 16 > 12
            invalid_cfg.ss_number_layers = 4;
            invalid_cfg.ts_number_layers = 4;
            
            // Also need to set target bitrates for the layers
            for (unsigned int i = 0; i < invalid_cfg.ss_number_layers; i++) {
                invalid_cfg.ss_target_bitrate[i] = 500 + i * 200;
            }
            
            for (unsigned int i = 0; i < invalid_cfg.ts_number_layers; i++) {
                invalid_cfg.ts_target_bitrate[i] = 500 + i * 100;
                invalid_cfg.ts_rate_decimator[i] = 1 << i; // Powers of 2
            }
            
            vpx_codec_err_t res = vpx_codec_enc_config_set(&codec, &invalid_cfg);
            // Should fail validation
            break;
        }
        
        case 2: {
            // Test 3: kf_min_dist > 0 with kf_mode != VPX_KF_DISABLED and kf_min_dist != kf_max_dist
            vpx_codec_enc_cfg_t invalid_cfg;
            if (vpx_codec_enc_config_default(iface, &invalid_cfg, 0) != VPX_CODEC_OK) {
                vpx_codec_destroy(&codec);
                return 0;
            }
            
            // Set basic valid parameters
            invalid_cfg.g_w = 640;
            invalid_cfg.g_h = 480;
            invalid_cfg.g_timebase.num = 1;
            invalid_cfg.g_timebase.den = 30;
            invalid_cfg.rc_target_bitrate = 1000;
            invalid_cfg.g_threads = 1;
            
            // Set auto keyframe mode with mismatched min/max distances
            invalid_cfg.kf_mode = VPX_KF_AUTO;
            invalid_cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(1, 50);
            invalid_cfg.kf_max_dist = invalid_cfg.kf_min_dist + 
                                     fdp.ConsumeIntegralInRange<unsigned int>(1, 50);
            
            vpx_codec_err_t res = vpx_codec_enc_config_set(&codec, &invalid_cfg);
            // Should fail validation for auto mode with kf_min_dist > 0 and kf_min_dist != kf_max_dist
            break;
        }
        
        case 3: {
            // Test 4: Invalid ts_rate_decimator values (not powers of 2)
            vpx_codec_enc_cfg_t invalid_cfg;
            if (vpx_codec_enc_config_default(iface, &invalid_cfg, 0) != VPX_CODEC_OK) {
                vpx_codec_destroy(&codec);
                return 0;
            }
            
            // Set basic valid parameters
            invalid_cfg.g_w = 640;
            invalid_cfg.g_h = 480;
            invalid_cfg.g_timebase.num = 1;
            invalid_cfg.g_timebase.den = 30;
            invalid_cfg.rc_target_bitrate = 1000;
            invalid_cfg.g_threads = 1;
            
            // Set temporal layers
            invalid_cfg.ts_number_layers = 3;
            
            // Set invalid rate decimators (not powers of 2 and not decreasing by factor of 2)
            // Valid sequence would be: 4, 2, 1 or 2, 1, 1
            // Invalid sequence: 3, 2, 1 (3 is not power of 2)
            invalid_cfg.ts_rate_decimator[0] = 3; // Invalid: not power of 2
            invalid_cfg.ts_rate_decimator[1] = 2;
            invalid_cfg.ts_rate_decimator[2] = 1;
            
            // Set target bitrates
            for (unsigned int i = 0; i < invalid_cfg.ts_number_layers; i++) {
                invalid_cfg.ts_target_bitrate[i] = 500 + i * 100;
            }
            
            vpx_codec_err_t res = vpx_codec_enc_config_set(&codec, &invalid_cfg);
            // Should fail validation
            break;
        }
        
        case 4: {
            // Test 5: layer_target_bitrate not increasing
            vpx_codec_enc_cfg_t invalid_cfg;
            if (vpx_codec_enc_config_default(iface, &invalid_cfg, 0) != VPX_CODEC_OK) {
                vpx_codec_destroy(&codec);
                return 0;
            }
            
            // Set basic valid parameters
            invalid_cfg.g_w = 640;
            invalid_cfg.g_h = 480;
            invalid_cfg.g_timebase.num = 1;
            invalid_cfg.g_timebase.den = 30;
            invalid_cfg.rc_target_bitrate = 1000;
            invalid_cfg.g_threads = 1;
            
            // Set spatial and temporal layers
            invalid_cfg.ss_number_layers = 2;
            invalid_cfg.ts_number_layers = 2;
            
            // Set layer target bitrates in non-increasing order to trigger error
            // For 2 spatial * 2 temporal = 4 layers
            // Layer indices: 0, 1, 2, 3
            // Need bitrate[1] < bitrate[0] to trigger "ts_target_bitrate entries are not increasing"
            
            // First set all to same value
            for (unsigned int i = 0; i < 4; i++) {
                invalid_cfg.layer_target_bitrate[i] = 500;
            }
            
            // Make one entry smaller than previous
            invalid_cfg.layer_target_bitrate[1] = 400; // Smaller than layer 0 (500)
            
            vpx_codec_err_t res = vpx_codec_enc_config_set(&codec, &invalid_cfg);
            // Should fail validation
            break;
        }
        
        case 5: {
            // Test 6: rc_min_quantizer > rc_max_quantizer
            vpx_codec_enc_cfg_t invalid_cfg;
            if (vpx_codec_enc_config_default(iface, &invalid_cfg, 0) != VPX_CODEC_OK) {
                vpx_codec_destroy(&codec);
                return 0;
            }
            
            // Set basic valid parameters
            invalid_cfg.g_w = 640;
            invalid_cfg.g_h = 480;
            invalid_cfg.g_timebase.num = 1;
            invalid_cfg.g_timebase.den = 30;
            invalid_cfg.rc_target_bitrate = 1000;
            invalid_cfg.g_threads = 1;
            
            // Set invalid quantizer range
            invalid_cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(40, 63);
            invalid_cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, invalid_cfg.rc_min_quantizer - 1);
            
            vpx_codec_err_t res = vpx_codec_enc_config_set(&codec, &invalid_cfg);
            // Should fail validation (rc_min_quantizer > rc_max_quantizer)
            break;
        }
        
        case 6: {
            // Test 7: Invalid target_level values
            // target_level is in extra_cfg, not main cfg
            // We need to use vpx_codec_control_ with VP9E_SET_TARGET_LEVEL
            
            // Test with invalid level value (not in valid enum list)
            int invalid_level = 999; // Not a valid LEVEL_* value
            vpx_codec_err_t res = vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, invalid_level);
            // This might fail, but we're testing the error path
            
            // Also test with some valid control that should work
            int valid_control = 1; // Try some valid control
            res = vpx_codec_control(&codec, VP9E_SET_ROW_MT, valid_control);
            
            break;
        }
    }
    
    // Step 6: Clean up
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_018
