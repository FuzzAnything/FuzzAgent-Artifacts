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

namespace harness_015 {
/*
 * Fuzzing harness for libvpx low-coverage utility modules and helper functions
 * Target modules: vpx_ports (23.64%), vpx_scale (39.19%), vpx_util (19.10%), and deeper vpx_dsp functions
 * Primary goal: Improve coverage in foundational utility modules that are prerequisites for reaching
 *               deeper encoder logic and branch blockers like validate_config
 * APIs to target:
 *   - vpx_img_wrap, vpx_img_set_rect (image creation/manipulation helpers)
 *   - vpx_codec_enc_init_ver, vpx_codec_enc_config_set, vpx_codec_encode, vpx_codec_get_cx_data (encoder context)
 *   - vpx_ports functions: system abstraction layer, timing, CPU detection (accessed indirectly through encoder)
 *   - vpx_scale functions: vpx_scale_frame (Scale2D with 0% coverage) - accessed through encoding paths
 *   - vpx_util functions: debug, threading utilities (accessed indirectly through encoder)
 * Semantic diversity: This harness specifically targets low-coverage utility modules (<40% coverage)
 *                     unlike previous harnesses that focused on high-level encoder/decoder APIs.
 *                     It creates a realistic encoder context to exercise helper functions that
 *                     are prerequisites for reaching deeper encoder validation paths.
 * Coverage goal: Improve coverage in vpx_ports, vpx_scale, vpx_util modules to enable better
 *                exploration of complex encoder logic and conditional branches
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed: configuration + image data + utility parameters
  if (size < 512) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // ============================================================
  // PART 1: CREATE ENCODER CONTEXT TO TRIGGER UTILITY MODULES
  // ============================================================
  
  // Choose encoder type: 0 for VP8, 1 for VP9
  int encoder_type = fdp.ConsumeIntegralInRange<int>(0, 1);
  
  vpx_codec_iface_t* encoder_iface = nullptr;
  if (encoder_type == 0) {
    encoder_iface = vpx_codec_vp8_cx();
  } else {
    encoder_iface = vpx_codec_vp9_cx();
  }
  
  if (encoder_iface == nullptr) {
    return 0;
  }

  // Get default encoder configuration
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_err_t err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
  if (err != VPX_CODEC_OK) {
    return 0;
  }

  // Consume configuration parameters from fuzzer input
  cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
  cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
  cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  
  // Ensure even dimensions for YUV formats
  if (cfg.g_w % 2) cfg.g_w++;
  if (cfg.g_h % 2) cfg.g_h++;

  // Encoder flags
  vpx_codec_flags_t flags = 0;
  bool use_highbitdepth = fdp.ConsumeBool();
  if (use_highbitdepth) {
    flags |= VPX_CODEC_USE_HIGHBITDEPTH;
  }

  // Initialize encoder (API: vpx_codec_enc_init_ver)
  vpx_codec_ctx_t codec;
  err = vpx_codec_enc_init_ver(&codec, encoder_iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
  if (err != VPX_CODEC_OK) {
    // Try without highbitdepth flag if initialization failed
    if (use_highbitdepth) {
      flags &= ~VPX_CODEC_USE_HIGHBITDEPTH;
      err = vpx_codec_enc_init_ver(&codec, encoder_iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
      if (err != VPX_CODEC_OK) {
        return 0;
      }
    } else {
      return 0;
    }
  }

  // Apply encoder configuration (API: vpx_codec_enc_config_set)
  // Test with slightly modified config to trigger validation paths
  cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 20000);
  vpx_codec_enc_config_set(&codec, &cfg);

  // ============================================================
  // PART 2: EXERCISE vpx_img_wrap AND vpx_img_set_rect
  // ============================================================
  
  // Choose image format
  int format_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
  vpx_img_fmt_t image_format;
  switch (format_choice) {
    case 0: image_format = VPX_IMG_FMT_I420; break;
    case 1: image_format = VPX_IMG_FMT_I422; break;
    case 2: image_format = VPX_IMG_FMT_I444; break;
    case 3: image_format = VPX_IMG_FMT_I42016; break;
    case 4: image_format = VPX_IMG_FMT_I42216; break;
    default: image_format = VPX_IMG_FMT_I420; break;
  }

  // Calculate required buffer size
  unsigned int stride_align = 32;
  unsigned int bytes_per_pixel = (image_format & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1;
  unsigned int y_stride = (cfg.g_w * bytes_per_pixel + stride_align - 1) & ~(stride_align - 1);
  unsigned int uv_stride = 0;
  unsigned int uv_height = 0;
  
  // Calculate UV plane dimensions based on format
  if (image_format == VPX_IMG_FMT_I420 || image_format == VPX_IMG_FMT_I42016) {
    uv_stride = y_stride / 2;
    uv_height = cfg.g_h / 2;
  } else if (image_format == VPX_IMG_FMT_I422 || image_format == VPX_IMG_FMT_I42216) {
    uv_stride = y_stride;
    uv_height = cfg.g_h;
  } else if (image_format == VPX_IMG_FMT_I444 || image_format == VPX_IMG_FMT_I44416) {
    uv_stride = y_stride;
    uv_height = cfg.g_h;
  }
  
  // Calculate total buffer size
  size_t buffer_size = y_stride * cfg.g_h;
  if (uv_stride > 0 && uv_height > 0) {
    buffer_size += 2 * uv_stride * uv_height; // U and V planes
  }
  
  // Allocate image buffer
  std::vector<unsigned char> img_buffer(buffer_size);
  
  // Fill with fuzzed data
  size_t bytes_to_fill = std::min(buffer_size, fdp.remaining_bytes());
  if (bytes_to_fill > 0) {
    std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(bytes_to_fill);
    if (image_data.size() > 0) {
      memcpy(img_buffer.data(), image_data.data(), std::min(image_data.size(), img_buffer.size()));
    }
  }

  // Create image using vpx_img_wrap (API target)
  vpx_image_t img;
  vpx_image_t* wrapped_img = vpx_img_wrap(&img, image_format, cfg.g_w, cfg.g_h, 
                                          stride_align, img_buffer.data());
  if (wrapped_img == nullptr) {
    vpx_codec_destroy(&codec);
    return 0;
  }

  // Test vpx_img_set_rect with various rectangles (API target)
  unsigned int rect_x = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.g_w / 2);
  unsigned int rect_y = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.g_h / 2);
  unsigned int rect_w = fdp.ConsumeIntegralInRange<unsigned int>(1, cfg.g_w - rect_x);
  unsigned int rect_h = fdp.ConsumeIntegralInRange<unsigned int>(1, cfg.g_h - rect_y);
  
  // Ensure even dimensions for chroma subsampling
  if (rect_w % 2) rect_w--;
  if (rect_h % 2) rect_h--;
  if (rect_w == 0) rect_w = 2;
  if (rect_h == 0) rect_h = 2;
  
  int set_rect_result = vpx_img_set_rect(&img, rect_x, rect_y, rect_w, rect_h);
  (void)set_rect_result;

  // ============================================================
  // PART 3: ENCODE FRAMES WITH VARIOUS SETTINGS TO TRIGGER UTILITY MODULES
  // ============================================================
  
  // Encode multiple frames with different settings to trigger
  // vpx_scale, vpx_ports, and vpx_util modules
  
  int num_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
  for (int frame_idx = 0; frame_idx < num_frames && fdp.remaining_bytes() > 0; frame_idx++) {
    // Modify image data slightly between frames
    if (img_buffer.size() > 0 && fdp.remaining_bytes() > 0) {
      size_t modify_bytes = std::min(img_buffer.size() / 4, fdp.remaining_bytes());
      if (modify_bytes > 0) {
        std::vector<uint8_t> new_data = fdp.ConsumeBytes<uint8_t>(modify_bytes);
        for (size_t i = 0; i < new_data.size() && i < img_buffer.size(); i++) {
          img_buffer[i] ^= new_data[i]; // Simple modification
        }
      }
    }
    
    // Encode the image (API: vpx_codec_encode)
    // Use different deadlines to trigger different code paths
    unsigned long deadline = VPX_DL_REALTIME;
    if (fdp.ConsumeBool()) {
      deadline = VPX_DL_GOOD_QUALITY;
    } else if (fdp.ConsumeBool()) {
      deadline = VPX_DL_BEST_QUALITY;
    }
    
    int force_keyframe = (frame_idx == 0 || fdp.ConsumeBool()) ? 1 : 0;
    
    err = vpx_codec_encode(&codec, &img, frame_idx, 1, force_keyframe, deadline);
    
    if (err == VPX_CODEC_OK) {
      // Retrieve encoded data (API: vpx_codec_get_cx_data)
      const vpx_codec_cx_pkt_t *pkt = nullptr;
      vpx_codec_iter_t iter = nullptr;
      
      while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process different packet types
        switch (pkt->kind) {
          case VPX_CODEC_CX_FRAME_PKT:
            // Frame data available - this may trigger vpx_scale operations
            // for reference frame scaling and vpx_ports for timing
            break;
          case VPX_CODEC_STATS_PKT:
            // Statistics data - may trigger vpx_util debug/logging functions
            break;
          case VPX_CODEC_PSNR_PKT:
            // PSNR data - may trigger vpx_util calculations
            break;
          default:
            // Other packet types
            break;
        }
      }
    }
    
    // Test configuration changes between frames
    if (frame_idx == num_frames / 2 && fdp.ConsumeBool()) {
      // Change bitrate mid-stream
      cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 30000);
      vpx_codec_enc_config_set(&codec, &cfg);
    }
  }

  // ============================================================
  // PART 4: TEST EDGE CASES TO TRIGGER UTILITY MODULE ERROR PATHS
  // ============================================================
  
  // Test with invalid image dimensions to trigger error handling in utility modules
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
    vpx_image_t invalid_img;
    // Try to wrap with zero dimensions (should fail gracefully)
    vpx_image_t* invalid_wrapped = vpx_img_wrap(&invalid_img, VPX_IMG_FMT_I420, 0, 0, 16, nullptr);
    (void)invalid_wrapped; // Expected to be nullptr
    
    // Try to set invalid rectangle
    if (wrapped_img != nullptr) {
      int invalid_result = vpx_img_set_rect(&img, cfg.g_w + 10, cfg.g_h + 10, 100, 100);
      (void)invalid_result; // Expected to be -1
    }
  }

  // ============================================================
  // PART 5: CLEANUP
  // ============================================================
  
  // Destroy encoder (API: vpx_codec_destroy)
  // This will trigger cleanup in vpx_ports, vpx_scale, and vpx_util modules
  vpx_codec_destroy(&codec);
  
  // Note: vpx_img_wrap doesn't allocate memory, so we don't need to free the image
  // The buffer is owned by img_buffer vector which will be automatically cleaned up

  return 0;
}

} // namespace harness_015
