#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"

namespace harness_005 {
/*
 * Fuzzing harness for libvpx codec information query operations
 * Targets: vpx_codec_iface_name, vpx_codec_get_caps, vpx_codec_version_str,
 *          vpx_codec_version, vpx_codec_build_config, vpx_codec_version_extra_str
 * Helper APIs: vpx_codec_dec_init_ver/vpx_codec_enc_init_ver, vpx_codec_destroy
 * Codecs: VP8 and VP9 (both decoder and encoder interfaces)
 * Semantic diversity: This harness focuses on codec informational/query functions
 *                     (vs. harness_000: decoding, harness_001: single encoder encoding,
 *                      harness_002: multi-encoder, harness_003: control functions,
 *                      harness_004: image/color space operations)
 * Coverage goal: Complete API surface coverage (6 remaining uncovered APIs)
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed: configuration parameters for codec initialization
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Choose codec type and mode from fuzzer input
  // 0: VP8 decoder, 1: VP8 encoder, 2: VP9 decoder, 3: VP9 encoder
  int codec_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
  
  // Step 2: Choose initialization parameters
  unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
  vpx_codec_flags_t flags = 0;
  
  // Randomly enable some flags
  bool use_postproc = fdp.ConsumeBool();
  bool use_error_concealment = fdp.ConsumeBool();
  bool use_frame_threading = fdp.ConsumeBool();
  bool use_psnr = fdp.ConsumeBool();
  bool use_highbitdepth = fdp.ConsumeBool();
  
  if (use_postproc) {
    flags |= VPX_CODEC_USE_POSTPROC;
  }
  if (use_error_concealment) {
    flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
  }
  if (use_frame_threading) {
    flags |= VPX_CODEC_USE_FRAME_THREADING;
  }
  if (use_psnr) {
    flags |= VPX_CODEC_USE_PSNR;
  }
  if (use_highbitdepth) {
    flags |= VPX_CODEC_USE_HIGHBITDEPTH;
  }

  vpx_codec_ctx_t codec;
  vpx_codec_err_t err = VPX_CODEC_OK;
  vpx_codec_iface_t* iface = nullptr;
  
  // Step 3: Initialize appropriate codec based on choice
  switch (codec_choice) {
    case 0:  // VP8 decoder
      iface = vpx_codec_vp8_dx();
      {
        vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
        err = vpx_codec_dec_init_ver(&codec, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
      }
      break;
      
    case 1:  // VP8 encoder
      iface = vpx_codec_vp8_cx();
      {
        vpx_codec_enc_cfg_t cfg;
        // Get default configuration first
        err = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (err == VPX_CODEC_OK) {
          // Set some reasonable values for fuzzing
          cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
          cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
          cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
          cfg.g_timebase.num = 1;
          cfg.g_timebase.den = 30;
          
          err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        }
      }
      break;
      
    case 2:  // VP9 decoder
      iface = vpx_codec_vp9_dx();
      {
        vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
        err = vpx_codec_dec_init_ver(&codec, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
      }
      break;
      
    case 3:  // VP9 encoder
      iface = vpx_codec_vp9_cx();
      {
        vpx_codec_enc_cfg_t cfg;
        // Get default configuration first
        err = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (err == VPX_CODEC_OK) {
          // Set some reasonable values for fuzzing
          cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
          cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
          cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
          cfg.g_timebase.num = 1;
          cfg.g_timebase.den = 30;
          
          err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
        }
      }
      break;
  }
  
  // Step 4: Handle initialization failures
  if (err != VPX_CODEC_OK) {
    // Try initialization without flags if it failed
    flags = 0;
    
    // Re-initialize with same parameters but no flags
    switch (codec_choice) {
      case 0:  // VP8 decoder
        {
          vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
          err = vpx_codec_dec_init_ver(&codec, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
        }
        break;
      case 1:  // VP8 encoder
        {
          vpx_codec_enc_cfg_t cfg;
          if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
            err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
          }
        }
        break;
      case 2:  // VP9 decoder
        {
          vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
          err = vpx_codec_dec_init_ver(&codec, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
        }
        break;
      case 3:  // VP9 encoder
        {
          vpx_codec_enc_cfg_t cfg;
          if (vpx_codec_enc_config_default(iface, &cfg, 0) == VPX_CODEC_OK) {
            err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
          }
        }
        break;
    }
    
    if (err != VPX_CODEC_OK) {
      return 0;  // Initialization failed even without flags
    }
  }
  
  // Step 5: Now test the 6 target informational APIs
  
  // 5.1: vpx_codec_iface_name - Get codec interface name
  const char* iface_name = vpx_codec_iface_name(iface);
  if (iface_name) {
    // Just access the string to ensure it's valid (no crash)
    volatile char first_char = iface_name[0];
    (void)first_char;  // Prevent unused variable warning
  }
  
  // 5.2: vpx_codec_get_caps - Get codec capabilities
  vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
  // caps is a bitfield, we can check some bits
  volatile int has_decoder_cap = (caps & VPX_CODEC_CAP_DECODER) != 0;
  volatile int has_encoder_cap = (caps & VPX_CODEC_CAP_ENCODER) != 0;
  volatile int has_highbitdepth_cap = (caps & VPX_CODEC_CAP_HIGHBITDEPTH) != 0;
  (void)has_decoder_cap;
  (void)has_encoder_cap;
  (void)has_highbitdepth_cap;
  
  // 5.3: vpx_codec_version - Get numeric version
  int version = vpx_codec_version();
  volatile int version_major = (version >> 16) & 0xFF;
  volatile int version_minor = (version >> 8) & 0xFF;
  volatile int version_patch = version & 0xFF;
  (void)version_major;
  (void)version_minor;
  (void)version_patch;
  
  // 5.4: vpx_codec_version_str - Get version string
  const char* version_str = vpx_codec_version_str();
  if (version_str) {
    volatile char version_first_char = version_str[0];
    (void)version_first_char;
  }
  
  // 5.5: vpx_codec_version_extra_str - Get extra version string
  const char* version_extra_str = vpx_codec_version_extra_str();
  if (version_extra_str) {
    volatile char extra_first_char = version_extra_str[0];
    (void)extra_first_char;
  }
  
  // 5.6: vpx_codec_build_config - Get build configuration string
  const char* build_config = vpx_codec_build_config();
  if (build_config) {
    volatile char config_first_char = build_config[0];
    (void)config_first_char;
  }
  
  // Step 6: Additional error information queries (optional, for coverage)
  // Get error message if any error occurred
  const char* error_detail = vpx_codec_error_detail(&codec);
  if (error_detail) {
    volatile char error_first_char = error_detail[0];
    (void)error_first_char;
  }
  
  // Get error message
  const char* error_msg = vpx_codec_error(&codec);
  if (error_msg) {
    volatile char msg_first_char = error_msg[0];
    (void)msg_first_char;
  }
  
  // Step 7: Clean up
  vpx_codec_destroy(&codec);
  
  return 0;
}

} // namespace harness_005
