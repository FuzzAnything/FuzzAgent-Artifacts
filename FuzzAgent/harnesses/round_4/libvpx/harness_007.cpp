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

namespace harness_007 {
/*
 * Fuzzing harness for libvpx VP9 encoder configuration validation and complex encoding scenarios
 * Targets: Major branch blockers in validate_config (220 blocked branches), vp9_pick_inter_mode (237 blocked),
 *          rd_pick_partition (220 blocked), encode_frame_to_data_rate (215 blocked)
 * APIs: vpx_codec_enc_init_ver, vpx_codec_enc_config_default, vpx_codec_encode, vpx_codec_get_cx_data,
 *       vpx_codec_destroy, vpx_codec_control_, vpx_img_alloc, vpx_img_free
 * Specific configuration targets:
 *   - Set extra_cfg->max_gf_interval > 0 to hit line 228 in validate_config (major blocker)
 *   - Set cfg->rc_resize_allowed = 1 with scaled dimensions for resize validation
 *   - Set cfg->ts_number_layers > 1 with proper rate decimators for multi-layer validation
 *   - Set cfg->kf_mode != VPX_KF_DISABLED with kf_min_dist > 0 for keyframe validation
 *   - Test various AQ modes, cyclic refresh, and row-based multi-threading
 * Semantic diversity: This harness specifically targets configuration validation paths that previous
 *                     harnesses missed, particularly the max_gf_interval > 0 validation branch
 *                     (vs. harness_000: basic decoding, harness_001: single encoder encoding,
 *                      harness_002: multi-encoder, harness_003: error handling,
 *                      harness_004: image operations, harness_005: codec info queries,
 *                      harness_006: SVC configurations)
 * Coverage goal: Unlock 220 blocked branches in validate_config by setting max_gf_interval > 0
 *                and exercise complex encoding decision paths in vp9_pick_inter_mode and rd_pick_partition
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for complex configuration parameters
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

  // Step 4: Configure to target validation paths (CRITICAL FOR COVERAGE)
  
  // CRITICAL: Set max_gf_interval > 0 to hit line 228 in validate_config
  // This is the #1 branch blocker with 220 blocked branches
  unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(2, 16);
  
  // CRITICAL: Enable resize validation
  cfg.rc_resize_allowed = 1;
  cfg.rc_scaled_width = fdp.ConsumeIntegralInRange<unsigned int>(width / 2, width);
  cfg.rc_scaled_height = fdp.ConsumeIntegralInRange<unsigned int>(height / 2, height);
  
  // CRITICAL: Enable multi-layer encoding for ts_number_layers > 1 validation
  cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(2, 3);
  cfg.ss_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 2);
  
  // Configure rate decimators properly (required for ts_number_layers > 1)
  for (unsigned int tl = 0; tl < cfg.ts_number_layers; ++tl) {
    cfg.ts_rate_decimator[tl] = 1 << (cfg.ts_number_layers - tl - 1);
  }
  
  // CRITICAL: Set kf_mode != VPX_KF_DISABLED with kf_min_dist > 0
  cfg.kf_mode = fdp.ConsumeBool() ? VPX_KF_AUTO : VPX_KF_DISABLED;
  if (cfg.kf_mode != VPX_KF_DISABLED) {
    cfg.kf_min_dist = fdp.ConsumeIntegralInRange<unsigned int>(1, 100);
    cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(cfg.kf_min_dist, 1000);
  }
  
  // Configure lag in frames to work with max_gf_interval (line 234-237 validation)
  if (max_gf_interval > 0) {
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, max_gf_interval + 5);
  } else {
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 16);
  }
  
  // Set basic encoder parameters
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.rc_target_bitrate = bitrate;
  cfg.g_timebase.num = framerate_num;
  cfg.g_timebase.den = framerate_den;
  cfg.g_pass = VPX_RC_ONE_PASS;
  
  // Configure layer bitrates for multi-layer encoding
  for (unsigned int sl = 0; sl < cfg.ss_number_layers; ++sl) {
    for (unsigned int tl = 0; tl < cfg.ts_number_layers; ++tl) {
      unsigned int layer = sl * cfg.ts_number_layers + tl;
      if (layer < VPX_MAX_LAYERS) {
        cfg.layer_target_bitrate[layer] = bitrate / (cfg.ss_number_layers * cfg.ts_number_layers);
      }
    }
  }

  // Step 5: Initialize encoder with complex configuration
  vpx_codec_ctx_t codec;
  vpx_codec_flags_t flags = 0;
  
  // Randomly enable some flags from fuzzer input
  bool use_highbitdepth = fdp.ConsumeBool();
  bool use_psnr = fdp.ConsumeBool();
  bool use_output_partition = fdp.ConsumeBool();
  
  if (use_highbitdepth) {
    flags |= VPX_CODEC_USE_HIGHBITDEPTH;
  }
  if (use_psnr) {
    flags |= VPX_CODEC_USE_PSNR;
  }
  if (use_output_partition) {
    flags |= VPX_CODEC_USE_OUTPUT_PARTITION;
  }
  
  // Initialize encoder
  err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    // If initialization fails due to invalid configuration, try with simpler config
    // but we still want to test the validation paths
    return 0;
  }

  // Step 6: Apply advanced encoder controls (vpx_codec_control_) to test more validation
  
  // CRITICAL: Set max_gf_interval > 0 using control interface
  if (max_gf_interval > 0) {
    vpx_codec_control(&codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
  }
  
  // Set other encoder controls to exercise different paths
  if (fdp.ConsumeBool()) {
    vpx_codec_control(&codec, VP9E_SET_ALT_REF_AQ, fdp.ConsumeIntegralInRange<int>(0, 1));
  }
  
  if (fdp.ConsumeBool()) {
    vpx_codec_control(&codec, VP9E_SET_AQ_MODE, fdp.ConsumeIntegralInRange<int>(0, 4));
  }
  
  if (fdp.ConsumeBool()) {
    vpx_codec_control(&codec, VP9E_SET_ROW_MT, fdp.ConsumeIntegralInRange<int>(0, 1));
  }
  
  if (fdp.ConsumeBool()) {
    vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, fdp.ConsumeIntegralInRange<int>(0, 7));
  }
  
  if (fdp.ConsumeBool()) {
    vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, fdp.ConsumeIntegralInRange<int>(0, 31));
  }
  
  if (fdp.ConsumeBool()) {
    vpx_codec_control(&codec, VP9E_SET_FRAME_PARALLEL_DECODING, fdp.ConsumeIntegralInRange<int>(0, 1));
  }
  
  if (fdp.ConsumeBool()) {
    vpx_codec_control(&codec, VP9E_SET_NOISE_SENSITIVITY, fdp.ConsumeIntegralInRange<unsigned int>(0, 6));
  }

  // Step 7: Create input image for encoding
  
  // Choose image format based on high-bit-depth flag
  vpx_img_fmt_t image_format;
  if (use_highbitdepth) {
    image_format = VPX_IMG_FMT_I42016;
  } else {
    image_format = VPX_IMG_FMT_I420;
  }
  
  vpx_image_t* img = vpx_img_alloc(NULL, image_format, width, height, 1);
  if (img == NULL) {
    vpx_codec_destroy(&codec);
    return 0;
  }
  
  // Fill image with fuzzer data
  size_t image_size = width * height * 3 / 2;  // YUV420 size
  if (use_highbitdepth) {
    image_size *= 2;  // 16-bit samples
  }
  
  if (fdp.remaining_bytes() < image_size) {
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    return 0;
  }
  
  std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
  memcpy(img->planes[0], image_data.data(), width * height * (use_highbitdepth ? 2 : 1));
  if (image_data.size() > width * height * (use_highbitdepth ? 2 : 1)) {
    size_t uv_size = width * height / 4 * (use_highbitdepth ? 2 : 1);
    memcpy(img->planes[1], image_data.data() + width * height * (use_highbitdepth ? 2 : 1), uv_size);
    if (image_data.size() > width * height * (use_highbitdepth ? 2 : 1) + uv_size) {
      memcpy(img->planes[2], image_data.data() + width * height * (use_highbitdepth ? 2 : 1) + uv_size, uv_size);
    }
  }

  // Step 8: Encode multiple frames with different flags to exercise different paths
  
  unsigned int frame_count = fdp.ConsumeIntegralInRange<unsigned int>(1, 5);
  vpx_codec_flags_t encode_flags = 0;
  
  for (unsigned int i = 0; i < frame_count; ++i) {
    // Vary frame flags to test different encoding paths
    if (i == 0 && fdp.ConsumeBool()) {
      encode_flags |= VPX_EFLAG_FORCE_KF;  // Force keyframe
    }
    
    if (fdp.ConsumeBool()) {
      encode_flags |= VP8_EFLAG_NO_REF_LAST;  // Don't reference last frame
    }
    
    if (fdp.ConsumeBool()) {
      encode_flags |= VP8_EFLAG_NO_REF_GF;  // Don't reference golden frame
    }
    
    if (fdp.ConsumeBool()) {
      encode_flags |= VP8_EFLAG_NO_REF_ARF;  // Don't reference alt-ref frame
    }
    
    // Encode the frame
    err = vpx_codec_encode(&codec, img, i, 1, encode_flags, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
      // Encoding error - continue to test other paths
      continue;
    }
    
    // Step 9: Retrieve encoded packets to exercise get_cx_data paths
    const vpx_codec_cx_pkt_t *pkt = NULL;
    vpx_codec_iter_t iter = NULL;
    
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
      // Process different packet types
      switch (pkt->kind) {
        case VPX_CODEC_CX_FRAME_PKT:
          // Frame packet - this is what we want
          break;
        case VPX_CODEC_STATS_PKT:
          // Stats packet
          break;
        case VPX_CODEC_FPMB_STATS_PKT:
          // First pass MB stats
          break;
        default:
          break;
      }
    }
    
    // Reset flags for next frame
    encode_flags = 0;
  }

  // Step 10: Cleanup
  vpx_img_free(img);
  vpx_codec_destroy(&codec);

  return 0;
}

} // namespace harness_007
