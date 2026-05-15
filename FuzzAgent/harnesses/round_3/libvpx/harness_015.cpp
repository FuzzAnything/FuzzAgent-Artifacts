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

namespace harness_015 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation paths
 * Specifically targeting complex multi-parameter validation scenarios in 
 * validate_config() function to unlock 242 blocked branches (#1 branch blocker)
 * 
 * This harness targets deeper validation logic, complex parameter interdependencies,
 * multi-field validation constraints, and boundary conditions that previous
 * harnesses (011 and 012) haven't covered comprehensively.
 * 
 * Testing strategy:
 * 1. Initialize VP9 encoder with default configuration
 * 2. Create complex invalid parameter combinations that trigger multiple
 *    validation checks simultaneously
 * 3. Test vpx_codec_enc_config_set() with interdependent invalid parameters
 * 4. Focus on validation paths that require multiple conditions to be true
 * 5. Target two-pass statistics validation with complex layer configurations
 * 6. Test profile/bit-depth compatibility validation with interdependent fields
 * 7. Validate spatial/temporal layer multiplication and bitrate ordering
 * 8. Test GF interval validation with lag_in_frames relationships
 * 
 * Key complex validation targets:
 * - Multi-condition validation: lines 224-236 (GF intervals + lag_in_frames)
 * - Two-pass statistics validation with spatial layers: lines 301-352
 * - Profile/bit-depth compatibility: lines 355-370
 * - Layer multiplication validation: lines 258-275
 * - Temporal scalability validation with rate decimators: lines 271-274
 * - Target level enumeration validation: lines 247-256
 * - Complex RANGE_CHECK combinations with interdependent fields
 */




// Local definitions for constants that would normally come from headers
const unsigned int MAX_NUM_THREADS = 64;
const unsigned int MAX_LAG_BUFFERS = 25;
const unsigned int PROFILE_0 = 0;
const unsigned int PROFILE_1 = 1;
const unsigned int PROFILE_2 = 2;
const unsigned int PROFILE_3 = 3;

// Minimum input size for complex multi-parameter testing
const size_t MIN_INPUT_SIZE = 256;

// Maximum configuration values for boundary testing
const unsigned int MAX_WIDTH = 65536;
const unsigned int MAX_HEIGHT = 65536;


// Two-pass statistics packet structure (simplified)
typedef struct {
    double frame;
    double weight;
    double intra_error;
    double coded_error;
    double sr_coded_error;
    double frame_noise_energy;
    double pcnt_inter;
    double pcnt_motion;
    double pcnt_second_ref;
    double pcnt_neutral;
    double intra_skip_pct;
    double intra_smooth_pct;
    double inactive_zone_rows;
    double inactive_zone_cols;
    double mv_row;
    double mv_col;
    double mv_in_out_count;
    double duration;
    double spatial_layer_id;
    double count;
} FIRSTPASS_STATS;
// Helper to create complex invalid VP9 encoder configuration with interdependencies
static void create_complex_invalid_vp9_config(vpx_codec_enc_cfg_t* cfg, 
                                             FuzzedDataProvider* fdp,
                                             uint8_t complex_scenario) {
    if (!cfg || !fdp) return;
    
    // Start with default configuration
    const vpx_codec_iface_t* vp9_iface = vpx_codec_vp9_cx();
    if (!vp9_iface) return;
    
    if (vpx_codec_enc_config_default(vp9_iface, cfg, 0) != VPX_CODEC_OK) {
        return;
    }
    
    // Apply complex multi-parameter invalid configurations
    // Each scenario triggers multiple validation checks simultaneously
    switch (complex_scenario % 16) {
        case 0: {
            // Scenario 0: Complex GF interval validation with lag_in_frames
            // Targets lines 224-236: GF intervals + lag_in_frames relationship
            cfg->g_lag_in_frames = fdp->ConsumeIntegralInRange<unsigned int>(1, 10);
            // Note: extra_cfg->max_gf_interval would need to be set via control code
            // This would trigger: "Set lag in frames to 0 (low delay) or >= (max-gf-interval + 2)"
            
            // Also set invalid layer configuration to trigger multiple validations
            cfg->ss_number_layers = VPX_SS_MAX_LAYERS;
            cfg->ts_number_layers = VPX_TS_MAX_LAYERS;
            // This should trigger: ss_number_layers * ts_number_layers > VPX_MAX_LAYERS
            
            // Set invalid profile for bit-depth validation
            cfg->g_profile = 0;  // Profile 0
            cfg->g_bit_depth = static_cast<vpx_bit_depth_t>(10);  // VPX_BITS_10
            // This should trigger: "Codec high bit-depth not supported in profile < 2"
            break;
        }
        
        case 1: {
            // Scenario 1: Two-pass statistics validation with spatial layers
            // Targets lines 301-352: Two-pass stats validation
            cfg->g_pass = VPX_RC_LAST_PASS;
            cfg->ss_number_layers = fdp->ConsumeIntegralInRange<unsigned int>(2, VPX_SS_MAX_LAYERS);
            cfg->ts_number_layers = fdp->ConsumeIntegralInRange<unsigned int>(1, VPX_TS_MAX_LAYERS);
            
            // Set invalid two-pass stats buffer
            cfg->rc_twopass_stats_in.buf = NULL;
            // This should trigger: "rc_twopass_stats_in.buf not set."
            
            // Also set invalid target level
            // Targets lines 247-256: target_level validation
            // Note: target_level is in extra_cfg, would need control code
            break;
        }
        
        case 2: {
            // Scenario 2: Complex spatial/temporal layer validation
            // Targets lines 258-275: Layer multiplication and bitrate ordering
            cfg->ss_number_layers = fdp->ConsumeIntegralInRange<unsigned int>(2, VPX_SS_MAX_LAYERS);
            cfg->ts_number_layers = fdp->ConsumeIntegralInRange<unsigned int>(2, VPX_TS_MAX_LAYERS);
            
            // Set decreasing bitrates to trigger validation error
            // Targets lines 264-267: ts_target_bitrate entries are not increasing
            for (unsigned int i = 0; i < cfg->ss_number_layers * cfg->ts_number_layers; i++) {
                cfg->layer_target_bitrate[i] = 1000 - (i * 100); // Decreasing values
            }
            
            // Also set invalid ts_rate_decimator factors
            // Targets lines 271-274: ts_rate_decimator factors are not powers of 2
            cfg->ts_rate_decimator[0] = 4;
            cfg->ts_rate_decimator[1] = 3;  // Not 2*4=8 - should trigger error
            
            break;
        }
        
        case 3: {
            // Scenario 3: Profile/bit-depth compatibility validation
            // Targets lines 355-370: Profile and bit-depth compatibility
            uint8_t profile_choice = fdp->ConsumeIntegral<uint8_t>() % 4;
            
            switch (profile_choice) {
                case 0:
                    // Profile 0 with high bit-depth
                    cfg->g_profile = 0;  // Profile 0
                    cfg->g_bit_depth = static_cast<vpx_bit_depth_t>(10);  // VPX_BITS_10
                    cfg->g_input_bit_depth = 10;
                    break;
                case 1:
                    // Profile 0 with 12-bit input
                    cfg->g_profile = 0;  // Profile 0
                    cfg->g_bit_depth = static_cast<vpx_bit_depth_t>(8);  // VPX_BITS_8
                    cfg->g_input_bit_depth = 12;
                    break;
                case 2:
                    // Profile 2 with 8-bit codec
                    cfg->g_profile = 2;  // Profile 2
                    cfg->g_bit_depth = static_cast<vpx_bit_depth_t>(8);  // VPX_BITS_8
                    cfg->g_input_bit_depth = 10;
                    break;
                case 3:
                    // Profile 3 with mismatched bit depths
                    cfg->g_profile = 3;  // Profile 3
                    cfg->g_bit_depth = static_cast<vpx_bit_depth_t>(10);  // VPX_BITS_10
                    cfg->g_input_bit_depth = 8;
                    break;
            }
            
            // Also set invalid width/height for additional validation
            cfg->g_w = 0;  // Invalid: should be >= 1
            cfg->g_h = 0;  // Invalid: should be >= 1
            break;
        }
        
        case 4: {
            // Scenario 4: Complex timebase and quantizer validation
            // Targets lines 194-199: timebase and quantizer validation
            cfg->g_timebase.num = 0;  // Invalid: should be >= 1
            cfg->g_timebase.den = 0;  // Invalid: should be >= 1
            
            // Set invalid quantizer range
            cfg->rc_min_quantizer = 64;  // Invalid: should be <= 63
            cfg->rc_max_quantizer = 32;  // Valid, but min > max is also checked
            
            // Also set invalid thread count
            cfg->g_threads = MAX_NUM_THREADS + 1;
            break;
        }
        
        case 5: {
            // Scenario 5: RC mode and percentage validation
            // Targets lines 207-211: RC end usage and percentage validation
            cfg->rc_end_usage = static_cast<vpx_rc_mode>(VPX_Q + 1);  // Invalid
            
            // Set invalid percentage values
            cfg->rc_undershoot_pct = 150;  // Invalid: should be <= 100
            cfg->rc_overshoot_pct = 150;   // Invalid: should be <= 100
            cfg->rc_2pass_vbr_bias_pct = 150;  // Invalid: should be <= 100
            
            // Also set invalid kf_mode
            cfg->kf_mode = static_cast<vpx_kf_mode>(VPX_KF_AUTO + 1);
            break;
        }
        
        case 6: {
            // Scenario 6: Resize and dropframe validation
            // Targets lines 239-242 and 214-216
            cfg->rc_resize_allowed = 1;
            cfg->rc_scaled_width = cfg->g_w + 1;  // Invalid: should be <= g_w
            cfg->rc_scaled_height = cfg->g_h + 1; // Invalid: should be <= g_h
            
            // Set invalid dropframe thresholds
            cfg->rc_dropframe_thresh = 150;  // Invalid: should be <= 100
            cfg->rc_resize_up_thresh = 150;  // Invalid: should be <= 100
            cfg->rc_resize_down_thresh = 150; // Invalid: should be <= 100
            
            break;
        }
        
        case 7: {
            // Scenario 7: Complex pass mode validation
            // Targets lines 217-221 and 301-353
            #if !CONFIG_REALTIME_ONLY
            cfg->g_pass = VPX_RC_LAST_PASS;
            #else
            cfg->g_pass = VPX_RC_LAST_PASS;  // Should be invalid in realtime-only
            #endif
            
            // Create fake two-pass stats with invalid size
            cfg->rc_twopass_stats_in.buf = malloc(sizeof(FIRSTPASS_STATS) * 3);
            cfg->rc_twopass_stats_in.sz = sizeof(FIRSTPASS_STATS) * 3 + 1;  // Truncated
            
            if (cfg->rc_twopass_stats_in.buf) {
                free(cfg->rc_twopass_stats_in.buf);
                cfg->rc_twopass_stats_in.buf = NULL;
            }
            break;
        }
        
        case 8: {
            // Scenario 8: KF interval validation in auto mode
            // Targets lines 277-283
            cfg->kf_mode = VPX_KF_AUTO;
            cfg->kf_min_dist = 10;  // Invalid in auto mode when != 0 and != kf_max_dist
            cfg->kf_max_dist = 20;
            
            // Also set invalid lag_in_frames for ARF group validation
            cfg->g_lag_in_frames = 5;
            // Note: would need extra_cfg->max_gf_interval set via control code
            break;
        }
        
        case 9: {
            // Scenario 9: Complex boolean and enum validation
            // Targets lines 200-204
            // Note: lossless, frame_parallel_decoding_mode, aq_mode, alt_ref_aq,
            // frame_periodic_boost are in extra_cfg and would need control codes
            
            // Set invalid profile
            cfg->g_profile = 4;  // Invalid: should be <= 3
            
            // Set invalid thread and lag values
            cfg->g_threads = MAX_NUM_THREADS + 5;
            cfg->g_lag_in_frames = MAX_LAG_BUFFERS + 5;
            break;
        }
        
        case 10: {
            // Scenario 10: Complex layer configuration with invalid multiplication
            // Targets line 258-259
            cfg->ss_number_layers = VPX_SS_MAX_LAYERS;
            cfg->ts_number_layers = VPX_TS_MAX_LAYERS;
            // ss_number_layers * ts_number_layers > VPX_MAX_LAYERS
            
            // Also set invalid dimensions
            cfg->g_w = MAX_WIDTH + 1;
            cfg->g_h = MAX_HEIGHT + 1;
            break;
        }
        
        case 11: {
            // Scenario 11: Temporal scalability with invalid rate decimators
            // Targets lines 271-274
            cfg->ts_number_layers = 3;
            cfg->ts_rate_decimator[0] = 8;
            cfg->ts_rate_decimator[1] = 4;
            cfg->ts_rate_decimator[2] = 2;  // Valid: 8, 4, 2 are powers of 2
            
            // But make them invalid by breaking the pattern
            cfg->ts_rate_decimator[1] = 3;  // Not 2*4=8
            
            // Also set ts_number_layers > 1 but don't set increasing bitrates
            cfg->layer_target_bitrate[0] = 1000;
            cfg->layer_target_bitrate[1] = 500;  // Decreasing - should trigger error
            
            break;
        }
        
        case 12: {
            // Scenario 12: Complex two-pass stats with spatial layers
            // Targets lines 313-341
            cfg->g_pass = VPX_RC_LAST_PASS;
            cfg->ss_number_layers = 2;
            cfg->ts_number_layers = 1;
            
            // Allocate invalid two-pass stats buffer
            size_t invalid_packet_count = 1;  // Too few packets
            cfg->rc_twopass_stats_in.sz = invalid_packet_count * sizeof(FIRSTPASS_STATS);
            cfg->rc_twopass_stats_in.buf = malloc(cfg->rc_twopass_stats_in.sz);
            
            if (cfg->rc_twopass_stats_in.buf) {
                // Don't initialize spatial_layer_id or count fields properly
                free(cfg->rc_twopass_stats_in.buf);
                cfg->rc_twopass_stats_in.buf = NULL;
            }
            break;
        }
        
        case 13: {
            // Scenario 13: Profile build configuration validation
            // Targets lines 355-359
            #if !CONFIG_VP9_HIGHBITDEPTH
            cfg->g_profile = PROFILE_2;  // Invalid if highbitdepth not configured
            #else
            cfg->g_profile = PROFILE_3;  // Valid with highbitdepth
            #endif
            
            // Set mismatched bit depths
            cfg->g_bit_depth = VPX_BITS_10;
            cfg->g_input_bit_depth = 8;
            break;
        }
        
        case 14: {
            // Scenario 14: Color space and range validation
            // Targets lines 371-372
            // Note: color_space and color_range are in extra_cfg
            
            // Set invalid AQ mode
            // Note: aq_mode is in extra_cfg, range is 0..(AQ_MODE_COUNT - 2)
            
            // Set invalid dimensions for additional coverage
            cfg->g_w = 0;
            cfg->g_h = 65537;  // > 65536
            break;
        }
        
        case 15: {
            // Scenario 15: Complex vizier RC params validation
            // Targets lines 374-389
            // Set invalid denominator values (should be 1..1000)
            cfg->active_wq_factor.den = 0;
            cfg->err_per_mb_factor.den = 1001;
            cfg->sr_default_decay_limit.den = 0;
            
            // Also set invalid use_vizier_rc_params
            cfg->use_vizier_rc_params = 2;  // Should be 0 or 1
            
            // Set invalid frame dimensions
            cfg->g_w = 65537;
            cfg->g_h = 0;
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for complex multi-parameter testing
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which complex validation scenario to test
    uint8_t complex_scenario = fdp.ConsumeIntegral<uint8_t>() % 16;
    
    // Initialize VP9 encoder
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
    
    // Set basic valid configuration for initialization
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.rc_target_bitrate = 1000;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    // Initialize encoder
    vpx_codec_err_t init_err = vpx_codec_enc_init_ver(&enc, vp9_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (init_err != VPX_CODEC_OK) {
        // If initialization fails, try with different parameters
        cfg.g_w = 320;
        cfg.g_h = 240;
        init_err = vpx_codec_enc_init_ver(&enc, vp9_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (init_err != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Create complex invalid configuration based on scenario
    vpx_codec_enc_cfg_t invalid_cfg;
    create_complex_invalid_vp9_config(&invalid_cfg, &fdp, complex_scenario);
    
    // Try to set the invalid configuration
    // This should trigger validate_config() with our complex invalid parameters
    vpx_codec_err_t config_err = vpx_codec_enc_config_set(&enc, &invalid_cfg);
    
    // The error is expected - we're testing validation failure paths
    (void)config_err;
    
    // Clean up
    vpx_codec_destroy(&enc);
    
    return 0;
}

} // namespace harness_015
