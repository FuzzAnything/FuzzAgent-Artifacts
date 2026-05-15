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

namespace harness_009 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation with edge cases
 * Target APIs: validate_config function (internal), vpx_codec_enc_init, vpx_codec_enc_config_default,
 *              vpx_codec_enc_config_set, vpx_codec_destroy, vpx_codec_control with various VP9E_SET controls
 * Focus: Edge case encoder configurations to maximize coverage of validate_config function which has
 *        227 blocked branches. Specifically targets configuration validation paths, range checking,
 *        and invalid parameter handling for VP9 encoder.
 * Semantic differentiation: This harness uniquely focuses on encoder configuration validation
 * rather than encoder operation, output control, error handling, or image scaling.
 * Previous harnesses covered: decoder APIs (000), encoder APIs (001), multi-encoder (002),
 * decoder advanced features (003), image manipulation (004), error handling (005),
 * advanced encoder control (006), encoder output control (007), vpx_scale module (008).
 * 
 * Strategy: Generate diverse edge case configurations to exercise all validation paths
 * in the validate_config function including:
 * - Dimension validation (g_w, g_h ranges)
 * - Timebase validation
 * - Quantizer range validation
 * - Profile validation
 * - Thread count validation
 * - Lag-in-frames validation
 * - Rate control validation
 * - Keyframe configuration validation
 * - SVC layer validation
 * - Bit depth validation
 * - Tile configuration validation
 * - And many other validation checks
 */



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
    
    // Create multiple test configurations to exercise different validation paths
    // We'll test up to 5 different configurations from the same fuzzer input
    unsigned int num_configs = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
    
    for (unsigned int config_idx = 0; config_idx < num_configs && fdp.remaining_bytes() > 100; config_idx++) {
        // Get default encoder configuration
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_err_t config_err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        
        if (config_err != VPX_CODEC_OK) {
            // If we can't get default config, skip this iteration
            continue;
        }
        
        // Override configuration with fuzzed edge case data
        // This is the core of testing validate_config
        
        // 1. Test dimension edge cases
        // validate_config checks: RANGE_CHECK(cfg, g_w, 1, 65536)
        // validate_config checks: RANGE_CHECK(cfg, g_h, 1, 65536)
        cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(0, 70000);  // Include invalid ranges
        cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(0, 70000);  // Include invalid ranges
        
        // 2. Test timebase edge cases
        // validate_config checks: RANGE_CHECK(cfg, g_timebase.den, 1, 1000000000)
        // validate_config checks: RANGE_CHECK(cfg, g_timebase.num, 1, 1000000000)
        cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(0, 2000000000);
        cfg.g_timebase.num = fdp.ConsumeIntegralInRange<unsigned int>(0, 2000000000);
        
        // 3. Test profile edge cases
        // validate_config checks: RANGE_CHECK_HI(cfg, g_profile, 3)
        cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
        
        // 4. Test quantizer edge cases
        // validate_config checks: RANGE_CHECK_HI(cfg, rc_max_quantizer, 63)
        // validate_config checks: RANGE_CHECK_HI(cfg, rc_min_quantizer, cfg->rc_max_quantizer)
        cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
        cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
        
        // 5. Test thread count edge cases
        // validate_config checks: RANGE_CHECK_HI(cfg, g_threads, MAX_NUM_THREADS)
        cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(0, 128);
        
        // 6. Test lag-in-frames edge cases
        // validate_config checks: RANGE_CHECK_HI(cfg, g_lag_in_frames, MAX_LAG_BUFFERS)
        cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
        
        // 7. Test rate control edge cases
        // validate_config checks: RANGE_CHECK(cfg, rc_end_usage, VPX_VBR, VPX_Q)
        cfg.rc_end_usage = static_cast<vpx_rc_mode>(
            fdp.ConsumeIntegralInRange<unsigned int>(VPX_VBR - 5, VPX_Q + 5));
        
        // 8. Test keyframe configuration edge cases
        // validate_config checks: RANGE_CHECK(cfg, kf_mode, VPX_KF_DISABLED, VPX_KF_AUTO)
        cfg.kf_mode = static_cast<vpx_kf_mode>(
            fdp.ConsumeIntegralInRange<unsigned int>(VPX_KF_DISABLED - 5, VPX_KF_AUTO + 5));
        cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 10000);
        cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 10000);
        
        // 9. Test resize configuration edge cases
        // validate_config checks multiple resize-related validations
        cfg.rc_resize_allowed = fdp.ConsumeBool() ? 1 : 0;
        cfg.rc_scaled_width = fdp.ConsumeIntegralInRange<unsigned int>(0, 70000);
        cfg.rc_scaled_height = fdp.ConsumeIntegralInRange<unsigned int>(0, 70000);
        
        // 10. Test SVC layer configuration edge cases
        // validate_config checks: RANGE_CHECK(cfg, ss_number_layers, 1, VPX_SS_MAX_LAYERS)
        // validate_config checks: RANGE_CHECK(cfg, ts_number_layers, 1, VPX_TS_MAX_LAYERS)
        cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(0, 20);
        cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(0, 20);
        
        // 11. Test bit depth edge cases
        // validate_config checks: RANGE_CHECK(cfg, g_bit_depth, VPX_BITS_8, VPX_BITS_12)
        cfg.g_bit_depth = static_cast<vpx_bit_depth_t>(
            fdp.ConsumeIntegralInRange<unsigned int>(VPX_BITS_8 - 1, VPX_BITS_12 + 1));
        cfg.g_input_bit_depth = fdp.ConsumeIntegralInRange<unsigned int>(0, 20);
        
        // 12. Test two-pass statistics edge cases
        // validate_config has extensive two-pass stats validation
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
            size_t stats_size = fdp.ConsumeIntegralInRange<size_t>(0, 1000);
            std::vector<uint8_t> stats_data = fdp.ConsumeBytes<uint8_t>(stats_size);
            cfg.rc_twopass_stats_in.buf = stats_data.data();
            cfg.rc_twopass_stats_in.sz = stats_data.size();
        } else {
            cfg.rc_twopass_stats_in.buf = nullptr;
            cfg.rc_twopass_stats_in.sz = 0;
        }
        
        // 13. Test various other configuration parameters
        cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000000);
        cfg.rc_undershoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 200);
        cfg.rc_overshoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 200);
        cfg.rc_2pass_vbr_bias_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 200);
        cfg.rc_2pass_vbr_corpus_complexity = fdp.ConsumeIntegralInRange<unsigned int>(0, 20000);
        cfg.rc_dropframe_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 200);
        cfg.rc_resize_up_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 200);
        cfg.rc_resize_down_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 200);
        
        // 14. Test g_pass edge cases
        // validate_config checks g_pass range based on CONFIG_REALTIME_ONLY
        cfg.g_pass = static_cast<vpx_enc_pass>(
            fdp.ConsumeIntegralInRange<unsigned int>(VPX_RC_ONE_PASS - 1, VPX_RC_LAST_PASS + 1));
        
        // Try to initialize encoder with this edge case configuration
        // This will trigger validate_config internally
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        // Test initialization with minimal flags to avoid extra validation complexity
        vpx_codec_err_t init_err = vpx_codec_enc_init(&codec, encoder_iface, &cfg, flags);
        
        // Regardless of initialization success, we've exercised validate_config
        // Clean up if initialization succeeded
        if (init_err == VPX_CODEC_OK) {
            // Test a few encoder control APIs with edge case values
            // These may also trigger additional validation
            
            // Test CPU used control (range: -9 to 9)
            int cpu_used = fdp.ConsumeIntegralInRange<int>(-15, 15);
            vpx_codec_control_(&codec, VP8E_SET_CPUUSED, cpu_used);
            
            // Test noise sensitivity control (range: 0-6)
            unsigned int noise_sensitivity = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
            vpx_codec_control_(&codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
            
            // Test tile configuration (columns: 0-6, rows: 0-2)
            unsigned int tile_columns = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
            unsigned int tile_rows = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
            vpx_codec_control_(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
            vpx_codec_control_(&codec, VP9E_SET_TILE_ROWS, tile_rows);
            
            // Test sharpness control (range: 0-7) - Use VP8 control for VP9
            unsigned int sharpness = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
            vpx_codec_control_(&codec, VP8E_SET_SHARPNESS, sharpness);
            
            // Test ARNR configuration - Use VP8 controls for VP9
            unsigned int arnr_max_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 20);
            unsigned int arnr_strength = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
            vpx_codec_control_(&codec, VP8E_SET_ARNR_MAXFRAMES, arnr_max_frames);
            vpx_codec_control_(&codec, VP8E_SET_ARNR_STRENGTH, arnr_strength);
            
            // Test CQ level (range: 0-63) - Use VP8 control for VP9
            unsigned int cq_level = fdp.ConsumeIntegralInRange<unsigned int>(0, 100);
            vpx_codec_control_(&codec, VP8E_SET_CQ_LEVEL, cq_level);
            
            // Test color space/range (edge cases) - These are VP9-specific
            int color_space = fdp.ConsumeIntegralInRange<int>(0, 10);
            int color_range = fdp.ConsumeIntegralInRange<int>(0, 10);
            vpx_codec_control_(&codec, VP9E_SET_COLOR_SPACE, color_space);
            vpx_codec_control_(&codec, VP9E_SET_COLOR_RANGE, color_range);
            
            // Clean up encoder
            vpx_codec_destroy(&codec);
        }
        
        // Reset two-pass stats pointer to avoid dangling pointer
        cfg.rc_twopass_stats_in.buf = nullptr;
        cfg.rc_twopass_stats_in.sz = 0;
    }
    
    return 0;
}

} // namespace harness_009
