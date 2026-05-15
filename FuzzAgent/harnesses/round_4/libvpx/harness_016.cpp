#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_016 {
/*
 * Fuzzing harness for libvpx validate_config function with 303 blocked branches (42% blocked)
 * Target: validate_config function in vp9_cx_iface.c - specifically targeting encoder configuration
 *         validation error paths that remain uncovered (303 blocked branches)
 * Primary goal: Exercise encoder configuration validation error paths by testing invalid 
 *               configuration parameters to unlock the 303 blocked branches in validate_config
 * Specific invalid parameter tests:
 *   1. Invalid g_pass values (outside VPX_RC_ONE_PASS..VPX_RC_LAST_PASS range)
 *   2. Invalid gf_interval relationships (min_gf_interval > max_gf_interval)
 *   3. Invalid lag_in_frames relative to max_gf_interval (g_lag_in_frames < max_gf_interval + 2)
 *   4. Invalid spatial/temporal layer configurations (ss_layers * ts_layers > VPX_MAX_LAYERS)
 *   5. Invalid target_level values (outside valid enum values)
 *   6. Invalid rate control parameters (quantizer ranges, undershoot/overshoot percentages)
 * APIs: vpx_codec_enc_init_ver, vpx_codec_enc_config_default, vpx_codec_enc_config_set (primary),
 *       vpx_codec_control (with specific control IDs), vpx_codec_destroy
 * Specific control IDs to test (via vpx_codec_control):
 *   - VP8E_SET_CPUUSED (tests cpu_used range validation)
 *   - VP9E_SET_TILE_COLUMNS (tests tile_columns range)
 *   - VP9E_SET_AQ_MODE (tests aq_mode range)
 *   - VP8E_SET_ENABLEAUTOALTREF (tests enable_auto_alt_ref range)
 * Semantic diversity: This harness specifically targets the exact validation error paths
 *                     identified as blockers in validate_config (303 blocked branches),
 *                     with systematic testing of each validation condition using invalid
 *                     parameter values. Unlike harness_009 which tests a broader range
 *                     of edge cases, this harness focuses specifically on the validation
 *                     conditions that remain blocked according to coverage analysis.
 * Coverage goal: Unlock 300+ blocked branches in validate_config by systematically testing
 *                validation error paths with precisely invalid parameter combinations
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for comprehensive configuration testing
  if (size < 256) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Choose which specific validation test to run (0-7 for 8 test cases)
  unsigned int test_case = fdp.ConsumeIntegralInRange<unsigned int>(0, 7);
  
  // Step 2: Get VP9 encoder interface (primary target from guidance)
  vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
  if (iface == NULL) {
    // Fall back to VP8 encoder
    iface = vpx_codec_vp8_cx();
    if (iface == NULL) {
      return 0;
    }
  }

  // Step 3: Get default encoder configuration
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
  if (err != VPX_CODEC_OK) {
    return 0;
  }

  // Step 4: Configure basic parameters with valid ranges initially
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 192);
  
  // Ensure even dimensions for YUV formats
  if (width % 2) width++;
  if (height % 2) height++;
  
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
  cfg.g_lag_in_frames = 0; // Start with valid value, will be mutated in test cases

  // Initialize encoder with valid config first
  vpx_codec_ctx_t codec;
  vpx_codec_flags_t flags = 0;
  
  err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    // Try without flags if initialization failed
    flags = 0;
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
      return 0;
    }
  }

  // Step 5: Apply specific validation test cases targeting blocked branches
  switch (test_case) {
    case 0:  // Test 1: Invalid g_pass values (outside VPX_RC_ONE_PASS..VPX_RC_LAST_PASS)
      {
        // From validate_config lines 218-221:
        // #if CONFIG_REALTIME_ONLY
        //   RANGE_CHECK(cfg, g_pass, VPX_RC_ONE_PASS, VPX_RC_ONE_PASS);
        // #else
        //   RANGE_CHECK(cfg, g_pass, VPX_RC_ONE_PASS, VPX_RC_LAST_PASS);
        // #endif
        // Valid enum values: VPX_RC_ONE_PASS=0, VPX_RC_FIRST_PASS=1, VPX_RC_LAST_PASS=2
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Set g_pass to invalid value outside valid range
        // Use values 3-255 for invalid enum values
        unsigned int invalid_g_pass = fdp.ConsumeIntegralInRange<unsigned int>(3, 255);
        invalid_cfg.g_pass = static_cast<vpx_enc_pass>(invalid_g_pass);
        
        // Try to set invalid configuration to trigger validation error
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
      }
      break;

    case 1:  // Test 2: Invalid gf_interval relationships (min_gf_interval > max_gf_interval)
      {
        // From validate_config lines 227-230:
        // if (extra_cfg->min_gf_interval > 0 && extra_cfg->max_gf_interval > 0) {
        //   RANGE_CHECK(extra_cfg, max_gf_interval, extra_cfg->min_gf_interval,
        //               (MAX_LAG_BUFFERS - 1));
        // }
        
        // Need to use vpx_codec_control to set min_gf_interval and max_gf_interval
        // Use control API to set invalid relationship directly
        // Note: VP9E_SET_MIN_GF_INTERVAL and VP9E_SET_MAX_GF_INTERVAL are for VP9
        // Generate invalid interval relationship: min > max
        unsigned int min_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(4, 10);
        unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(1, min_gf_interval - 1);
        
        // Use control API to set invalid relationship
        // Note: VP9E_SET_MIN_GF_INTERVAL and VP9E_SET_MAX_GF_INTERVAL are for VP9
        if (iface == vpx_codec_vp9_cx()) {
          // Set min_gf_interval > max_gf_interval
          vpx_codec_control(&codec, VP9E_SET_MIN_GF_INTERVAL, min_gf_interval);
          vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
          
          // Try to encode to trigger validation
          vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
        }
      }
      break;

    case 2:  // Test 3: Invalid lag_in_frames relative to max_gf_interval
      {
        // From validate_config lines 234-237:
        // if (cfg->g_lag_in_frames > 0 && extra_cfg->max_gf_interval > 0 &&
        //     cfg->g_lag_in_frames < extra_cfg->max_gf_interval + 2) {
        //   ERROR("Set lag in frames to 0 (low delay) or >= (max-gf-interval + 2)");
        // }
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Set lag_in_frames > 0 but less than max_gf_interval + 2
        unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(2, 10);
        unsigned int lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(1, max_gf_interval + 1);
        
        invalid_cfg.g_lag_in_frames = lag_in_frames;
        
        // First set the configuration
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
        
        // Then try to set max_gf_interval through control API
        if (iface == vpx_codec_vp9_cx()) {
          vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
          
          // Try to reconfigure to trigger validation
          vpx_codec_enc_config_set(&codec, &invalid_cfg);
        }
      }
      break;

    case 3:  // Test 4: Invalid spatial/temporal layer configurations
      {
        // From validate_config lines 258-259:
        // if (cfg->ss_number_layers * cfg->ts_number_layers > VPX_MAX_LAYERS)
        //   ERROR("ss_number_layers * ts_number_layers is out of range");
        // VPX_MAX_LAYERS = 12 (from vpx_encoder.h)
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Set layer product > VPX_MAX_LAYERS (which is 12)
        unsigned int spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(4, 8);
        unsigned int temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(4, 8);
        
        // Ensure product > 12
        if (spatial_layers * temporal_layers > 12) {
          invalid_cfg.ss_number_layers = spatial_layers;
          invalid_cfg.ts_number_layers = temporal_layers;
          
          // Try to set invalid configuration
          vpx_codec_enc_config_set(&codec, &invalid_cfg);
        }
      }
      break;

    case 4:  // Test 5: Invalid target_level values
      {
        // From validate_config lines 248-256:
        // Checks for specific enum values: LEVEL_1, LEVEL_1_1, LEVEL_2, LEVEL_2_1, 
        // LEVEL_3, LEVEL_3_1, LEVEL_4, LEVEL_4_1, LEVEL_5, LEVEL_5_1, LEVEL_5_2,
        // LEVEL_6, LEVEL_6_1, LEVEL_6_2, LEVEL_UNKNOWN, LEVEL_AUTO, LEVEL_MAX
        
        // Set invalid target_level value
        // Valid values are specific enum values, so generate random values outside that set
        unsigned int invalid_target_level = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
        
        // Use vpx_codec_control to set target level
        // VP9E_SET_TARGET_LEVEL is for VP9
        if (iface == vpx_codec_vp9_cx()) {
          vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, invalid_target_level);
          
          // Try to encode to trigger validation
          vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
        }
      }
      break;

    case 5:  // Test 6: Invalid rate control parameters
      {
        // Test various rate control parameter validations:
        // - rc_max_quantizer > 63 (line 198)
        // - rc_min_quantizer > rc_max_quantizer (line 199)
        // - rc_undershoot_pct > 100 (line 208)
        // - rc_overshoot_pct > 100 (line 209)
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Generate invalid rate control parameters
        unsigned int invalid_param = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
        
        switch (invalid_param) {
          case 0:
            // rc_max_quantizer > 63
            invalid_cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(64, 255);
            break;
          case 1:
            // rc_min_quantizer > rc_max_quantizer
            invalid_cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(10, 40);
            invalid_cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(invalid_cfg.rc_max_quantizer + 1, 63);
            break;
          case 2:
            // rc_undershoot_pct > 100
            invalid_cfg.rc_undershoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(101, 200);
            break;
          case 3:
            // rc_overshoot_pct > 100
            invalid_cfg.rc_overshoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(101, 200);
            break;
        }
        
        // Try to set invalid configuration
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
      }
      break;

    case 6:  // Test 7: vpx_codec_control API with specific control IDs from guidance
      {
        // Test vpx_codec_control API with control IDs mentioned in guidance:
        // - VP8E_SET_CPUUSED (tests cpu_used range validation)
        // - VP9E_SET_TILE_COLUMNS (tests tile_columns range) 
        // - VP9E_SET_AQ_MODE (tests aq_mode range)
        // - VP8E_SET_ENABLEAUTOALTREF (tests enable_auto_alt_ref range)
        
        unsigned int control_test = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
        
        switch (control_test) {
          case 0: {
            // VP8E_SET_CPUUSED - test with invalid values
            // Valid range for VP9: -9..9, for VP8: -16..16
            int invalid_cpu_used = fdp.ConsumeIntegralInRange<int>(-50, 50);  // Include invalid values
            vpx_codec_control(&codec, VP8E_SET_CPUUSED, invalid_cpu_used);
            break;
          }
            
          case 1: {
            // VP9E_SET_TILE_COLUMNS - test with invalid values
            // Valid range: 0..6 (log2 values)
            if (iface == vpx_codec_vp9_cx()) {
              unsigned int invalid_tile_columns = fdp.ConsumeIntegralInRange<unsigned int>(7, 20);
              vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, invalid_tile_columns);
            }
            break;
          }
            
          case 2: {
            // VP9E_SET_AQ_MODE - test with invalid values
            // From validate_config line 202: RANGE_CHECK(extra_cfg, aq_mode, 0, AQ_MODE_COUNT - 2);
            if (iface == vpx_codec_vp9_cx()) {
              unsigned int invalid_aq_mode = fdp.ConsumeIntegralInRange<unsigned int>(10, 100);
              vpx_codec_control(&codec, VP9E_SET_AQ_MODE, invalid_aq_mode);
            }
            break;
          }
            
          case 3: {
            // VP8E_SET_ENABLEAUTOALTREF - test with invalid values
            // Valid range for VP8: 0..1, for VP9: 0..6
            unsigned int invalid_enable_autoaltref = fdp.ConsumeIntegralInRange<unsigned int>(10, 100);
            vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, invalid_enable_autoaltref);
            break;
          }
        }
        
        // Try to encode after setting invalid controls
        vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
      }
      break;

    case 7:  // Test 8: Complex validation with multiple invalid parameters
      {
        // Combine multiple validation errors to test complex validation paths
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Set multiple invalid parameters simultaneously
        invalid_cfg.g_pass = static_cast<vpx_enc_pass>(fdp.ConsumeIntegralInRange<unsigned int>(3, 10));  // Invalid g_pass
        invalid_cfg.rc_max_quantizer = 70;  // > 63
        invalid_cfg.rc_min_quantizer = 80;  // > rc_max_quantizer
        
        // Set invalid spatial/temporal layers
        invalid_cfg.ss_number_layers = 4;
        invalid_cfg.ts_number_layers = 4;  // Product = 16 > VPX_MAX_LAYERS (12)
        
        // Set invalid lag_in_frames for potential gf_interval validation
        invalid_cfg.g_lag_in_frames = 1;
        
        // Try to set the configuration with multiple violations
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
        
        // Also test invalid control values
        if (iface == vpx_codec_vp9_cx()) {
          vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, 999);  // Invalid target level
        }
      }
      break;
  }

  // Step 6: Clean up
  vpx_codec_destroy(&codec);

  return 0;
}

} // namespace harness_016
