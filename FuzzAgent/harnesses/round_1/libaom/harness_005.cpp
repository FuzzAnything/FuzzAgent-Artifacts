#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <memory>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_005 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Targeted fuzzing harness for libaom encoder metadata retrieval and informational API testing.
 * Coverage analysis shows API coverage at 70.21%, still in Phase 1. Target 8 APIs with 38+ undiscovered branches:
 * - aom_codec_get_preview_frame (10 branches, 0% covered)
 * - aom_codec_get_global_headers (10 branches, 0% covered)
 * - aom_img_get_metadata (6 branches, 0% covered)
 * - aom_img_num_metadata (4 branches, 0% covered)
 * - aom_codec_iface_name (2 branches, 0% covered)
 * - aom_codec_get_caps (2 branches, 0% covered)
 * - aom_img_plane_height (2 branches, 0% covered)
 * - aom_img_plane_width (2 branches, 0% covered)
 * Semantic differentiation from previous harnesses:
 * - harness_000: Broad decoder/encoder coverage
 * - harness_001: Basic encoder operations
 * - harness_002: Common utilities and error handling
 * - harness_003: Advanced encoder configuration and buffer management
 * - harness_004: Decoder frame buffer management and metadata operations
 * - harness_005: Encoder metadata retrieval and informational API testing (focused on metadata operations)
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum input size needed for meaningful encoder metadata testing
  // Need enough for: config params, image metadata, image data
  const size_t MIN_INPUT_SIZE = 256;
  if (size < MIN_INPUT_SIZE) {
    return 0;  // Insufficient input for meaningful testing
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Consume encoder configuration parameters from fuzzed input
  // Fixed-size parameters first
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 1024);
  unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
  unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
  bool high_bitdepth = fdp.ConsumeBool();
  bool error_resilient = fdp.ConsumeBool();
  unsigned int profile = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3: AV1 profiles
  
  // Metadata parameters
  uint32_t metadata_type1 = fdp.ConsumeIntegral<uint32_t>();
  uint32_t metadata_type2 = fdp.ConsumeIntegral<uint32_t>();
  size_t metadata_size1 = fdp.ConsumeIntegralInRange<size_t>(1, 512);
  size_t metadata_size2 = fdp.ConsumeIntegralInRange<size_t>(1, 512);
  
  // Consume metadata data
  std::vector<uint8_t> metadata_data1 = fdp.ConsumeBytes<uint8_t>(metadata_size1);
  std::vector<uint8_t> metadata_data2 = fdp.ConsumeBytes<uint8_t>(metadata_size2);
  
  // Get remaining data for image content
  std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
  if (image_data.size() < 64) {
    return 0;  // Not enough data for meaningful image
  }

  // Step 2: Get encoder interface
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  if (!encoder_iface) {
    return 0;
  }

  // Step 3: Test aom_codec_iface_name() - informational API
  const char *iface_name = aom_codec_iface_name(encoder_iface);
  if (iface_name == nullptr) {
    // Interface name should not be null for valid interface
    return 0;
  }

  // Step 4: Test aom_codec_get_caps() - informational API
  aom_codec_caps_t caps = aom_codec_get_caps(encoder_iface);
  // caps is a bitfield indicating capabilities - we can check if encoder is supported
  if ((caps & AOM_CODEC_CAP_ENCODER) == 0) {
    // Interface doesn't support encoding (should not happen for AV1 encoder)
    return 0;
  }

  // Step 5: Get default encoder configuration with aom_codec_enc_config_default()
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t cfg_err = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
  if (cfg_err != AOM_CODEC_OK) {
    return 0;
  }

  // Apply fuzzed configuration parameters
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_threads = threads;
  cfg.rc_target_bitrate = bitrate;
  cfg.g_error_resilient = error_resilient ? AOM_ERROR_RESILIENT_DEFAULT : 0;
  cfg.g_profile = profile;
  cfg.rc_end_usage = AOM_VBR;  // Use variable bitrate for testing

  // Step 6: Initialize encoder with aom_codec_enc_init_ver()
  aom_codec_ctx_t encoder;
  aom_codec_flags_t flags = 0;
  if (high_bitdepth) {
    flags |= AOM_CODEC_USE_HIGHBITDEPTH;
  }
  
  aom_codec_err_t init_err = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 
                                                    flags, AOM_ENCODER_ABI_VERSION);
  if (init_err != AOM_CODEC_OK) {
    return 0;
  }

  // Step 7: Create test image with aom_img_alloc()
  aom_image_t *test_image = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, width, height, 16);
  if (!test_image) {
    aom_codec_destroy(&encoder);
    return 0;
  }

  // Step 8: Test aom_img_plane_width() and aom_img_plane_height() - informational APIs
  // Test all planes (Y, U, V planes for I420 format)
  for (int plane = 0; plane < 3; plane++) {
    int plane_width = aom_img_plane_width(test_image, plane);
    int plane_height = aom_img_plane_height(test_image, plane);
    
    // These should return valid dimensions based on format and plane
    if (plane_width < 0 || plane_height < 0) {
      aom_img_free(test_image);
      aom_codec_destroy(&encoder);
      return 0;
    }
  }

  // Fill image with fuzzed data
  size_t fill_size = std::min(test_image->sz, image_data.size());
  if (fill_size > 0 && test_image->planes[0] != nullptr) {
    memcpy(test_image->planes[0], image_data.data(), fill_size);
  }

  // Step 9: Add metadata to image with aom_img_add_metadata()
  // Test multiple metadata entries with different insertion flags
  int metadata_err1 = aom_img_add_metadata(test_image, metadata_type1, 
                                           metadata_data1.data(), metadata_data1.size(),
                                           AOM_MIF_ANY_FRAME);
  int metadata_err2 = aom_img_add_metadata(test_image, metadata_type2,
                                           metadata_data2.data(), metadata_data2.size(),
                                           AOM_MIF_KEY_FRAME);
  
  // Metadata addition may fail for various reasons (e.g., memory allocation)
  // We continue testing regardless of success

  // Step 10: Test aom_img_num_metadata() - informational API
  size_t metadata_count = aom_img_num_metadata(test_image);
  // metadata_count should be 0, 1, or 2 depending on success of aom_img_add_metadata

  // Step 11: Test aom_img_get_metadata() - informational API
  // Try to retrieve each metadata entry if they exist
  for (size_t i = 0; i < metadata_count; i++) {
    const aom_metadata_t *metadata = aom_img_get_metadata(test_image, i);
    if (metadata) {
      // Metadata successfully retrieved
      // We can verify type and size match what we added
      // Note: metadata->type and metadata->sz are available
    }
  }

  // Step 12: Encode the image with aom_codec_encode()
  aom_codec_pts_t pts = 0;
  unsigned long duration = 1;
  int force_keyframe = fdp.ConsumeBool() ? AOM_EFLAG_FORCE_KF : 0;
  
  aom_codec_err_t encode_err = aom_codec_encode(&encoder, test_image, pts, duration, force_keyframe);
  
  if (encode_err == AOM_CODEC_OK) {
    // Step 13: Test aom_codec_get_preview_frame() - informational API
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
    if (preview_frame != nullptr) {
      // Preview frame successfully retrieved
      // Test plane dimensions on preview frame as well
      for (int plane = 0; plane < 3; plane++) {
        int preview_plane_width = aom_img_plane_width(preview_frame, plane);
        int preview_plane_height = aom_img_plane_height(preview_frame, plane);
        // These should be valid dimensions
      }
    }

    // Step 14: Test aom_codec_get_global_headers() - informational API
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers != nullptr) {
      // Global headers successfully retrieved
      // Verify header data if available
      if (global_headers->buf != nullptr && global_headers->sz > 0) {
        // Global headers contain valid data
      }
      // Free the global headers as required by documentation
      free(global_headers->buf);
      free(global_headers);
    }

    // Try to get compressed data to complete the encoding cycle
    const aom_codec_cx_pkt_t *pkt = nullptr;
    aom_codec_iter_t iter = nullptr;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
      // Process different packet types
      switch (pkt->kind) {
        case AOM_CODEC_CX_FRAME_PKT:
          // Frame packet successfully retrieved
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

    // Test metadata APIs again after encoding
    size_t post_encode_metadata_count = aom_img_num_metadata(test_image);
    // Count should match pre-encode count

    // Try flush encoding to test additional paths
    encode_err = aom_codec_encode(&encoder, nullptr, 0, 0, 0);
    if (encode_err == AOM_CODEC_OK) {
      // Test APIs again after flush
      preview_frame = aom_codec_get_preview_frame(&encoder);
      global_headers = aom_codec_get_global_headers(&encoder);
      if (global_headers != nullptr) {
        free(global_headers->buf);
        free(global_headers);
      }
    }
  }

  // Step 15: Cleanup
  // Note: aom_img_remove_metadata could be called here but aom_img_free handles cleanup
  aom_img_free(test_image);
  aom_codec_destroy(&encoder);

  return 0;
}

} // namespace harness_005
