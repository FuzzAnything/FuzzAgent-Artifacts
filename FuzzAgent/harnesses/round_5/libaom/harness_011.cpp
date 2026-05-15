#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"

namespace harness_011 {
/*
 * Fuzzing harness for libaom AV1 encoder targeting global motion estimation and FAST corner detection
 * Primary Target: aom_fast9_detect function with 1798 blocked branches (91% blockage)
 * Strategy: Generate synthetic images with varying corner patterns to trigger complex conditional logic
 * Differentiation from existing harnesses: Specialized corner pattern generation for FAST detection
 */



// Helper function to create synthetic corner patterns in image data
static void generate_corner_pattern(uint8_t* y_plane, int width, int height, int stride,
                                    FuzzedDataProvider& fdp) {
  // Clear the plane first
  std::fill(y_plane, y_plane + height * stride, 128);
  
  // Determine number of corners to generate
  int num_corners = fdp.ConsumeIntegralInRange<int>(0, 50);
  int corner_size = fdp.ConsumeIntegralInRange<int>(1, 5);
  int corner_intensity = fdp.ConsumeIntegralInRange<int>(20, 100);
  
  for (int i = 0; i < num_corners; i++) {
    if (fdp.remaining_bytes() < 8) break;
    
    // Random corner position (avoiding edges for FAST algorithm requirements)
    int x = fdp.ConsumeIntegralInRange<int>(3, width - 4);
    int y = fdp.ConsumeIntegralInRange<int>(3, height - 4);
    
    // Get corner pattern type from fuzzer input
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Create different corner patterns to trigger different branches in aom_fast9_detect
    switch (pattern_type) {
      case 0: { // Bright corner (high intensity center, lower surroundings)
        for (int dy = -corner_size; dy <= corner_size; dy++) {
          for (int dx = -corner_size; dx <= corner_size; dx++) {
            int px = x + dx;
            int py = y + dy;
            if (px >= 0 && px < width && py >= 0 && py < height) {
              uint8_t* pixel = y_plane + py * stride + px;
              // Center is brightest
              if (dx == 0 && dy == 0) {
                *pixel = 255;
              } else {
                // Surroundings are darker
                int distance = std::abs(dx) + std::abs(dy);
                *pixel = 128 - corner_intensity * distance / (2 * corner_size);
              }
            }
          }
        }
        break;
      }
        
      case 1: { // Dark corner (low intensity center, higher surroundings)
        for (int dy = -corner_size; dy <= corner_size; dy++) {
          for (int dx = -corner_size; dx <= corner_size; dx++) {
            int px = x + dx;
            int py = y + dy;
            if (px >= 0 && px < width && py >= 0 && py < height) {
              uint8_t* pixel = y_plane + py * stride + px;
              // Center is darkest
              if (dx == 0 && dy == 0) {
                *pixel = 0;
              } else {
                // Surroundings are brighter
                int distance = std::abs(dx) + std::abs(dy);
                *pixel = 128 + corner_intensity * distance / (2 * corner_size);
              }
            }
          }
        }
        break;
      }
        
      case 2: { // Edge pattern (vertical/horizontal intensity gradient)
        bool is_vertical = fdp.ConsumeBool();
        if (is_vertical) {
          // Vertical edge
          for (int dy = -corner_size; dy <= corner_size; dy++) {
            for (int dx = -corner_size; dx <= corner_size; dx++) {
              int px = x + dx;
              int py = y + dy;
              if (px >= 0 && px < width && py >= 0 && py < height) {
                uint8_t* pixel = y_plane + py * stride + px;
                *pixel = 128 + (dx * corner_intensity) / corner_size;
              }
            }
          }
        } else {
          // Horizontal edge
          for (int dy = -corner_size; dy <= corner_size; dy++) {
            for (int dx = -corner_size; dx <= corner_size; dx++) {
              int px = x + dx;
              int py = y + dy;
              if (px >= 0 && px < width && py >= 0 && py < height) {
                uint8_t* pixel = y_plane + py * stride + px;
                *pixel = 128 + (dy * corner_intensity) / corner_size;
              }
            }
          }
        }
        break;
      }
        
      case 3: { // Checkerboard pattern
        for (int dy = -corner_size; dy <= corner_size; dy++) {
          for (int dx = -corner_size; dx <= corner_size; dx++) {
            int px = x + dx;
            int py = y + dy;
            if (px >= 0 && px < width && py >= 0 && py < height) {
              uint8_t* pixel = y_plane + py * stride + px;
              if ((dx + dy) % 2 == 0) {
                *pixel = 128 + corner_intensity;
              } else {
                *pixel = 128 - corner_intensity;
              }
            }
          }
        }
        break;
      }
    }
  }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Need enough data for encoder configuration and corner pattern generation
  if (size < 512) {
    return 0;
  }
  FuzzedDataProvider fdp(data, size);

  // Consume encoder configuration parameters from fuzzer input
  // Use smaller dimensions for faster fuzzing while still testing corner detection
  uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(32, 128);
  uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(32, 128);
  
  // Ensure even dimensions (required by AV1)
  width = (width / 2) * 2;
  height = (height / 2) * 2;
  if (width < 32 || height < 32) {
    width = 32;
    height = 32;
  }

  // CRITICAL: Set usage to AOM_USAGE_REALTIME for global motion estimation
  uint32_t usage = AOM_USAGE_REALTIME;  // Fixed to target global motion estimation
  
  uint32_t cpu_used = fdp.ConsumeIntegralInRange<uint32_t>(0, 10);  // CPU usage (speed settings)
  uint32_t target_bitrate = fdp.ConsumeIntegralInRange<uint32_t>(100, 2000);
  bool error_resilient = fdp.ConsumeBool();
  uint32_t frame_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 3);

  // Get AV1 encoder interface
  aom_codec_iface_t *encoder = aom_codec_av1_cx();
  if (!encoder) {
    return 0;
  }

  // Step 1: Call aom_codec_enc_config_default to get default encoder config
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t res = aom_codec_enc_config_default(encoder, &cfg, usage);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Configure encoder for global motion estimation
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;  // 30 FPS
  cfg.rc_target_bitrate = target_bitrate;
  cfg.g_error_resilient = error_resilient ? AOM_ERROR_RESILIENT_DEFAULT : 0;
  cfg.g_lag_in_frames = 0;  // No lookahead for fuzzing

  // Step 2: Initialize encoder context with aom_codec_enc_init_ver
  aom_codec_ctx_t codec;
  res = aom_codec_enc_init_ver(&codec, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Step 3: Configure encoder specifically for global motion estimation
  // CRITICAL: Enable global motion estimation to reach aom_fast9_detect
  aom_codec_control(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, 1);  // Always enable for this harness
  
  // Set CPU usage (speed/quality tradeoff)
  aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);
  
  // Enable warped motion (related to global motion estimation)
  aom_codec_control(&codec, AV1E_SET_ENABLE_WARPED_MOTION, 1);
  aom_codec_control(&codec, AV1E_SET_ALLOW_WARPED_MOTION, 1);
  
  // Enable other motion estimation features
  aom_codec_control(&codec, AV1E_SET_ENABLE_OBMC, fdp.ConsumeBool() ? 1 : 0);
  
  // Enable TPL model for temporal dependency
  aom_codec_control(&codec, AV1E_SET_ENABLE_TPL_MODEL, fdp.ConsumeBool() ? 1 : 0);

  // Step 4: Create test image with synthetic corner patterns
  aom_image_t raw;
  if (!aom_img_alloc(&raw, AOM_IMG_FMT_I420, width, height, 1)) {
    aom_codec_destroy(&codec);
    return 0;
  }

  // Calculate required image buffer sizes
  size_t y_plane_size = width * height;
  size_t uv_plane_size = (width / 2) * (height / 2);
  
  // Check if we have enough data for the image
  if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
    aom_img_free(&raw);
    aom_codec_destroy(&codec);
    return 0;
  }

  // Fill Y plane with synthetic corner patterns
  std::vector<uint8_t> y_plane_buffer = fdp.ConsumeBytes<uint8_t>(y_plane_size);
  std::copy(y_plane_buffer.begin(), y_plane_buffer.end(), raw.planes[0]);
  
  // Apply synthetic corner patterns to trigger FAST corner detection
  generate_corner_pattern(raw.planes[0], width, height, raw.stride[0], fdp);

  // Fill U and V planes with uniform data (less important for corner detection)
  std::vector<uint8_t> u_plane = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
  std::vector<uint8_t> v_plane = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
  std::copy(u_plane.begin(), u_plane.end(), raw.planes[1]);
  std::copy(v_plane.begin(), v_plane.end(), raw.planes[2]);

  // Set stride information
  raw.stride[0] = width;
  raw.stride[1] = width / 2;
  raw.stride[2] = width / 2;

  // Step 5: Call aom_codec_encode multiple times with different frame types
  for (uint32_t frame_idx = 0; frame_idx < frame_count; frame_idx++) {
    int flags = 0;
    
    // Force keyframe for first frame to test keyframe path
    if (frame_idx == 0) {
      flags |= AOM_EFLAG_FORCE_KF;
    }
    
    // Use remaining fuzzer data to determine frame duration
    uint32_t duration = 1;  // Default 1 tick
    if (fdp.remaining_bytes() >= sizeof(uint32_t)) {
      duration = fdp.ConsumeIntegralInRange<uint32_t>(1, 4);
    }

    // Encode the frame - this should trigger global motion estimation and FAST corner detection
    res = aom_codec_encode(&codec, &raw, frame_idx, duration, flags);
    
    // Even if encoding fails, continue to test error paths
    
    // Step 6: Call aom_codec_get_cx_data in loop to retrieve compressed data packets
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
      // Test different packet types
      switch (pkt->kind) {
        case AOM_CODEC_CX_FRAME_PKT:
          // Frame packet - normal case
          break;
        case AOM_CODEC_STATS_PKT:
          // Statistics packet
          break;
        case AOM_CODEC_PSNR_PKT:
          // PSNR packet
          break;
        default:
          // Other packet types
          break;
      }
    }
  }

  // Step 7: Cleanup
  aom_img_free(&raw);
  aom_codec_destroy(&codec);
  
  return 0;
}

} // namespace harness_011
