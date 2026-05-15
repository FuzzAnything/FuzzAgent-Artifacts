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

namespace harness_006 {
/*
 * Fuzzing harness for libvpx VP9 encoder with SVC (Scalable Video Coding) configurations
 * Targets: VP9 encoder with SVC configurations and varied encoder controls to exercise
 *          blocked branches in vp9_pick_inter_mode (237/764), rd_pick_partition (220/342),
 *          and validate_config (208/720)
 * APIs: vpx_codec_enc_init_ver, vpx_codec_enc_config_default, vpx_codec_control_,
 *       vpx_codec_encode, vpx_codec_get_cx_data, vpx_codec_destroy, vpx_img_alloc, vpx_img_free
 * Specific controls: VP9E_SET_SVC_PARAMETERS, VP9E_SET_SVC_LAYER_ID, VP9E_SET_INTERP_FILTER,
 *                    VP9E_SET_TEMPORAL_LAYERING_MODE, VP8E_SET_CPUUSED, VP8E_SET_ARNR_MAXFRAMES,
 *                    VP8E_SET_GF_CBR_BOOST_PCT
 * Semantic diversity: This harness focuses on SVC configurations and encoder controls to test
 *                     complex encoder decision paths not covered by previous harnesses
 *                     (vs. harness_000: basic decoding, harness_001: single encoder encoding,
 *                      harness_002: multi-encoder, harness_003: error handling,
 *                      harness_004: image operations, harness_005: codec info queries)
 * Coverage goal: Target major branch coverage gaps in vp9_pick_inter_mode, rd_pick_partition,
 *                and validate_config functions by testing SVC and encoder control configurations
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for SVC configuration parameters
  if (size < 128) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Consume encoder configuration parameters from fuzzer input
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 320);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 240);
  unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
  unsigned int framerate_num = 1;
  unsigned int framerate_den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
  
  // Ensure even dimensions for YUV formats
  if (width % 2) width++;
  if (height % 2) height++;

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

  // Step 4: Configure for SVC (Scalable Video Coding)
  // Set spatial layers > 1 and temporal layers > 1 to enable SVC
  unsigned int spatial_layers = fdp.ConsumeIntegralInRange<unsigned int>(2, 3);
  unsigned int temporal_layers = fdp.ConsumeIntegralInRange<unsigned int>(2, 3);
  
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.rc_target_bitrate = bitrate;
  cfg.g_timebase.num = framerate_num;
  cfg.g_timebase.den = framerate_den;
  cfg.g_pass = VPX_RC_ONE_PASS;
  cfg.g_lag_in_frames = 0;  // No lookahead for real-time encoding
  
  // Enable SVC by setting multiple layers
  cfg.ss_number_layers = spatial_layers;
  cfg.ts_number_layers = temporal_layers;
  
  // Configure layer bitrates (simplified distribution)
  for (unsigned int sl = 0; sl < spatial_layers; ++sl) {
    for (unsigned int tl = 0; tl < temporal_layers; ++tl) {
      unsigned int layer = sl * temporal_layers + tl;
      if (layer < VPX_MAX_LAYERS) {
        cfg.layer_target_bitrate[layer] = bitrate / (spatial_layers * temporal_layers);
      }
    }
  }

  // Step 5: Initialize encoder with SVC configuration
  vpx_codec_ctx_t codec;
  vpx_codec_flags_t flags = 0;
  
  // Randomly enable some flags from fuzzer input
  bool use_highbitdepth = fdp.ConsumeBool();
  bool use_psnr = fdp.ConsumeBool();
  
  if (use_highbitdepth) {
    flags |= VPX_CODEC_USE_HIGHBITDEPTH;
  }
  if (use_psnr) {
    flags |= VPX_CODEC_USE_PSNR;
  }

  err = vpx_codec_enc_init_ver(&codec, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    return 0;
  }

  // Step 6: Set SVC parameters using vpx_codec_control_
  vpx_svc_extra_cfg_t svc_params;
  memset(&svc_params, 0, sizeof(svc_params));
  
  // Configure SVC parameters from fuzzer input
  for (unsigned int i = 0; i < VPX_MAX_LAYERS; ++i) {
    svc_params.max_quantizers[i] = fdp.ConsumeIntegralInRange<int>(20, 63);
    svc_params.min_quantizers[i] = fdp.ConsumeIntegralInRange<int>(0, 40);
    
    // Set scaling factors for spatial layers
    if (i < spatial_layers) {
      svc_params.scaling_factor_num[i] = fdp.ConsumeIntegralInRange<int>(1, 16);
      svc_params.scaling_factor_den[i] = fdp.ConsumeIntegralInRange<int>(16, 32);
      svc_params.speed_per_layer[i] = fdp.ConsumeIntegralInRange<int>(-8, 8);
    }
  }
  
  // Set temporal layering mode (0-3)
  svc_params.temporal_layering_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
  
  // Apply SVC parameters
  err = vpx_codec_control(&codec, VP9E_SET_SVC_PARAMETERS, &svc_params);
  // Error may occur if SVC not properly configured, but continue anyway

  // Step 7: Set various encoder controls to test different code paths
  // 7.1: Enable SVC
  int enable_svc = 1;
  vpx_codec_control(&codec, VP9E_SET_SVC, enable_svc);
  
  // 7.2: Set CPU usage (affects speed/quality tradeoff)
  int cpu_used = fdp.ConsumeIntegralInRange<int>(-8, 8);
  vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
  
  // 7.3: Set AQ mode (adaptive quantization)
  unsigned int aq_mode = fdp.ConsumeIntegralInRange<unsigned int>(0, 4);
  vpx_codec_control(&codec, VP9E_SET_AQ_MODE, aq_mode);
  
  // 7.4: Set temporal layer ID
  int temporal_layer_id = fdp.ConsumeIntegralInRange<int>(0, temporal_layers - 1);
  vpx_codec_control(&codec, VP8E_SET_TEMPORAL_LAYER_ID, temporal_layer_id);
  
  // 7.5: Set ARNR max frames (noise reduction)
  unsigned int arnr_max_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 15);
  vpx_codec_control(&codec, VP8E_SET_ARNR_MAXFRAMES, arnr_max_frames);
  
  // 7.6: Set GF CBR boost percentage (golden frame rate control)
  unsigned int gf_cbr_boost_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
  vpx_codec_control(&codec, VP8E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);

  // Step 8: Create input image for encoding
  vpx_image_t* img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, 16);
  if (img == NULL) {
    vpx_codec_destroy(&codec);
    return 0;
  }
  
  // Fill image with synthetic data from fuzzer input
  size_t y_size = width * height;
  size_t uv_size = (width / 2) * (height / 2);
  
  if (fdp.remaining_bytes() < y_size + 2 * uv_size) {
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    return 0;
  }
  
  // Fill Y plane
  std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_size);
  if (y_data.size() == y_size) {
    memcpy(img->planes[VPX_PLANE_Y], y_data.data(), y_size);
  }
  
  // Fill U plane
  std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_size);
  if (u_data.size() == uv_size) {
    memcpy(img->planes[VPX_PLANE_U], u_data.data(), uv_size);
  }
  
  // Fill V plane
  std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_size);
  if (v_data.size() == uv_size) {
    memcpy(img->planes[VPX_PLANE_V], v_data.data(), uv_size);
  }

  // Step 9: Encode multiple frames with different frame types and SVC layer configurations
  unsigned int num_frames = fdp.ConsumeIntegralInRange<unsigned int>(1, 10);
  
  for (unsigned int frame_idx = 0; frame_idx < num_frames; ++frame_idx) {
    if (fdp.remaining_bytes() < 16) {
      break;  // Not enough data for more frames
    }
    
    // Set SVC layer ID for this frame
    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));
    
    // Choose spatial and temporal layer IDs from fuzzer input
    layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<int>(0, spatial_layers - 1);
    layer_id.temporal_layer_id = fdp.ConsumeIntegralInRange<int>(0, temporal_layers - 1);
    
    // Set temporal layer ID per spatial layer
    for (unsigned int sl = 0; sl < VPX_SS_MAX_LAYERS; ++sl) {
      layer_id.temporal_layer_id_per_spatial[sl] = 
          fdp.ConsumeIntegralInRange<int>(0, temporal_layers - 1);
    }
    
    // Apply layer ID
    vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
    
    // Determine frame type (I-frame, P-frame, etc.)
    unsigned long duration = 1;
    vpx_codec_flags_t encode_flags = 0;
    
    // First frame should be keyframe
    if (frame_idx == 0) {
      encode_flags |= VPX_EFLAG_FORCE_KF;
    } else {
      // Randomly force keyframe based on fuzzer input
      bool force_keyframe = fdp.ConsumeBool();
      if (force_keyframe) {
        encode_flags |= VPX_EFLAG_FORCE_KF;
      }
    }
    
    // Randomly set reference frame flags
    bool no_ref_last = fdp.ConsumeBool();
    bool no_ref_gf = fdp.ConsumeBool();
    bool no_ref_arf = fdp.ConsumeBool();
    
    if (no_ref_last) encode_flags |= VP8_EFLAG_NO_REF_LAST;
    if (no_ref_gf) encode_flags |= VP8_EFLAG_NO_REF_GF;
    if (no_ref_arf) encode_flags |= VP8_EFLAG_NO_REF_ARF;
    
    // Encode the frame
    err = vpx_codec_encode(&codec, img, frame_idx, duration, encode_flags, VPX_DL_REALTIME);
    
    // Step 10: Retrieve encoded data
    const vpx_codec_cx_pkt_t *pkt = NULL;
    vpx_codec_iter_t iter = NULL;
    
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
      // Process different packet types
      switch (pkt->kind) {
        case VPX_CODEC_CX_FRAME_PKT:
          // Frame data available
          // Could process or validate frame data here
          break;
        case VPX_CODEC_STATS_PKT:
          // Statistics data
          break;
        case VPX_CODEC_FPMB_STATS_PKT:
          // First pass MB statistics
          break;
        case VPX_CODEC_PSNR_PKT:
          // PSNR data
          break;
        default:
          // Other packet types
          break;
      }
    }
    
    // Update image data slightly for next frame to create variation
    if (fdp.remaining_bytes() > 0 && frame_idx < num_frames - 1) {
      // Add some noise to Y plane
      size_t modify_count = std::min<size_t>(100, fdp.remaining_bytes());
      std::vector<uint8_t> noise = fdp.ConsumeBytes<uint8_t>(modify_count);
      
      // Apply noise to random positions
      for (size_t i = 0; i < std::min<size_t>(noise.size(), 100); ++i) {
        size_t pos = static_cast<size_t>(noise[i]) % y_size;
        img->planes[VPX_PLANE_Y][pos] ^= 0x01;  // Flip LSB
      }
    }
  }

  // Step 11: Cleanup
  vpx_img_free(img);
  vpx_codec_destroy(&codec);

  return 0;
}

} // namespace harness_006
