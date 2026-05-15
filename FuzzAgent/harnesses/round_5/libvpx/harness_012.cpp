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
#include "vpx/vpx_image.h"

namespace harness_012 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation targeting
 * comprehensive parameter space exploration to trigger all 323 blocked branches
 * in validate_config function (highest blocked complexity in project).
 * 
 * Target APIs: vpx_codec_enc_init_ver (Primary Target - encoder initialization with diverse configs),
 *              vpx_codec_enc_config_default (Required Helper: get default configuration),
 *              vpx_codec_destroy (Required Helper: cleanup),
 *              vpx_img_alloc (Required Helper: create input image buffers),
 *              vpx_codec_vp9_cx (Required: VP9 encoder interface constant)
 * 
 * Focus: Systematic exploration of encoder configuration parameter space to trigger
 * validation branches identified as critical coverage gaps:
 * 1. All g_pass mode combinations (VPX_RC_ONE_PASS, VPX_RC_FIRST_PASS, VPX_RC_LAST_PASS)
 * 2. g_lag_in_frames ranges (0-25) with special ARF group validation
 * 3. All rc_end_usage modes (VPX_VBR, VPX_CBR, VPX_CQ, VPX_Q)
 * 4. kf_mode combinations (VPX_KF_DISABLED, VPX_KF_AUTO) with kf_min_dist/kf_max_dist constraints
 * 5. Spatial and temporal layering combinations with bitrate allocation validation
 * 6. Profile-bit-depth compatibility validations
 * 7. Resize configuration validation paths
 * 8. Two-pass stats validation for VPX_RC_LAST_PASS mode
 * 9. Extra configuration parameters (cpu_used, enable_auto_alt_ref, tile_columns, etc.)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_010.cpp: Focuses on specific validation path combinations and sequences
 * - harness_012.cpp: Comprehensive parameter space exploration targeting all 323 blocked branches
 *   in validate_config through systematic parameter mutation and boundary testing
 * 
 * Strategy: Use FuzzedDataProvider to consume structured input for all configuration parameters,
 * test parameter combinations that trigger specific validation branches, and ensure proper
 * error handling for invalid configurations.
 */



// Define constants from libvpx internal headers
#define MAX_LAG_BUFFERS 25  // From vp9/encoder/vp9_lookahead.h
#define MAX_ARF_LAYERS 6    // From vp9/encoder/vp9_firstpass.h
#define MAX_NUM_THREADS 64  // From vp9/encoder/vp9_encoder.h

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
    const size_t MIN_SIZE = 1024;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Use VP9 encoder for configuration validation testing
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        return 0;
    }
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t config_err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    
    if (config_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Test multiple configuration scenarios to explore different validation branches
    unsigned int num_scenarios = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
    
    for (unsigned int scenario_idx = 0; scenario_idx < num_scenarios && fdp.remaining_bytes() > 500; scenario_idx++) {
        // Reset to default config for each scenario
        config_err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        if (config_err != VPX_CODEC_OK) {
            continue;
        }
        
        // Consume configuration parameters from fuzzer input
        
        // 1. General settings (g_* parameters)
        cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(0, MAX_NUM_THREADS);
        cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);  // VP9 supports profiles 0-3
        cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(1, 65536);
        cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(1, 65536);
        
        // Bit-depth and input bit-depth (triggers profile-bit-depth validation)
        unsigned int bit_depth_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
        switch (bit_depth_choice) {
            case 0: cfg.g_bit_depth = VPX_BITS_8; cfg.g_input_bit_depth = 8; break;
            case 1: cfg.g_bit_depth = VPX_BITS_10; cfg.g_input_bit_depth = 10; break;
            case 2: cfg.g_bit_depth = VPX_BITS_12; cfg.g_input_bit_depth = 12; break;
            case 3: cfg.g_bit_depth = VPX_BITS_8; cfg.g_input_bit_depth = 10; break;  // Mismatch case
        }
        
        // Timebase - must have valid numerator/denominator
        cfg.g_timebase.num = fdp.ConsumeIntegralInRange<int>(1, 1000000000);
        cfg.g_timebase.den = fdp.ConsumeIntegralInRange<int>(1, 1000000000);
        
        // 2. Multi-pass encoding mode (g_pass) - critical validation path
        unsigned int pass_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        switch (pass_mode) {
            case 0: cfg.g_pass = VPX_RC_ONE_PASS; break;
            case 1: cfg.g_pass = VPX_RC_FIRST_PASS; break;
            case 2: cfg.g_pass = VPX_RC_LAST_PASS; break;
        }
        
        // 3. Lag-in-frames with ARF group validation
        cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, MAX_LAG_BUFFERS);
        
        // 4. Rate control configuration
        unsigned int rc_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
        switch (rc_mode) {
            case 0: cfg.rc_end_usage = VPX_VBR; break;
            case 1: cfg.rc_end_usage = VPX_CBR; break;
            case 2: cfg.rc_end_usage = VPX_CQ; break;
            case 3: cfg.rc_end_usage = VPX_Q; break;
        }
        
        cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(1, 100000);
        cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
        cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
        
        // Rate control thresholds
        cfg.rc_undershoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
        cfg.rc_overshoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
        
        // 5. Keyframe configuration
        unsigned int kf_mode_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        switch (kf_mode_choice) {
            case 0: cfg.kf_mode = VPX_KF_FIXED; break;
            case 1: cfg.kf_mode = VPX_KF_AUTO; break;
            case 2: cfg.kf_mode = VPX_KF_DISABLED; break;
        }
        
        cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
        cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(cfg.kf_min_dist, 1000);
        
        // 6. Spatial and temporal scalability
        cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
        cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
        
        // Test layer target bitrate allocation (triggers "ts_target_bitrate entries are not increasing")
        unsigned int total_layers = cfg.ss_number_layers * cfg.ts_number_layers;
        if (total_layers > VPX_MAX_LAYERS) {
            // Adjust to stay within limits for validation
            cfg.ss_number_layers = 2;
            cfg.ts_number_layers = 2;
            total_layers = 4;
        }
        
        // Initialize layer bitrates
        unsigned int base_bitrate = cfg.rc_target_bitrate;
        for (unsigned int i = 0; i < total_layers; i++) {
            // Create increasing or non-increasing pattern based on fuzzer input
            if (fdp.ConsumeBool()) {
                cfg.layer_target_bitrate[i] = base_bitrate + (i * 100);
            } else {
                cfg.layer_target_bitrate[i] = base_bitrate - (i * 100);  // Potentially invalid
            }
        }
        
        // Temporal rate decimators (must be powers of 2)
        for (unsigned int i = 0; i < cfg.ts_number_layers; i++) {
            unsigned int decimator_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
            switch (decimator_choice) {
                case 0: cfg.ts_rate_decimator[i] = 1; break;
                case 1: cfg.ts_rate_decimator[i] = 2; break;
                case 2: cfg.ts_rate_decimator[i] = 4; break;
                case 3: cfg.ts_rate_decimator[i] = 8; break;  // Potentially invalid for validation
            }
        }
        
        // 7. Resize configuration
        cfg.rc_resize_allowed = fdp.ConsumeBool() ? 1 : 0;
        if (cfg.rc_resize_allowed) {
            cfg.rc_scaled_width = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.g_w);
            cfg.rc_scaled_height = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.g_h);
            cfg.rc_resize_up_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
            cfg.rc_resize_down_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
        }
        
        cfg.rc_dropframe_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
        
        // 8. Two-pass stats for LAST_PASS mode
        std::unique_ptr<uint8_t[]> stats_buffer;
        if (cfg.g_pass == VPX_RC_LAST_PASS) {
            // Create mock two-pass stats data
            size_t stats_size = fdp.ConsumeIntegralInRange<size_t>(sizeof(FIRSTPASS_STATS) * 2, 
                                                                   sizeof(FIRSTPASS_STATS) * 50);
            if (stats_size > fdp.remaining_bytes()) {
                stats_size = fdp.remaining_bytes();
            }
            
            if (stats_size > 0) {
                stats_buffer.reset(new uint8_t[stats_size]);
                if (stats_buffer) {
                    std::vector<uint8_t> temp_data = fdp.ConsumeBytes<uint8_t>(stats_size);
                    std::copy(temp_data.begin(), temp_data.end(), stats_buffer.get());
                    cfg.rc_twopass_stats_in.buf = stats_buffer.get();
                    cfg.rc_twopass_stats_in.sz = stats_size;
                }
            }
            
            cfg.rc_2pass_vbr_bias_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
            cfg.rc_2pass_vbr_corpus_complexity = fdp.ConsumeIntegralInRange<unsigned int>(0, 10000);
        }
        
        // 9. Create a simple image for encoder input
        vpx_image_t* img = nullptr;
        if (fdp.remaining_bytes() > 100) {
            unsigned int fmt = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
            vpx_img_fmt_t img_fmt;
            switch (fmt) {
                case 0: img_fmt = VPX_IMG_FMT_I420; break;
                case 1: img_fmt = VPX_IMG_FMT_I422; break;
                case 2: img_fmt = VPX_IMG_FMT_I444; break;
                default: img_fmt = VPX_IMG_FMT_I420; break;
            }
            
            unsigned int align = 1 << fdp.ConsumeIntegralInRange<unsigned int>(0, 5);
            
            img = vpx_img_alloc(nullptr, img_fmt, cfg.g_w, cfg.g_h, align);
        }
        
        // 10. Initialize encoder with mutated configuration
        vpx_codec_ctx_t codec;
        vpx_codec_err_t init_result;
        
        // Try initialization with different flags
        vpx_codec_flags_t flags = 0;
        if (fdp.ConsumeBool()) {
            flags |= VPX_CODEC_USE_PSNR;
        }
        if (fdp.ConsumeBool()) {
            flags |= VPX_CODEC_USE_HIGHBITDEPTH;
        }
        
        init_result = vpx_codec_enc_init_ver(&codec, encoder_iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        
        // Clean up resources
        if (init_result == VPX_CODEC_OK) {
            vpx_codec_destroy(&codec);
        }
        
        if (img) {
            vpx_img_free(img);
        }
        
        // Consume some bytes to advance fuzzer state even if initialization failed
        if (fdp.remaining_bytes() > 100) {
            fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, 100));
        }
    }
    
    return 0;
}

} // namespace harness_012
