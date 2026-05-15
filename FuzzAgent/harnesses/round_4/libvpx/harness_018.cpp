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

namespace harness_018 {
/*
 * Fuzzing harness for libvpx VP8 encoder configuration validation testing
 * Target: validate_config function in vp8/vp8_cx_iface.c with 309 blocked branches
 * Focus: Comprehensive testing of VP8 encoder configuration validation error paths
 *        specifically targeting validation logic unique to VP8 (not covered by VP9 harnesses)
 * APIs: vpx_codec_enc_config_default, vpx_codec_enc_init_ver, vpx_codec_enc_config_set, 
 *       vpx_codec_control, vpx_codec_encode, vpx_codec_destroy
 * Semantic diversity: This harness specifically targets VP8-specific validation logic
 *                     not covered by harness_017 (which focuses on VP9), including:
 *                     1. Token partition validation specific to VP8
 *                     2. Real-time only configuration constraints
 *                     3. VP8-specific extra configuration parameters
 *                     4. Multi-resolution encoding constraints
 * Coverage goal: Target 309 blocked branches in VP8 validate_config function by testing:
 *                1. Token partition validation (ONE_TOKENPARTITION to EIGHT_TOKENPARTITION)
 *                2. Real-time only constraints (g_pass, g_lag_in_frames limitations)
 *                3. Multi-resolution encoding specific validation
 *                4. VP8-specific range checks (cpu_used range -16..16, different from VP9)
 *                5. Two-pass statistics validation for VP8
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed: configuration parameters for comprehensive validation testing
  if (size < 512) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Get VP8 encoder interface (focus on VP8-specific validation)
  vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
  if (iface == NULL) {
    return 0;
  }

  // Step 2: Get default encoder configuration
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
  if (err != VPX_CODEC_OK) {
    return 0;
  }

  // Step 3: Consume basic configuration parameters from fuzzer input
  // Image dimensions (limited for fuzzing efficiency)
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
  
  // Ensure even dimensions for YUV formats
  if (width % 2) width++;
  if (height % 2) height++;
  
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
  
  // Step 4: Test specific VP8 validation cases
  unsigned int test_case = fdp.ConsumeIntegralInRange<unsigned int>(0, 5);
  
  switch (test_case) {
    case 0: {
      // Test 1: Token partition validation (VP8_ONE_TOKENPARTITION to VP8_EIGHT_TOKENPARTITION)
      // From validate_config lines 208-209:
      // RANGE_CHECK(vp8_cfg, token_partitions, VP8_ONE_TOKENPARTITION, VP8_EIGHT_TOKENPARTITION);
      
      // Set valid token partitions first
      unsigned int token_partitions = fdp.ConsumeIntegralInRange<unsigned int>(
          VP8_ONE_TOKENPARTITION, VP8_EIGHT_TOKENPARTITION);
      
      // Initialize encoder
      vpx_codec_ctx_t codec;
      vpx_codec_flags_t flags = 0;
      
      err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err != VPX_CODEC_OK) {
        return 0;
      }
      
      // Set token partitions via control API
      vpx_codec_control(&codec, VP8E_SET_TOKEN_PARTITIONS, token_partitions);
      
      // Test invalid token partitions (outside valid range)
      unsigned int invalid_token_partitions = fdp.ConsumeIntegralInRange<unsigned int>(
          VP8_EIGHT_TOKENPARTITION + 1, 255);
      vpx_codec_control(&codec, VP8E_SET_TOKEN_PARTITIONS, invalid_token_partitions);
      
      // Try to encode to trigger validation
      vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
      
      vpx_codec_destroy(&codec);
      break;
    }
    
    case 1: {
      // Test 2: Real-time only constraints (g_pass, g_lag_in_frames limitations)
      // From validate_config lines 156-162, 181-188
      // #if CONFIG_REALTIME_ONLY
      //   RANGE_CHECK_HI(cfg, g_lag_in_frames, 0);
      //   RANGE_CHECK(cfg, g_pass, VPX_RC_ONE_PASS, VPX_RC_ONE_PASS);
      // #endif
      
      // Set invalid g_pass for real-time mode (VPX_RC_FIRST_PASS or VPX_RC_LAST_PASS)
      vpx_codec_enc_cfg_t invalid_cfg = cfg;
      
      // Use g_pass values that may be invalid for real-time only builds
      unsigned int pass_choice = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
      vpx_enc_pass invalid_pass;
      switch (pass_choice) {
        case 0: invalid_pass = VPX_RC_ONE_PASS; break;     // Valid
        case 1: invalid_pass = VPX_RC_FIRST_PASS; break;   // May be invalid
        case 2: invalid_pass = VPX_RC_LAST_PASS; break;    // May be invalid
        default: invalid_pass = VPX_RC_ONE_PASS; break;
      }
      
      invalid_cfg.g_pass = invalid_pass;
      
      // Also test invalid lag_in_frames for real-time only
      unsigned int lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(1, 25);
      invalid_cfg.g_lag_in_frames = lag_in_frames;
      
      // Initialize encoder with potentially invalid configuration
      vpx_codec_ctx_t codec;
      vpx_codec_flags_t flags = 0;
      
      err = vpx_codec_enc_init_ver(&codec, iface, &invalid_cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err == VPX_CODEC_OK) {
        // Try to encode
        vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
        vpx_codec_destroy(&codec);
      }
      break;
    }
    
    case 2: {
      // Test 3: Multi-resolution encoding specific validation
      // From validate_config lines 158-160, 171-174:
      // #if CONFIG_MULTI_RES_ENCODING
      //   if (ctx->base.enc.total_encoders > 1) RANGE_CHECK_HI(cfg, g_lag_in_frames, 0);
      //   if (ctx->base.enc.total_encoders > 1) RANGE_CHECK_HI(cfg, rc_resize_allowed, 0);
      // #endif
      
      // Test rc_resize_allowed validation
      vpx_codec_enc_cfg_t test_cfg = cfg;
      
      // Set rc_resize_allowed to non-boolean value to trigger RANGE_CHECK_BOOL
      unsigned int resize_allowed = fdp.ConsumeIntegralInRange<unsigned int>(2, 255);
      test_cfg.rc_resize_allowed = resize_allowed;
      
      // Set scaled dimensions for resize validation
      if (resize_allowed == 1) {
        test_cfg.rc_scaled_width = fdp.ConsumeIntegralInRange<unsigned int>(0, test_cfg.g_w);
        test_cfg.rc_scaled_height = fdp.ConsumeIntegralInRange<unsigned int>(0, test_cfg.g_h);
      }
      
      // Initialize encoder
      vpx_codec_ctx_t codec;
      vpx_codec_flags_t flags = 0;
      
      err = vpx_codec_enc_init_ver(&codec, iface, &test_cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err == VPX_CODEC_OK) {
        vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
        vpx_codec_destroy(&codec);
      }
      break;
    }
    
    case 3: {
      // Test 4: VP8-specific range checks (cpu_used range -16..16, different from VP9)
      // From validate_config line 200:
      // RANGE_CHECK(vp8_cfg, cpu_used, -16, 16);
      
      // Initialize encoder with valid config first
      vpx_codec_ctx_t codec;
      vpx_codec_flags_t flags = 0;
      
      err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err != VPX_CODEC_OK) {
        return 0;
      }
      
      // Test valid cpu_used values
      int cpu_used = fdp.ConsumeIntegralInRange<int>(-16, 16);
      vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
      
      // Test invalid cpu_used values (outside -16..16 range)
      int invalid_cpu_used;
      if (fdp.ConsumeBool()) {
        invalid_cpu_used = fdp.ConsumeIntegralInRange<int>(-100, -17);
      } else {
        invalid_cpu_used = fdp.ConsumeIntegralInRange<int>(17, 100);
      }
      vpx_codec_control(&codec, VP8E_SET_CPUUSED, invalid_cpu_used);
      
      // Try to encode to trigger validation
      vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
      
      vpx_codec_destroy(&codec);
      break;
    }
    
    case 4: {
      // Test 5: Two-pass statistics validation for VP8
      // From validate_config lines 220-241:
      // if (cfg->g_pass == VPX_RC_LAST_PASS) {
      //   // Two-pass stats validation...
      // }
      
      // Only test if not CONFIG_REALTIME_ONLY
      vpx_codec_enc_cfg_t two_pass_cfg = cfg;
      two_pass_cfg.g_pass = VPX_RC_LAST_PASS;
      
      // Create fake two-pass stats buffer with various invalid conditions
      size_t stats_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
      std::vector<uint8_t> stats_buffer(stats_size);
      
      // Fill with random data
      if (stats_size > 0) {
        std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(stats_size);
        if (random_data.size() == stats_size) {
          memcpy(stats_buffer.data(), random_data.data(), stats_size);
        }
      }
      
      two_pass_cfg.rc_twopass_stats_in.buf = stats_buffer.data();
      two_pass_cfg.rc_twopass_stats_in.sz = stats_size;
      
      // Initialize encoder
      vpx_codec_ctx_t codec;
      vpx_codec_flags_t flags = 0;
      
      err = vpx_codec_enc_init_ver(&codec, iface, &two_pass_cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err == VPX_CODEC_OK) {
        vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
        vpx_codec_destroy(&codec);
      }
      break;
    }
    
    case 5: {
      // Test 6: Comprehensive parameter range violations
      // Test multiple range checks in validate_config
      vpx_codec_enc_cfg_t invalid_cfg = cfg;
      
      // Test various range violations
      unsigned int range_test = fdp.ConsumeIntegralInRange<unsigned int>(0, 7);
      
      switch (range_test) {
        case 0:
          // Invalid g_w/g_h (outside 1..16383 range for VP8)
          invalid_cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16384, 65535);
          invalid_cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16384, 65535);
          break;
          
        case 1:
          // Invalid g_timebase
          invalid_cfg.g_timebase.den = 0;  // Invalid: must be >= 1
          invalid_cfg.g_timebase.num = 0;  // Invalid: must be >= 1
          break;
          
        case 2:
          // Invalid g_profile (> 3)
          invalid_cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(4, 255);
          break;
          
        case 3:
          // Invalid quantizer ranges
          invalid_cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(64, 255);
          invalid_cfg.rc_min_quantizer = invalid_cfg.rc_max_quantizer + 1;  // min > max
          break;
          
        case 4:
          // Invalid g_threads (> 64)
          invalid_cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(65, 255);
          break;
          
        case 5:
          // Invalid rc_end_usage
          invalid_cfg.rc_end_usage = static_cast<vpx_rc_mode>(
              fdp.ConsumeIntegralInRange<unsigned int>(VPX_Q + 1, 255));
          break;
          
        case 6:
          // Invalid kf_mode
          invalid_cfg.kf_mode = static_cast<vpx_kf_mode>(
              fdp.ConsumeIntegralInRange<unsigned int>(VPX_KF_AUTO + 1, 255));
          break;
          
        case 7:
          // Invalid ts_number_layers (> 5)
          invalid_cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(6, 255);
          break;
      }
      
      // Initialize encoder with invalid configuration
      vpx_codec_ctx_t codec;
      vpx_codec_flags_t flags = 0;
      
      err = vpx_codec_enc_init_ver(&codec, iface, &invalid_cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err == VPX_CODEC_OK) {
        vpx_codec_encode(&codec, NULL, 0, 1, 0, VPX_DL_REALTIME);
        vpx_codec_destroy(&codec);
      }
      break;
    }
  }

  return 0;
}

} // namespace harness_018
