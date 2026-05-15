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

namespace harness_008 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation edge cases
 * Targets: validate_config function with 257 blocked branches out of 720
 * APIs: vpx_codec_enc_init_ver (primary), vpx_codec_enc_config_set (secondary), 
 *       vpx_codec_enc_config_default (helper), vpx_img_alloc (helper), 
 *       vpx_codec_destroy (helper), vpx_img_free (helper)
 * Specific edge cases to test:
 *   1. Set both min_gf_interval and max_gf_interval > 0 with max < min (trigger line 230-233)
 *   2. Set invalid target_level values
 *   3. Set ss_number_layers * ts_number_layers > VPX_MAX_LAYERS (12)
 *   4. Set g_pass to VPX_RC_LAST_PASS
 *   5. Set rc_resize_allowed = 1 with invalid rc_scaled_width/height
 *   6. Set g_lag_in_frames < max_gf_interval + 2 (trigger error at line 239)
 *   7. Set kf_mode != VPX_KF_DISABLED with kf_min_dist != kf_max_dist and kf_min_dist > 0
 * Semantic diversity: This harness specifically targets configuration validation edge cases
 *                     that should fail validation but exercise blocked branches in validate_config
 *                     (vs. harness_000: basic decoding, harness_001: single encoder encoding,
 *                      harness_002: multi-encoder, harness_003: error handling,
 *                      harness_004: image operations, harness_005: codec info queries,
 *                      harness_006: SVC configurations, harness_007: complex encoding scenarios)
 * Coverage goal: Target 257 blocked branches in validate_config by testing invalid parameter
 *                combinations that trigger specific validation error paths
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for complex configuration edge cases
  if (size < 256) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Consume encoder configuration parameters from fuzzer input
  
  // Image dimensions (limited to reasonable sizes for fuzzing)
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 320);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 240);
  
  // Ensure even dimensions for YUV formats
  if (width % 2) width++;
  if (height % 2) height++;
  
  // Bitrate and framerate settings
  unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
  unsigned int framerate_num = 1;
  unsigned int framerate_den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);

  
  // Step 2: Get VP9 encoder interface
  vpx_codec_iface_t* iface = vpx_codec_vp9_cx();
  if (iface == NULL) {
    return 0;
  }

  // Step 3: Get default encoder configuration
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
  if (err != VPX_CODEC_OK) {
    return 0;
  }

  // Step 4: Configure basic parameters
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.rc_target_bitrate = bitrate;
  cfg.g_timebase.num = framerate_num;
  cfg.g_timebase.den = framerate_den;
  cfg.g_pass = VPX_RC_ONE_PASS;  // Default, will be modified for edge cases
  
  // CRITICAL: Choose which edge case to test based on fuzzer input
  unsigned int edge_case = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
  
  // Step 5: Apply specific edge case configurations to trigger validation errors
  
  switch (edge_case) {
    case 0:  // Edge case 1: min_gf_interval and max_gf_interval > 0 with max < min
      {
        // Configure with both min and max > 0, and max < min to trigger validation at lines 230-233
        unsigned int min_gf = fdp.ConsumeIntegralInRange<unsigned int>(3, 10);
        unsigned int max_gf = fdp.ConsumeIntegralInRange<unsigned int>(1, min_gf - 1);
        
        // Create configuration with these values
        vpx_codec_enc_cfg_t test_cfg = cfg;
        
        // We need to test this through the encoder initialization
        // The validation happens in validate_config which checks extra_cfg->min_gf_interval
        // and extra_cfg->max_gf_interval. 
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        // First initialize with default config
        err = vpx_codec_enc_init_ver(&codec, iface, &test_cfg, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
          // Try to set invalid GF intervals through control interface
          // These controls modify the extra_cfg structure
          vpx_codec_control(&codec, VP9E_SET_MIN_GF_INTERVAL, min_gf);
          vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf);
          
          // Try to reconfigure to trigger validation with the new settings
          vpx_codec_enc_config_set(&codec, &test_cfg);
          
          vpx_codec_destroy(&codec);
        }
      }
      break;
      
    case 1:  // Edge case 2: invalid target_level values
      {
        // target_level values are defined in vpx/vpx_encoder.h
        // Valid values: LEVEL_1, LEVEL_1_1, LEVEL_2, LEVEL_2_1, LEVEL_3, LEVEL_3_1,
        //               LEVEL_4, LEVEL_4_1, LEVEL_5, LEVEL_5_1, LEVEL_5_2, LEVEL_6,
        //               LEVEL_6_1, LEVEL_6_2, LEVEL_UNKNOWN, LEVEL_AUTO, LEVEL_MAX
        // We'll set an invalid value to trigger the error at line 255
        
        unsigned int invalid_target_level = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
        
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
          // Try to set invalid target level
          vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, invalid_target_level);
          
          vpx_codec_destroy(&codec);
        }
      }
      break;
      
    case 2:  // Edge case 3: ss_number_layers * ts_number_layers > VPX_MAX_LAYERS
      {
        // VPX_MAX_LAYERS is 12, so we need product > 12
        // Try spatial layers * temporal layers > 12
        
        unsigned int spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(4, 6);
        unsigned int temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(4, 6);
        
        // Ensure product > 12
        if (spatial_layers * temporal_layers > 12) {
          cfg.ss_number_layers = spatial_layers;
          cfg.ts_number_layers = temporal_layers;
          
          // Try to initialize with invalid layer configuration
          vpx_codec_ctx_t codec;
          vpx_codec_flags_t flags = 0;
          
          err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
          if (err == VPX_CODEC_OK) {
            vpx_codec_destroy(&codec);
          }
        }
      }
      break;
      
    case 3:  // Edge case 4: set g_pass to VPX_RC_LAST_PASS
      {
        // Set to LAST_PASS but don't provide required two-pass stats
        // This should trigger error at line 307-308
        
        cfg.g_pass = VPX_RC_LAST_PASS;
        // Don't set rc_twopass_stats_in, leaving it NULL
        
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
          vpx_codec_destroy(&codec);
        }
      }
      break;
      
    case 4:  // Edge case 5: rc_resize_allowed = 1 with invalid rc_scaled_width/height
      {
        // Enable resize but set scaled dimensions outside valid range
        cfg.rc_resize_allowed = 1;
        
        // Set scaled width > original width or scaled height > original height
        // or set them to 0 when resize is allowed (0 is also invalid)
        unsigned int scale_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
        
        if (scale_choice == 0) {
          // Width too large
          cfg.rc_scaled_width = width + fdp.ConsumeIntegralInRange<unsigned int>(1, 100);
          cfg.rc_scaled_height = height / 2;
        } else if (scale_choice == 1) {
          // Height too large  
          cfg.rc_scaled_width = width / 2;
          cfg.rc_scaled_height = height + fdp.ConsumeIntegralInRange<unsigned int>(1, 100);
        } else {
          // Both 0 (invalid when resize_allowed = 1)
          cfg.rc_scaled_width = 0;
          cfg.rc_scaled_height = 0;
        }
        
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
          vpx_codec_destroy(&codec);
        }
      }
      break;
      
    case 5:  // Edge case 6: g_lag_in_frames < max_gf_interval + 2
      {
        // Need to set max_gf_interval > 0 and g_lag_in_frames < max_gf_interval + 2
        // This triggers error at line 239
        
        unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(3, 10);
        unsigned int lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(1, max_gf_interval + 1);
        
        cfg.g_lag_in_frames = lag_in_frames;
        
        // Initialize encoder first
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
          // Try to set max_gf_interval through control
          vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
          
          // Try to reconfigure to trigger validation
          vpx_codec_enc_config_set(&codec, &cfg);
          
          vpx_codec_destroy(&codec);
        }
      }
      break;
      
    case 6:  // Edge case 7: kf_mode != VPX_KF_DISABLED with kf_min_dist != kf_max_dist and kf_min_dist > 0
      {
        // Set kf_mode to AUTO (not DISABLED)
        cfg.kf_mode = VPX_KF_AUTO;
        
        // Set kf_min_dist > 0 and != kf_max_dist
        cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(1, 10);
        cfg.kf_max_dist = cfg.kf_min_dist + fdp.ConsumeIntegralInRange<unsigned int>(1, 10);
        
        vpx_codec_ctx_t codec;
        vpx_codec_flags_t flags = 0;
        
        err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
          vpx_codec_destroy(&codec);
        }
      }
      break;
  }

  // Step 6: Additional testing - for valid configurations, initialize successfully
  // then test vpx_codec_enc_config_set with invalid changes
  
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 50) {
    // Create a valid configuration first
    vpx_codec_enc_cfg_t valid_cfg;
    err = vpx_codec_enc_config_default(iface, &valid_cfg, 0);
    if (err == VPX_CODEC_OK) {
      valid_cfg.g_w = width;
      valid_cfg.g_h = height;
      valid_cfg.rc_target_bitrate = bitrate;
      valid_cfg.g_timebase.num = framerate_num;
      valid_cfg.g_timebase.den = framerate_den;
      valid_cfg.g_pass = VPX_RC_ONE_PASS;
      valid_cfg.g_lag_in_frames = 0;
      
      vpx_codec_ctx_t codec;
      vpx_codec_flags_t flags = 0;
      
      err = vpx_codec_enc_init_ver(&codec, iface, &valid_cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err == VPX_CODEC_OK) {
        // Create test image for encoding
        vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 1);
        if (img != NULL) {
          // Fill image with test data
          size_t y_plane_size = width * height;
          size_t uv_plane_size = (width/2) * (height/2);
          
          // Consume some bytes for image data
          if (fdp.remaining_bytes() >= y_plane_size + uv_plane_size * 2) {
            std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
            std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
            std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
            
            memcpy(img->planes[VPX_PLANE_Y], y_data.data(), y_plane_size);
            memcpy(img->planes[VPX_PLANE_U], u_data.data(), uv_plane_size);
            memcpy(img->planes[VPX_PLANE_V], v_data.data(), uv_plane_size);
            
            // Try to encode one frame
            vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
            
            // Try to change configuration with invalid settings
            vpx_codec_enc_cfg_t invalid_cfg = valid_cfg;
            
            // Make an invalid change - e.g., set width to 0
            invalid_cfg.g_w = 0;
            vpx_codec_enc_config_set(&codec, &invalid_cfg);
          }
          
          vpx_img_free(img);
        }
        
        vpx_codec_destroy(&codec);
      }
    }
  }

  return 0;
}

} // namespace harness_008
