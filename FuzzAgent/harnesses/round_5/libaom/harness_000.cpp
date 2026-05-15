#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"

namespace harness_000 {
/*
 * Fuzzing harness for libaom AV1 encoder
 * Targets core encoding APIs to complement existing decoder fuzzer
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size < 128) {
    return 0;  // Need enough data for meaningful encoding
  }

  FuzzedDataProvider fdp(data, size);

  // Consume configuration parameters from fuzzer input
  uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
  uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
  uint32_t usage = fdp.ConsumeIntegralInRange<uint32_t>(0, 2);  // 0=GOOD_QUALITY, 1=REALTIME, 2=ALL_INTRA
  uint32_t cpu_used = fdp.ConsumeIntegralInRange<uint32_t>(0, 10);
  uint32_t target_bitrate = fdp.ConsumeIntegralInRange<uint32_t>(100, 10000);
  uint32_t error_resilient = fdp.ConsumeBool() ? 1 : 0;
  uint32_t frame_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 10);
  uint32_t keyframe_interval = fdp.ConsumeIntegralInRange<uint32_t>(0, 10);

  // Ensure even dimensions (required by AV1)
  width = (width / 2) * 2;
  height = (height / 2) * 2;
  if (width < 16 || height < 16) {
    width = 16;
    height = 16;
  }

  // Get AV1 encoder interface
  aom_codec_iface_t *encoder = aom_codec_av1_cx();
  if (!encoder) {
    return 0;
  }

  // Initialize encoder configuration
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t res = aom_codec_enc_config_default(encoder, &cfg, usage);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Configure encoder
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;  // 30 FPS
  cfg.rc_target_bitrate = target_bitrate;
  cfg.g_error_resilient = static_cast<aom_codec_er_flags_t>(error_resilient);
  cfg.g_lag_in_frames = 0;  // No lookahead for fuzzing

  // Initialize encoder context
  aom_codec_ctx_t codec;
  res = aom_codec_enc_init(&codec, encoder, &cfg, 0);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Set CPU usage (speed/quality tradeoff)
  aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);

  // Create a synthetic image for encoding
  aom_image_t raw;
  if (!aom_img_alloc(&raw, AOM_IMG_FMT_I420, width, height, 1)) {
    aom_codec_destroy(&codec);
    return 0;
  }

  // Fill image planes with fuzzer data
  size_t y_plane_size = width * height;
  size_t uv_plane_size = (width / 2) * (height / 2);
  
  // Check if we have enough data for the image
  if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
    aom_img_free(&raw);
    aom_codec_destroy(&codec);
    return 0;
  }

  // Fill Y plane
  std::vector<uint8_t> y_plane = fdp.ConsumeBytes<uint8_t>(y_plane_size);
  std::copy(y_plane.begin(), y_plane.end(), raw.planes[0]);

  // Fill U plane
  std::vector<uint8_t> u_plane = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
  std::copy(u_plane.begin(), u_plane.end(), raw.planes[1]);

  // Fill V plane
  std::vector<uint8_t> v_plane = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
  std::copy(v_plane.begin(), v_plane.end(), raw.planes[2]);

  // Set stride information
  raw.stride[0] = width;
  raw.stride[1] = width / 2;
  raw.stride[2] = width / 2;

  // Encode frames
  for (uint32_t frame_idx = 0; frame_idx < frame_count; frame_idx++) {
    int flags = 0;
    
    // Force keyframe at interval if specified
    if (keyframe_interval > 0 && frame_idx % keyframe_interval == 0) {
      flags |= AOM_EFLAG_FORCE_KF;
    }

    // Encode the frame
    res = aom_codec_encode(&codec, &raw, frame_idx, 1, flags);
    if (res != AOM_CODEC_OK) {
      // Continue even if encoding fails to test error paths
    }

    // Retrieve encoded data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
      // Just consume the packets - we're testing the encoding process
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Frame encoded successfully
      }
    }

    // Modify image data slightly for next frame to test temporal coding
    if (frame_idx < frame_count - 1 && fdp.remaining_bytes() > 0) {
      // Add some noise to the image for next frame
      size_t noise_bytes = std::min(fdp.remaining_bytes(), y_plane_size / 10);
      std::vector<uint8_t> noise = fdp.ConsumeBytes<uint8_t>(noise_bytes);
      for (size_t i = 0; i < noise_bytes && i < y_plane_size; i++) {
        raw.planes[0][i] = (raw.planes[0][i] + noise[i]) % 256;
      }
    }
  }

  // Flush encoder
  while (true) {
    res = aom_codec_encode(&codec, nullptr, -1, 1, 0);
    if (res != AOM_CODEC_OK) {
      break;
    }
    
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    bool got_packet = false;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
      got_packet = true;
    }
    
    if (!got_packet) {
      break;
    }
  }

  // Test encoder control APIs with fuzzer data
  if (fdp.remaining_bytes() > 0) {
    uint32_t control_test = fdp.ConsumeIntegral<uint32_t>() % 4;
    
    switch (control_test) {
      case 0:
        aom_codec_control(&codec, AOME_SET_CPUUSED, 
                         fdp.ConsumeIntegralInRange<int>(0, 10));
        break;
      case 1:
        aom_codec_control(&codec, AOME_SET_ENABLEAUTOALTREF,
                         fdp.ConsumeBool() ? 1 : 0);
        break;
      case 2:
        aom_codec_control(&codec, AOME_SET_SHARPNESS,
                         fdp.ConsumeIntegralInRange<int>(0, 7));
        break;
      case 3:
        aom_codec_control(&codec, AV1E_SET_COLOR_RANGE,
                         fdp.ConsumeBool() ? AOM_CR_FULL_RANGE : AOM_CR_STUDIO_RANGE);
        break;
    }
  }

  // Cleanup
  aom_img_free(&raw);
  aom_codec_destroy(&codec);

  return 0;
}

} // namespace harness_000
