#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"

namespace harness_010 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation targeting
 * specific validation paths in validate_config function (227 blocked branches).
 * 
 * Target APIs: vpx_codec_enc_config_default, vpx_codec_enc_init_ver, 
 *              vpx_codec_encode, vpx_codec_get_cx_data, vpx_codec_control_,
 *              vpx_codec_destroy
 * 
 * Focus: Systematic testing of specific validation paths identified as coverage gaps:
 * 1. Multi-pass encoding configuration (VPX_RC_LAST_PASS) with two-pass stats
 * 2. Resize-enabled encoding with scaled dimensions  
 * 3. Temporal and spatial layering with proper bitrate allocation
 * 4. Auto keyframe mode with kf_min_dist restrictions
 * 5. Golden frame interval validations (min_gf_interval, max_gf_interval)
 * 6. Lag-in-frames constraints for ARF groups
 * 7. Various encoder control parameter ranges
 * 
 * Semantic differentiation from harness_009.cpp:
 * - harness_009 tests edge cases for individual parameters broadly
 * - harness_010 focuses on specific validation path combinations and sequences
 * - harness_010 follows exact invocation sequence from coverage guidance
 * - harness_010 specifically tests the 7 validation paths with proper dependencies
 */



// Define constants used in validation (from libvpx internal headers)
#define MAX_LAG_BUFFERS 25  // From vp9/encoder/vp9_lookahead.h
#define MAX_ARF_LAYERS 6    // From vp9/encoder/vp9_firstpass.h

// VP9 level constants (from vp9/encoder/vp9_encoder.h)
enum {
    LEVEL_UNKNOWN = 0,
    LEVEL_AUTO = 1,
    LEVEL_1 = 10,
    LEVEL_1_1 = 11,
    LEVEL_2 = 20,
    LEVEL_2_1 = 21,
    LEVEL_3 = 30,
    LEVEL_3_1 = 31,
    LEVEL_4 = 40,
    LEVEL_4_1 = 41,
    LEVEL_5 = 50,
    LEVEL_5_1 = 51,
    LEVEL_5_2 = 52,
    LEVEL_6 = 60,
    LEVEL_6_1 = 61,
    LEVEL_6_2 = 62,
    LEVEL_MAX = 255
};

// Mock FIRSTPASS_STATS structure for two-pass stats testing
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
    double pcnt_intra_low;
    double pcnt_intra_high;
    double intra_skip_pct;
    double intra_smooth_pct;
    double inactive_zone_rows;
    double inactive_zone_cols;
    double MVr;
    double mvr_abs;
    double MVc;
    double mvc_abs;
    double MVrv;
    double MVcv;
    double mv_in_out_count;
    double duration;
    double count;
    double new_mv_count;
    int64_t spatial_layer_id;
} FIRSTPASS_STATS;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for complex configuration structures
    const size_t MIN_SIZE = 512;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Use VP9 encoder for configuration validation testing
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        return 0;
    }
    
    // 1. Call vpx_codec_enc_config_default to get default VP9 encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t config_err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    
    if (config_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test up to 3 different configuration scenarios
    unsigned int num_scenarios = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
    
    for (unsigned int scenario_idx = 0; scenario_idx < num_scenarios && fdp.remaining_bytes() > 200; scenario_idx++) {
        // Reset to default config for each scenario
        config_err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        if (config_err != VPX_CODEC_OK) {
            continue;
        }
        
        // Scenario type determines which validation paths to test
        unsigned int scenario_type = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
        
        // Declare stats_data as unique_ptr to manage heap-allocated buffer lifetime
        std::unique_ptr<uint8_t[]> stats_buffer;
        size_t stats_size = 0;
        
        switch (scenario_type) {
            case 0: { // Multi-pass encoding with two-pass stats
                // Set g_pass to VPX_RC_LAST_PASS for two-pass encoding
                cfg.g_pass = VPX_RC_LAST_PASS;
                
                // Create two-pass stats data (mock structure)
                stats_size = fdp.ConsumeIntegralInRange<size_t>(sizeof(FIRSTPASS_STATS) * 2, 
                                                                 sizeof(FIRSTPASS_STATS) * 100);
                if (stats_size > fdp.remaining_bytes()) {
                    stats_size = fdp.remaining_bytes();
                }
                
                // Allocate heap buffer for stats data that will persist through encoder lifetime
                stats_buffer.reset(new uint8_t[stats_size]);
                if (stats_buffer) {
                    // Copy fuzzer data into the heap buffer
                    std::vector<uint8_t> temp_data = fdp.ConsumeBytes<uint8_t>(stats_size);
                    std::copy(temp_data.begin(), temp_data.end(), stats_buffer.get());
                    cfg.rc_twopass_stats_in.buf = stats_buffer.get();
                    cfg.rc_twopass_stats_in.sz = stats_size;
                }
                
                // Set other two-pass related parameters
                cfg.rc_end_usage = VPX_VBR;
                cfg.rc_2pass_vbr_bias_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
                cfg.rc_2pass_vbr_corpus_complexity = fdp.ConsumeIntegralInRange<unsigned int>(0, 10000);
                
                // Test SVC layers with two-pass (validation checks this)
                cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, VPX_SS_MAX_LAYERS);
                cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, VPX_TS_MAX_LAYERS);
                
                break;
            }
            
            case 1: { // Resize-enabled encoding with scaled dimensions
                // Enable resize
                cfg.rc_resize_allowed = 1;
                
                // Set original dimensions
                cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(64, 1920);
                cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(64, 1080);
                
                // Set scaled dimensions (must be <= original dimensions)
                cfg.rc_scaled_width = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.g_w);
                cfg.rc_scaled_height = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.g_h);
                
                // Set resize thresholds
                cfg.rc_resize_up_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
                cfg.rc_resize_down_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
                
                break;
            }
            
            case 2: { // Temporal and spatial layering with proper bitrate allocation
                // Configure temporal and spatial layering
                cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, VPX_TS_MAX_LAYERS);
                cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, VPX_SS_MAX_LAYERS);
                
                // Ensure total layers doesn't exceed maximum
                if (cfg.ts_number_layers * cfg.ss_number_layers > VPX_MAX_LAYERS) {
                    cfg.ts_number_layers = 1;
                    cfg.ss_number_layers = 1;
                }
                
                // Set layer target bitrates with increasing values (validation requires this)
                unsigned int total_layers = cfg.ts_number_layers * cfg.ss_number_layers;
                unsigned int base_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
                
                for (unsigned int layer = 0; layer < total_layers && layer < VPX_MAX_LAYERS; layer++) {
                    cfg.layer_target_bitrate[layer] = base_bitrate * (layer + 1);
                }
                
                // Set rate decimators for temporal layers (must be powers of 2)
                if (cfg.ts_number_layers > 1) {
                    cfg.ts_rate_decimator[cfg.ts_number_layers - 1] = 1;
                    for (unsigned int tl = cfg.ts_number_layers - 2; tl > 0; tl--) {
                        cfg.ts_rate_decimator[tl] = cfg.ts_rate_decimator[tl + 1] * 2;
                    }
                }
                
                break;
            }
            
            case 3: { // Auto keyframe mode with kf_min_dist restrictions
                // Configure auto keyframe mode
                cfg.kf_mode = VPX_KF_AUTO;
                
                // Set kf_max_dist
                cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
                
                // Test kf_min_dist restrictions (should be 0 or equal to kf_max_dist in auto mode)
                unsigned int kf_min_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
                if (kf_min_choice == 0) {
                    cfg.kf_min_dist = 0;  // Valid: 0 in auto mode
                } else if (kf_min_choice == 1) {
                    cfg.kf_min_dist = cfg.kf_max_dist;  // Valid: equal to max_dist
                } else {
                    // Invalid case to test validation
                    cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(1, cfg.kf_max_dist - 1);
                }
                
                break;
            }
            
            case 4: { // Golden frame interval validations
                // Set reasonable frame dimensions
                cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(64, 1920);
                cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(64, 1080);
                
                // Set lag in frames (will be validated against gf intervals)
                cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, MAX_LAG_BUFFERS);
                
                // Note: min_gf_interval and max_gf_interval are set via encoder controls
                // We'll test those in the control section after initialization
                
                break;
            }
            
            case 5: { // Lag-in-frames constraints for ARF groups
                // Set max golden frame interval
                unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(2, MAX_LAG_BUFFERS - 1);
                
                // Set lag in frames - test validation rules
                unsigned int lag_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
                if (lag_choice == 0) {
                    cfg.g_lag_in_frames = 0;  // Valid: low delay mode
                } else if (lag_choice == 1) {
                    cfg.g_lag_in_frames = max_gf_interval + 2;  // Valid: >= (max_gf_interval + 2)
                } else {
                    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(1, max_gf_interval + 1);  // Invalid
                }
                
                // Set reasonable dimensions
                cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(64, 1920);
                cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(64, 1080);
                
                break;
            }
            case 6: { // Various encoder control parameter ranges
                // Test various configuration parameters that have range validation
                cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
                cfg.g_bit_depth = static_cast<vpx_bit_depth_t>(
                    fdp.ConsumeIntegralInRange<unsigned int>(VPX_BITS_8, VPX_BITS_12));
                cfg.g_input_bit_depth = fdp.ConsumeIntegralInRange<unsigned int>(8, 12);
                
                // Test quantizer range validation
                cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
                cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.rc_max_quantizer);
                
                // Test thread count
                cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
                
                // Test target bitrate
                cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000000);
                
                break;
            }
        }
        
        // Common configuration for all scenarios
        cfg.g_timebase.num = fdp.ConsumeIntegralInRange<unsigned int>(1, 1000);
        cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(1, 1000000000);
        
        // 3. Call vpx_codec_enc_init_ver with the modified configuration
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        vpx_codec_err_t init_err = vpx_codec_enc_init_ver(&codec, encoder_iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        
        // 4. Use vpx_codec_control_ to set additional encoder features if needed
        if (init_err == VPX_CODEC_OK) {
            // Test various encoder controls with parameter validation
            
            // Test target level setting (various VP9 levels)
            int target_level = fdp.ConsumeIntegralInRange<int>(LEVEL_1, LEVEL_MAX);
            vpx_codec_control_(&codec, VP9E_SET_TARGET_LEVEL, target_level);
            
            // Test tile configuration
            unsigned int tile_columns = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
            unsigned int tile_rows = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
            vpx_codec_control_(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
            vpx_codec_control_(&codec, VP9E_SET_TILE_ROWS, tile_rows);
            
            // Test CPU used
            int cpu_used = fdp.ConsumeIntegralInRange<int>(-9, 9);
            vpx_codec_control_(&codec, VP8E_SET_CPUUSED, cpu_used);
            
            // Test noise sensitivity
            unsigned int noise_sensitivity = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
            vpx_codec_control_(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
            
            // Test golden frame intervals (min_gf_interval, max_gf_interval)
            unsigned int min_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(0, MAX_LAG_BUFFERS - 1);
            unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(0, MAX_LAG_BUFFERS - 1);
            vpx_codec_control_(&codec, VP9E_SET_MIN_GF_INTERVAL, min_gf_interval);
            vpx_codec_control_(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
            
            // Test color space/range
            int color_space = fdp.ConsumeIntegralInRange<int>(VPX_CS_UNKNOWN, VPX_CS_SRGB);
            int color_range = fdp.ConsumeIntegralInRange<int>(VPX_CR_STUDIO_RANGE, VPX_CR_FULL_RANGE);
            vpx_codec_control_(&codec, VP9E_SET_COLOR_SPACE, color_space);
            vpx_codec_control_(&codec, VP9E_SET_COLOR_RANGE, color_range);
            
            // Test ARNR (altref noise reduction) parameters
            unsigned int arnr_max_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 15);
            unsigned int arnr_strength = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
            vpx_codec_control_(&codec, VP8E_SET_ARNR_MAXFRAMES, arnr_max_frames);
            vpx_codec_control_(&codec, VP8E_SET_ARNR_STRENGTH, arnr_strength);
            
            // Test CQ level
            unsigned int cq_level = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
            vpx_codec_control_(&codec, VP8E_SET_CQ_LEVEL, cq_level);
            
            // Test auto alt-ref
            unsigned int enable_auto_alt_ref = fdp.ConsumeIntegralInRange<unsigned int>(0, MAX_ARF_LAYERS);
            vpx_codec_control_(&codec, VP8E_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
            
            // 5. Call vpx_codec_encode with minimal test
            if (fdp.remaining_bytes() > 50) {
                // Simple encode call to test API (without actual image data)
                vpx_codec_encode(&codec, nullptr, 0, 1, 0, VPX_DL_REALTIME);
                
                // 6. Call vpx_codec_get_cx_data to retrieve encoded packets
                const vpx_codec_cx_pkt_t *pkt;
                vpx_codec_iter_t iter = nullptr;
                
                while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                    // Just consume the packets to test the API
                }
            }
            
            // 7. Call vpx_codec_destroy to clean up encoder resources
            vpx_codec_destroy(&codec);
        }
        
        // Reset two-pass stats pointer to avoid dangling pointer
        cfg.rc_twopass_stats_in.buf = nullptr;
        cfg.rc_twopass_stats_in.sz = 0;
    }
    
    return 0;
}

} // namespace harness_010
