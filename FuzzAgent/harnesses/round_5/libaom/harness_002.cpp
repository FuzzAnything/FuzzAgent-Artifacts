#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"

namespace harness_002 {
/*
 * Lightweight fuzzing harness for libaom AV1 encoder
 * Targets core encoder APIs with minimal computation for fast AFL execution
 * Based on coverage guidance targeting aom_codec_enc_init_ver, aom_codec_encode,
 * aom_codec_get_cx_data, aom_codec_enc_config_set
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimal size check - we need enough for basic parameters and tiny image
  if (size < 64) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Fixed tiny image size for speed (minimum valid size for AV1)
  const uint32_t width = 16;
  const uint32_t height = 16;

  // Consume minimal encoder parameters from fuzzer input
  uint32_t usage = fdp.ConsumeIntegralInRange<uint32_t>(0, 2);  // 0=GOOD_QUALITY, 1=REALTIME, 2=ALL_INTRA
  uint32_t target_bitrate = fdp.ConsumeIntegralInRange<uint32_t>(100, 1000);
  bool error_resilient = fdp.ConsumeBool();
  
  // Use only 1 frame for speed (vs 10 in harness_000)
  uint32_t frame_count = 1;

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

  // Minimal configuration for speed
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;  // 30 FPS
  cfg.rc_target_bitrate = target_bitrate;
  cfg.g_error_resilient = error_resilient ? AOM_ERROR_RESILIENT_DEFAULT : 0;
  cfg.g_lag_in_frames = 0;  // No lookahead for speed

  // Initialize encoder context with aom_codec_enc_init_ver as per guidance
  aom_codec_ctx_t codec;
  res = aom_codec_enc_init_ver(&codec, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION);
  if (res != AOM_CODEC_OK) {
    return 0;
  }

  // Create minimal image for encoding
  aom_image_t raw;
  if (!aom_img_alloc(&raw, AOM_IMG_FMT_I420, width, height, 1)) {
    aom_codec_destroy(&codec);
    return 0;
  }

  // Fill image planes with fuzzer data (tiny planes for speed)
  size_t y_plane_size = width * height;           // 16*16 = 256 bytes
  size_t uv_plane_size = (width / 2) * (height / 2);  // 8*8 = 64 bytes
  
  // Check if we have enough data for the image
  if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
    aom_img_free(&raw);
    aom_codec_destroy(&codec);
    return 0;
  }

  // Fill Y plane (256 bytes)
  std::vector<uint8_t> y_plane = fdp.ConsumeBytes<uint8_t>(y_plane_size);
  std::copy(y_plane.begin(), y_plane.end(), raw.planes[0]);

  // Fill U plane (64 bytes)
  std::vector<uint8_t> u_plane = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
  std::copy(u_plane.begin(), u_plane.end(), raw.planes[1]);

  // Fill V plane (64 bytes)
  std::vector<uint8_t> v_plane = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
  std::copy(v_plane.begin(), v_plane.end(), raw.planes[2]);

  // Set stride information
  raw.stride[0] = width;
  raw.stride[1] = width / 2;
  raw.stride[2] = width / 2;

  // Encode single frame (vs multiple frames in harness_000)
  for (uint32_t frame_idx = 0; frame_idx < frame_count; frame_idx++) {
    int flags = 0;
    
    // Force keyframe for first frame to test keyframe path
    if (frame_idx == 0) {
      flags |= AOM_EFLAG_FORCE_KF;
    }

    // Target API: aom_codec_encode
    res = aom_codec_encode(&codec, &raw, frame_idx, 1, flags);
    // Don't check result - continue even if encoding fails to test error paths

    // Target API: aom_codec_get_cx_data
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
      // Just consume the packets - we're testing the encoding process
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Frame encoded successfully
      }
    }
  }

  // Test aom_codec_enc_config_set with a modified configuration
  // Use remaining fuzzer data to modify some config parameters
  if (fdp.remaining_bytes() >= sizeof(uint32_t) * 2) {
    aom_codec_enc_cfg_t new_cfg = cfg;
    
    // Modify some parameters with fuzzer data
    uint32_t new_bitrate = fdp.ConsumeIntegralInRange<uint32_t>(50, 2000);
    uint32_t new_usage = fdp.ConsumeIntegralInRange<uint32_t>(0, 2);
    
    new_cfg.rc_target_bitrate = new_bitrate;
    
    // Target API: aom_codec_enc_config_set
    res = aom_codec_enc_config_set(&codec, &new_cfg);
    // Don't check result - test both success and failure paths
  }

  // Cleanup
  aom_img_free(&raw);
  aom_codec_destroy(&codec);

  return 0;
}

} // namespace harness_002
