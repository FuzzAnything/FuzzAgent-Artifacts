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
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"

namespace harness_004 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Targeted fuzzing harness for libaom AV1 decoder frame buffer management 
 * and metadata operations. Focuses on decoder subsystem with comprehensive 
 * coverage of frame buffer callbacks and image metadata APIs:
 * - aom_codec_set_frame_buffer_functions (14 branches, 0% covered)
 * - aom_img_get_metadata (6 branches, 0% covered)  
 * - aom_img_num_metadata (4 branches, 0% covered)
 * Semantic differentiation from previous harnesses:
 * - harness_000: Broad decoder/encoder coverage
 * - harness_001: Basic encoder operations
 * - harness_002: Common utilities and error handling  
 * - harness_003: Advanced encoder configuration and buffer management
 * - harness_004: Decoder frame buffer management and metadata operations
 */



// Custom frame buffer callback functions for external frame buffer management
static int get_frame_buffer_cb(void *priv, size_t min_size, 
                               aom_codec_frame_buffer_t *fb) {
  // Allocate frame buffer with requested minimum size
  fb->data = static_cast<uint8_t*>(malloc(min_size));
  if (!fb->data) {
    return -1;  // Allocation failure
  }
  
  // Zero out the allocated buffer as required by the API
  memset(fb->data, 0, min_size);
  fb->size = min_size;
  
  // Store allocation info in priv for tracking/release
  fb->priv = priv;
  return 0;  // Success
}

static int release_frame_buffer_cb(void *priv, aom_codec_frame_buffer_t *fb) {
  // Free the allocated frame buffer
  if (fb->data) {
    free(fb->data);
    fb->data = nullptr;
  }
  fb->size = 0;
  fb->priv = nullptr;
  return 0;  // Success
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum input size needed for decoder initialization and metadata operations
  // Need enough for: config params, bitstream data, metadata type/data
  const size_t MIN_INPUT_SIZE = 128;
  if (size < MIN_INPUT_SIZE) {
    return 0;  // Insufficient input for meaningful testing
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Consume decoder configuration parameters from fuzzed input
  // Fixed-size parameters first
  unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
  bool allow_lowbitdepth = fdp.ConsumeBool();
  bool is_annexb = fdp.ConsumeBool();
  
  // Consume metadata parameters
  uint32_t metadata_type = fdp.ConsumeIntegral<uint32_t>();
  size_t metadata_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
  
  // Get remaining data for AV1 bitstream
  std::vector<uint8_t> bitstream_data = fdp.ConsumeRemainingBytes<uint8_t>();
  if (bitstream_data.size() < 32) {
    return 0;  // Not enough data for meaningful bitstream
  }

  // Step 2: Get decoder interface
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  if (!decoder_iface) {
    return 0;
  }

  // Step 3: Initialize decoder with aom_codec_dec_init_ver()
  aom_codec_ctx_t decoder;
  aom_codec_dec_cfg_t dec_cfg = {threads, 0, 0, allow_lowbitdepth ? 1u : 0u};
  aom_codec_flags_t flags = 0;
  
  aom_codec_err_t init_err = aom_codec_dec_init_ver(&decoder, decoder_iface, 
                                                    &dec_cfg, flags, 
                                                    AOM_DECODER_ABI_VERSION);
  if (init_err != AOM_CODEC_OK) {
    return 0;
  }

  // Set decoder controls based on fuzzed data
  AOM_CODEC_CONTROL_TYPECHECKED(&decoder, AV1D_SET_IS_ANNEXB, is_annexb ? 1 : 0);

  // Step 4: Allocate metadata structure with aom_img_metadata_alloc()
  // Create metadata data from consumed bytes or use dummy data
  std::vector<uint8_t> metadata_buffer(metadata_size);
  if (fdp.remaining_bytes() >= metadata_size) {
    // Use actual fuzzed data for metadata
    auto metadata_data = fdp.ConsumeBytes<uint8_t>(metadata_size);
    memcpy(metadata_buffer.data(), metadata_data.data(), metadata_size);
  }
  
  aom_metadata_t *metadata = aom_img_metadata_alloc(
      metadata_type, metadata_buffer.data(), metadata_size, 
      static_cast<aom_metadata_insert_flags_t>(AOM_MIF_ANY_FRAME));
  
  if (!metadata) {
    // Clean up and return if metadata allocation fails
    aom_codec_destroy(&decoder);
    return 0;
  }

  // Step 5: Set custom frame buffer callbacks with aom_codec_set_frame_buffer_functions()
  // Create callback private data (simple integer for tracking)
  int callback_priv = 42;  // Arbitrary value for callback private data
  
  aom_codec_err_t fb_err = aom_codec_set_frame_buffer_functions(
      &decoder, get_frame_buffer_cb, release_frame_buffer_cb, &callback_priv);
  
  // Continue even if frame buffer functions fail - test both code paths
  // Some decoders may not support external frame buffers

  // Step 6: Decode AV1 bitstream with aom_codec_decode()
  size_t decode_size = std::min(bitstream_data.size(), static_cast<size_t>(4096));
  aom_codec_err_t decode_err = aom_codec_decode(&decoder, 
                                                bitstream_data.data(), 
                                                decode_size, 
                                                nullptr);

  // Step 7: Retrieve decoded frames with aom_codec_get_frame() loop
  if (decode_err == AOM_CODEC_OK) {
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = nullptr;
    int frame_count = 0;
    
    while ((decoded_img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
      frame_count++;
      
      // Step 8: Query metadata with aom_img_num_metadata() and aom_img_get_metadata()
      // Check if image has metadata array
      if (decoded_img->metadata) {
        size_t num_metadata = aom_img_num_metadata(decoded_img);
        
        // Try to get each metadata item
        for (size_t i = 0; i < num_metadata; i++) {
          const aom_metadata_t *img_metadata = aom_img_get_metadata(decoded_img, i);
          if (img_metadata) {
            // Metadata successfully retrieved, can access:
            // img_metadata->type, img_metadata->payload, img_metadata->sz, img_metadata->insert_flag
          }
        }
      }
      
      // Step 9: Add test metadata with aom_img_add_metadata()
      // Add our allocated metadata to the decoded image
      int add_metadata_err = aom_img_add_metadata(decoded_img, 
                                                  metadata->type,
                                                  metadata->payload,
                                                  metadata->sz,
                                                  metadata->insert_flag);
      
      // Add another metadata item with different type from fuzzed input
      if (fdp.remaining_bytes() > 0) {
        uint32_t extra_metadata_type = fdp.ConsumeIntegral<uint32_t>();
        size_t extra_metadata_size = std::min(fdp.remaining_bytes(), static_cast<size_t>(256));
        std::vector<uint8_t> extra_metadata_data = fdp.ConsumeBytes<uint8_t>(extra_metadata_size);
        
        if (extra_metadata_size > 0) {
          aom_img_add_metadata(decoded_img, 
                              extra_metadata_type,
                              extra_metadata_data.data(),
                              extra_metadata_size,
                              static_cast<aom_metadata_insert_flags_t>(AOM_MIF_ANY_FRAME));
        }
      }
      
      // Query metadata count again after adding new metadata
      if (decoded_img->metadata) {
        size_t updated_num_metadata = aom_img_num_metadata(decoded_img);
        // Can compare with previous count to verify addition worked
      }
      
      // Limit to processing a reasonable number of frames
      if (frame_count >= 10) {
        break;
      }
    }
  }

  // Step 10: Cleanup
  // Free allocated metadata
  aom_img_metadata_free(metadata);
  
  // Destroy decoder context
  aom_codec_destroy(&decoder);

  return 0;
}

} // namespace harness_004
