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

namespace harness_012 {
/*
 * Fuzzing harness for libvpx high-bit-depth VP9 encoding 
 * Targets: vpx_dsp module (37.52% function coverage) and major branch blockers:
 *          vp9_pick_inter_mode, encode_frame_to_data_rate, rd_pick_partition, setup_rtcd_internal
 * APIs: vpx_codec_enc_init_ver with VPX_CODEC_USE_HIGHBITDEPTH, vpx_codec_enc_config_default,
 *       vpx_codec_encode, vpx_codec_get_cx_data, vpx_codec_destroy, vpx_img_alloc with high bit depth
 * Specific focus: High-bit-depth VP9 encoding (10-bit and 12-bit) with VPX_CODEC_USE_HIGHBITDEPTH flag,
 *                 different color formats (I42016, I42216, I44416), and advanced encoder features
 *                 to exercise vpx_dsp module functions
 * Semantic diversity: This harness specifically targets high-bit-depth VP9 encoding paths
 *                     to address coverage gaps in vpx_dsp module not covered by previous harnesses
 *                     (vs. harness_001: general encoding with some high-bit-depth,
 *                      harness_006: SVC configurations, harness_007: complex encoding,
 *                      harness_008-011: validation and control API testing)
 * Coverage goal: Improve vpx_dsp module coverage (37.52% function coverage) by testing
 *                high-bit-depth encoding paths and exercise major branch blockers
 *                in vp9_pick_inter_mode, encode_frame_to_data_rate, rd_pick_partition
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for high-bit-depth encoding configuration
  if (size < 512) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Choose high-bit-depth configuration from fuzzer input
  
  // Select bit depth: 10-bit or 12-bit encoding
  int bit_depth_choice = fdp.ConsumeIntegralInRange<int>(0, 1); // 0=10-bit, 1=12-bit
  vpx_bit_depth_t bit_depth = (bit_depth_choice == 0) ? VPX_BITS_10 : VPX_BITS_12;
  
  // Image dimensions for high-bit-depth testing
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 320);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 240);
  
  // Ensure even dimensions for YUV formats
  if (width % 2) width++;
  if (height % 2) height++;
  
  // Bitrate settings appropriate for high-bit-depth
  unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(500, 20000);
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

  // Step 4: Configure for high-bit-depth encoding
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.rc_target_bitrate = bitrate;
  cfg.g_timebase.num = framerate_num;
  cfg.g_timebase.den = framerate_den;
  cfg.g_pass = VPX_RC_ONE_PASS;
  cfg.g_lag_in_frames = 0;
  
  // Configure for high-bit-depth processing
  // This affects vpx_dsp module functions for high-bit-depth transforms
  cfg.g_bit_depth = bit_depth;
  cfg.g_input_bit_depth = bit_depth;
  
  // Enable some advanced features to exercise rd_pick_partition and vp9_pick_inter_mode
  cfg.g_error_resilient = fdp.ConsumeBool() ? 1 : 0;
  
  // Set quantizer ranges appropriate for high-bit-depth
  cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
  cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(cfg.rc_min_quantizer, 63);
  
  // Configure temporal scalability to exercise encode_frame_to_data_rate
  cfg.ts_number_layers = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
  for (unsigned int tl = 0; tl < cfg.ts_number_layers; ++tl) {
    cfg.ts_rate_decimator[tl] = 1 << (cfg.ts_number_layers - tl - 1);
    cfg.layer_target_bitrate[tl] = bitrate / cfg.ts_number_layers;
  }

  // Step 5: Initialize encoder with HIGHBITDEPTH flag (CRITICAL)
  vpx_codec_ctx_t codec;
  vpx_codec_flags_t flags = VPX_CODEC_USE_HIGHBITDEPTH; // Enable high-bit-depth encoding
  
  // Additional flags based on fuzzer input
  bool use_psnr = fdp.ConsumeBool();
  bool use_output_partition = fdp.ConsumeBool();
  
  if (use_psnr) {
    flags |= VPX_CODEC_USE_PSNR;
  }
  if (use_output_partition) {
    flags |= VPX_CODEC_USE_OUTPUT_PARTITION;
  }

  err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    // Try without extra flags if initialization failed
    flags = VPX_CODEC_USE_HIGHBITDEPTH; // Keep high-bit-depth flag
    err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
      return 0;
    }
  }

  // Step 6: Set encoder controls for high-bit-depth optimization
  // These controls affect vpx_dsp module functions
  
  // Set CPU usage (affects rd_pick_partition and vp9_pick_inter_mode speed/quality tradeoffs)
  int cpu_used = fdp.ConsumeIntegralInRange<int>(-8, 8);
  vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
  
  // Set AQ mode for high-bit-depth content
  int aq_mode = fdp.ConsumeIntegralInRange<int>(0, 4);
  vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode);
  
  // Set color space for high-bit-depth (important for vpx_dsp color conversion)
  int color_space_val = fdp.ConsumeIntegralInRange<int>(0, 7);
  vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space_val);
  
  // Set color range (full vs studio) for high-bit-depth
  vpx_color_range_t color_range = fdp.ConsumeBool() ? VPX_CR_FULL_RANGE : VPX_CR_STUDIO_RANGE;
  vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, static_cast<int>(color_range));
  
  // Set target level for high-bit-depth profiles
  int target_level = fdp.ConsumeIntegralInRange<int>(10, 62);
  vpx_codec_control(&codec, VP9E_SET_TARGET_LEVEL, target_level);

  // Step 7: Create high-bit-depth image with appropriate format
  // Choose high-bit-depth image format
  int format_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
  vpx_img_fmt_t image_format;
  
  switch (format_choice) {
    case 0:
      image_format = VPX_IMG_FMT_I42016;  // 4:2:0 high-bit-depth
      break;
    case 1:
      image_format = VPX_IMG_FMT_I42216;  // 4:2:2 high-bit-depth
      break;
    case 2:
      image_format = VPX_IMG_FMT_I44416;  // 4:4:4 high-bit-depth
      break;
    case 3:
      image_format = VPX_IMG_FMT_I44016;  // 4:4:0 high-bit-depth
      break;
    default:
      image_format = VPX_IMG_FMT_I42016;
      break;
  }
  
  // Alignment for high-bit-depth images (important for SIMD optimizations in vpx_dsp)
  unsigned int align = fdp.ConsumeIntegralInRange<unsigned int>(16, 64);
  
  // Allocate high-bit-depth image
  vpx_image_t* img = vpx_img_alloc(NULL, image_format, width, height, align);
  if (img == NULL) {
    vpx_codec_destroy(&codec);
    return 0;
  }
  
  // Set image properties for high-bit-depth
  img->bit_depth = bit_depth;
  img->range = color_range;
  img->cs = static_cast<vpx_color_space_t>(color_space_val);
  
  // Step 8: Fill image with fuzzer data for high-bit-depth encoding
  // Calculate total bytes needed for high-bit-depth image data
  size_t bytes_per_sample = 2; // 10-bit and 12-bit both use 16-bit (2-byte) storage
  size_t luma_size = width * height * bytes_per_sample;
  size_t chroma_size;
  switch (image_format) {
    case VPX_IMG_FMT_I42016:  // 4:2:0
      chroma_size = (width / 2) * (height / 2) * bytes_per_sample * 2;
      break;
    case VPX_IMG_FMT_I42216:  // 4:2:2
      chroma_size = (width / 2) * height * bytes_per_sample * 2;
      break;
    case VPX_IMG_FMT_I44416:  // 4:4:4
      chroma_size = width * height * bytes_per_sample * 2;
      break;
    case VPX_IMG_FMT_I44016:  // 4:4:0
      chroma_size = width * (height / 2) * bytes_per_sample * 2;
      break;
    default:
      chroma_size = (width / 2) * (height / 2) * bytes_per_sample * 2;
      break;
  }
  
  size_t total_image_size = luma_size + chroma_size;
  
  // Check if we have enough fuzzer data for image
  if (fdp.remaining_bytes() < total_image_size) {
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    return 0;
  }
  
  // Fill luma plane (Y)
  std::vector<uint8_t> luma_data = fdp.ConsumeBytes<uint8_t>(luma_size);
  memcpy(img->planes[VPX_PLANE_Y], luma_data.data(), luma_size);
  
  // Fill chroma planes (U and V)
  std::vector<uint8_t> chroma_data = fdp.ConsumeBytes<uint8_t>(chroma_size);
  size_t chroma_plane_size = chroma_size / 2;
  
  memcpy(img->planes[VPX_PLANE_U], chroma_data.data(), chroma_plane_size);
  memcpy(img->planes[VPX_PLANE_V], chroma_data.data() + chroma_plane_size, chroma_plane_size);

  // Step 9: Encode the high-bit-depth frame
  // Set deadline (quality vs speed tradeoff)
  unsigned long deadline = fdp.ConsumeIntegralInRange<unsigned long>(0, 1000000);
  
  // Encode the frame (this exercises vpx_dsp module functions)
  err = vpx_codec_encode(&codec, img, 0, 1, 0, deadline);
  
  // Step 10: Get encoded data (exercises data path)
  const vpx_codec_cx_pkt_t *pkt = NULL;
  vpx_codec_iter_t iter = NULL;
  
  while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
    // Process packet if needed
    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
      // Frame data available - could be used for further processing
      // This exercises encode_frame_to_data_rate path
    }
  }

  // Step 11: Encode additional frames with different parameters
  // to exercise different paths in rd_pick_partition and vp9_pick_inter_mode
  
  if (fdp.remaining_bytes() > total_image_size) {
    // Create a second frame with different data
    std::vector<uint8_t> luma_data2 = fdp.ConsumeBytes<uint8_t>(luma_size);
    std::vector<uint8_t> chroma_data2 = fdp.ConsumeBytes<uint8_t>(chroma_size);
    
    if (luma_data2.size() == luma_size && chroma_data2.size() == chroma_size) {
      memcpy(img->planes[VPX_PLANE_Y], luma_data2.data(), luma_size);
      memcpy(img->planes[VPX_PLANE_U], chroma_data2.data(), chroma_plane_size);
      memcpy(img->planes[VPX_PLANE_V], chroma_data2.data() + chroma_plane_size, chroma_plane_size);
      
      // Encode second frame with different flags
      unsigned int force_keyframe = fdp.ConsumeBool() ? VPX_EFLAG_FORCE_KF : 0;
      err = vpx_codec_encode(&codec, img, 1, 1, force_keyframe, deadline);
      
      // Get data for second frame
      iter = NULL;
      while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packet
      }
    }
  }

  // Step 12: Test flush to get remaining frames
  // This exercises completion paths in encode_frame_to_data_rate
  err = vpx_codec_encode(&codec, NULL, 0, 0, 0, deadline);
  
  iter = NULL;
  while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
    // Process final packets
  }

  // Step 13: Cleanup
  vpx_img_free(img);
  vpx_codec_destroy(&codec);

  return 0;
}

} // namespace harness_012
