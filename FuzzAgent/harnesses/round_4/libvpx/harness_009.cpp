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

namespace harness_009 {
/*
 * Fuzzing harness for libvpx encoder configuration validation edge cases
 * Targets: validate_config function in vp9_cx_iface.c (272 blocked branches)
 * APIs: vpx_codec_enc_init_ver, vpx_codec_enc_config_default, vpx_codec_enc_config_set (primary),
 *       vpx_codec_encode, vpx_codec_destroy, vpx_codec_control_
 * Specific validation edge cases to test:
 *   1. Invalid width/height combinations (0, 65537, negative via overflow)
 *   2. Invalid timebase values (0 denominator/numerator)
 *   3. Out-of-range quantizer values (min > max, > 63)
 *   4. Invalid lag_in_frames vs max_gf_interval relationships (g_lag_in_frames < max_gf_interval + 2)
 *   5. Layer configuration inconsistencies (ss_layers * ts_layers > VPX_MAX_LAYERS)
 *   6. Invalid target_level values
 *   7. Bitrate ordering violations for temporal layers
 *   8. vpx_codec_control_ API with VP8E_SET_CPUUSED, VP8E_SET_ENABLEAUTOALTREF, etc.
 * Semantic diversity: This harness comprehensively tests all validation paths in validate_config
 *                     using vpx_codec_control API to modify configuration parameters, targeting
 *                     the 272 blocked branches not covered by previous harnesses
 *                     (vs. harness_008: limited edge cases, harness_007: complex encoding scenarios)
 * Coverage goal: Unlock 272 blocked branches in validate_config by systematically testing
 *                all validation error paths with invalid parameter combinations
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for comprehensive configuration testing
  if (size < 512) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Choose which validation test case to run
  unsigned int test_case = fdp.ConsumeIntegralInRange<unsigned int>(0, 9);
  
  // Step 2: Get VP9 encoder interface (primary target from guidance)
  vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
  if (iface == NULL) {
    // Also test VP8 encoder as mentioned in guidance
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
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
  
  // Ensure even dimensions for YUV formats
  if (width % 2) width++;
  if (height % 2) height++;
  
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(1, 120);
  cfg.g_pass = VPX_RC_ONE_PASS;
  cfg.g_lag_in_frames = 0;

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

  // Step 5: Apply specific validation test cases using vpx_codec_control API
  switch (test_case) {
    case 0:  // Test 1: Invalid width/height combinations
      {
        // Test width/height validation (lines 192-193 in validate_config)
        // RANGE_CHECK(cfg, g_w, 1, 65536);
        // RANGE_CHECK(cfg, g_h, 1, 65536);
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Test various invalid width/height values
        // Test various invalid width/height values
        unsigned int invalid_dim = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        switch (invalid_dim) {
          case 0: {
            invalid_cfg.g_w = 0;  // Below minimum (1)
            break;
          }
          case 1: {
            invalid_cfg.g_w = 65537;  // Above maximum (65536)
            break;
          }
          case 2: {
            invalid_cfg.g_h = 0;  // Below minimum (1)
            break;
          }
        }
        // Try to set invalid configuration
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
      }
      break;
    case 1:  // Test 2: Invalid timebase values
      {
        // Test timebase validation (lines 194-195 in validate_config)
        // RANGE_CHECK(cfg, g_timebase.den, 1, 1000000000);
        // RANGE_CHECK(cfg, g_timebase.num, 1, 1000000000);
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Set invalid timebase (0 denominator or numerator)
        if (fdp.ConsumeBool()) {
          invalid_cfg.g_timebase.den = 0;  // Invalid: denominator cannot be 0
        } else {
          invalid_cfg.g_timebase.num = 0;  // Invalid: numerator cannot be 0
        }
        
        // Try to set invalid configuration
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
      }
      break;

    case 2:  // Test 3: Out-of-range quantizer values
      {
        // Test quantizer validation (lines 198-199 in validate_config)
        // RANGE_CHECK_HI(cfg, rc_max_quantizer, 63);
        // RANGE_CHECK_HI(cfg, rc_min_quantizer, cfg->rc_max_quantizer);
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Test various quantizer violations
        unsigned int quant_test = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        switch (quant_test) {
          case 0: {
            // max_quantizer > 63
            invalid_cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(64, 100);
            break;
          }
          case 1: {
            // min_quantizer > max_quantizer
            invalid_cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(40, 63);
            invalid_cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, invalid_cfg.rc_min_quantizer - 1);
            break;
          }
          case 2: {
            // Both invalid
            invalid_cfg.rc_max_quantizer = 100;
            invalid_cfg.rc_min_quantizer = 70;
            break;
          }
        }
        
        // Try to set invalid configuration
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
      }
      break;

    case 3:  // Test 4: Invalid lag_in_frames vs max_gf_interval relationships
      {
        // Test lag_in_frames validation (lines 234-237 in validate_config)
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
        // Note: VP9E_SET_MAX_GF_INTERVAL is for VP9, VP8 uses different controls
        if (iface == vpx_codec_vp9_cx()) {
          vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
          
          // Try to reconfigure to trigger validation
          vpx_codec_enc_config_set(&codec, &invalid_cfg);
        }
      }
      break;

    case 4:  // Test 5: Layer configuration inconsistencies
      {
        // Test layer validation (lines 258-259 in validate_config)
        // if (cfg->ss_number_layers * cfg->ts_number_layers > VPX_MAX_LAYERS)
        //   ERROR("ss_number_layers * ts_number_layers is out of range");
        
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

    case 5:  // Test 6: Invalid target_level values
      {
        // Test target_level validation (lines 248-256 in validate_config)
        // Checks for specific enum values
        
        // Set invalid target_level value
        // Valid values: LEVEL_1, LEVEL_1_1, LEVEL_2, LEVEL_2_1, LEVEL_3, LEVEL_3_1,
        //               LEVEL_4, LEVEL_4_1, LEVEL_5, LEVEL_5_1, LEVEL_5_2, LEVEL_6,
        //               LEVEL_6_1, LEVEL_6_2, LEVEL_UNKNOWN, LEVEL_AUTO, LEVEL_MAX
        unsigned int invalid_target_level = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
        
        // Use vpx_codec_control to set target level
        // VP9E_SET_TARGET_LEVEL is for VP9
        if (iface == vpx_codec_vp9_cx()) {
          vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, invalid_target_level);
          
          // Try to encode to trigger validation
          vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
          if (img != NULL) {
            // Fill image with some data
            size_t remaining = fdp.remaining_bytes();
            if (remaining > 0) {
              size_t fill_size = std::min(remaining, (size_t)(width * height * 3 / 2));
              std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(fill_size);
              if (img_data.size() >= width * height) {
                // Fill Y plane
                memcpy(img->planes[0], img_data.data(), width * height);
              }
            }
            
            vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
            vpx_img_free(img);
          }
        }
      }
      break;

    case 6:  // Test 7: Bitrate ordering violations for temporal layers
      {
        // Test temporal layer bitrate ordering (lines 265-267 in validate_config)
        // if (cfg->layer_target_bitrate[layer] <
        //     cfg->layer_target_bitrate[layer - 1])
        //   ERROR("ts_target_bitrate entries are not increasing");
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Set up temporal layers
        invalid_cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(2, 3);
        invalid_cfg.ss_number_layers = 1;
        
        // Set decreasing bitrates to violate ordering
        for (unsigned int tl = 0; tl < invalid_cfg.ts_number_layers; ++tl) {
          // Bitrates decreasing instead of increasing
          invalid_cfg.layer_target_bitrate[tl] = (invalid_cfg.ts_number_layers - tl) * 100;
        }
        
        // Try to set invalid configuration
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
      }
      break;

    case 7:  // Test 8: vpx_codec_control API with various controls
      {
        // Test vpx_codec_control API with different control IDs
        // This tests the control API paths that may not be reached
        
        unsigned int control_test = fdp.ConsumeIntegralInRange<unsigned int>(0, 5);
        
        switch (control_test) {
          case 0: {
            // VP8E_SET_CPUUSED (line 173 in vp8cx.h)
            // Valid range for VP9: -9..9, for VP8: -16..16
            if (iface == vpx_codec_vp9_cx()) {
              int cpu_used = fdp.ConsumeIntegralInRange<int>(-20, 20);  // Include invalid values
              vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
            } else {
              int cpu_used = fdp.ConsumeIntegralInRange<int>(-20, 20);
              vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
            }
            break;
          }
            
          case 1: {
            // VP8E_SET_ENABLEAUTOALTREF (line 182 in vp8cx.h)
            // Valid range for VP8: 0..1, for VP9: 0..6
            unsigned int enable_autoaltref = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);  // Include invalid
            vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, enable_autoaltref);
            break;
          }
            
          case 2: {
            // VP8E_SET_SHARPNESS (line 200 in vp8cx.h)
            // Valid range: 0..7
            unsigned int sharpness = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);  // Include invalid
            vpx_codec_control(&codec, VP8E_SET_SHARPNESS, sharpness);
            break;
          }
            
          case 3: {
            // VP8E_SET_STATIC_THRESHOLD (line 206 in vp8cx.h)
            unsigned int static_thresh = fdp.ConsumeIntegral<unsigned int>();
            vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, static_thresh);
            break;
          }
            
          case 4: {
            // VP8E_SET_ARNR_MAXFRAMES (line 235 in vp8cx.h)
            // Valid range: 0..15
            unsigned int arnr_maxframes = fdp.ConsumeIntegralInRange<unsigned int>(0, 20);  // Include invalid
            vpx_codec_control(&codec, VP8E_SET_ARNR_MAXFRAMES, arnr_maxframes);
            break;
          }
            
          case 5: {
            // VP8E_SET_ARNR_STRENGTH (line 241 in vp8cx.h)
            // Valid range: 0..6
            unsigned int arnr_strength = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);  // Include invalid
            vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, arnr_strength);
            break;
          }
        }
        // Try to encode after setting controls
        vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
      }
      break;

    case 8:  // Test 9: Complex validation with multiple violations
      {
        // Combine multiple validation errors to test complex paths
        
        vpx_codec_enc_cfg_t invalid_cfg = cfg;
        
        // Set multiple invalid parameters
        invalid_cfg.g_w = 0;  // Invalid width
        invalid_cfg.g_timebase.den = 0;  // Invalid timebase denominator
        invalid_cfg.rc_max_quantizer = 100;  // Invalid quantizer
        invalid_cfg.rc_min_quantizer = 80;   // min > max
        
        // Try to set the configuration with multiple violations
        vpx_codec_enc_config_set(&codec, &invalid_cfg);
      }
      break;

    case 9:  // Test 10: Test VP8 encoder configuration
      {
        // Test VP8 encoder configuration as mentioned in guidance
        if (iface == vpx_codec_vp9_cx()) {
          // Destroy current VP9 encoder
          vpx_codec_destroy(&codec);
          
          // Initialize VP8 encoder
          iface = vpx_codec_vp8_cx();
          err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
          if (err != VPX_CODEC_OK) {
            return 0;
          }
        }
        // Test VP8-specific controls
        unsigned int vp8_control = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        switch (vp8_control) {
          case 0: {
            // VP8E_SET_CPUUSED for VP8 (range -16..16)
            int cpu_used = fdp.ConsumeIntegralInRange<int>(-20, 20);
            vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
            break;
          }
            
          case 1: {
            // VP8E_SET_NOISE_SENSITIVITY (VP8 only)
            unsigned int noise_sensitivity = fdp.ConsumeIntegralInRange<unsigned int>(0, 10);
            vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);
            break;
          }
            
          case 2: {
            // VP8E_SET_TOKEN_PARTITIONS
            int token_partitions = fdp.ConsumeIntegralInRange<int>(0, 3);
            vpx_codec_control(&codec, VP8E_SET_TOKEN_PARTITIONS, token_partitions);
            break;
          }
        }
        
        // Try to encode
        vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
      }
      break;
  }

  // Step 6: Clean up
  vpx_codec_destroy(&codec);

  return 0;
}

} // namespace harness_009
