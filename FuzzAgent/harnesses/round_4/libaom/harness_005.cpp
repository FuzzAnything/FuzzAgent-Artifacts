#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

namespace harness_005 {
/*
 * Fuzzing harness for libaom AV1 decoder frame buffer management and error reporting APIs
 * Targets decoder frame buffer management API (aom_codec_set_frame_buffer_functions) with 
 * 14 undiscovered branches, plus error reporting APIs (aom_codec_error, aom_codec_err_to_string)
 * with 46 branches total.
 * 
 * Target API sequence:
 * 1. aom_codec_av1_dx - Get AV1 decoder interface
 * 2. aom_codec_dec_init_ver - Initialize decoder context  
 * 3. Implement stub callback functions: get_frame_buffer_cb and release_frame_buffer_cb
 * 4. aom_codec_set_frame_buffer_functions - Register external frame buffer callbacks (UNCOVERED - 14 undiscovered branches)
 * 5. aom_codec_decode - Decode AV1 bitstream data
 * 6. aom_codec_get_frame - Retrieve decoded frames
 * 7. aom_codec_error - Get error string from decoder context (UNCOVERED - 24 undiscovered branches)
 * 8. aom_codec_err_to_string - Convert error code to string (UNCOVERED - 22 undiscovered branches)
 * 9. aom_codec_destroy - Clean up decoder resources
 * Additional Helper APIs:
 * - aom_img_alloc - Allocate image structures for callbacks
 * - aom_img_free - Free image structures
 */




// Simple external frame buffer structure for fuzzing
struct SimpleFrameBuffer {
  uint8_t *data;
  size_t size;
  int in_use;
};

struct SimpleFrameBufferList {
  int num_buffers;
  SimpleFrameBuffer *buffers;
};

// Callback used by libaom to request an external frame buffer
static int fuzzer_get_frame_buffer_cb(void *cb_priv, size_t min_size,
                                      aom_codec_frame_buffer_t *fb) {
  if (!cb_priv || !fb) return -1;
  
  SimpleFrameBufferList *fb_list = (SimpleFrameBufferList *)cb_priv;
  
  // Find a free frame buffer
  int free_index = -1;
  for (int i = 0; i < fb_list->num_buffers; ++i) {
    if (!fb_list->buffers[i].in_use) {
      free_index = i;
      break;
    }
  }
  
  if (free_index == -1) return -1;
  
  // Reallocate if current buffer is too small
  if (fb_list->buffers[free_index].size < min_size) {
    free(fb_list->buffers[free_index].data);
    fb_list->buffers[free_index].data = (uint8_t *)calloc(min_size, sizeof(uint8_t));
    if (!fb_list->buffers[free_index].data) return -1;
    fb_list->buffers[free_index].size = min_size;
  }
  
  fb->data = fb_list->buffers[free_index].data;
  fb->size = fb_list->buffers[free_index].size;
  fb_list->buffers[free_index].in_use = 1;
  fb->priv = &fb_list->buffers[free_index];
  
  return 0;
}

// Callback used by libaom when there are no references to the frame buffer
static int fuzzer_release_frame_buffer_cb(void *cb_priv,
                                          aom_codec_frame_buffer_t *fb) {
  if (!fb || !fb->priv) return -1;
  
  SimpleFrameBuffer *ext_fb = (SimpleFrameBuffer *)fb->priv;
  ext_fb->in_use = 0;
  (void)cb_priv;
  return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for basic decoder configuration
  if (size < 128) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume decoder configuration parameters from fuzzer input
  unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
  unsigned int allow_lowbitdepth = fdp.ConsumeBool() ? 1 : 0;
  
  // Consume number of external frame buffers (1-8, matching AOM_MAXIMUM_WORK_BUFFERS)
  int num_external_buffers = fdp.ConsumeIntegralInRange<int>(1, 8);
  
  // Consume whether to use external frame buffers
  bool use_external_buffers = fdp.ConsumeBool();
  
  // Consume remaining bytes as AV1 bitstream data
  std::vector<uint8_t> bitstream_data = fdp.ConsumeRemainingBytes<uint8_t>();
  if (bitstream_data.size() < 32) {
    // Need at least some data for decoding attempts
    return 0;
  }

  // Step 1: Get AV1 decoder interface
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  if (!decoder_iface) {
    return 0;
  }

  // Step 2: Initialize decoder configuration
  aom_codec_dec_cfg_t cfg = { threads, width, height, allow_lowbitdepth };
  
  // Step 3: Initialize decoder context
  aom_codec_ctx_t codec;
  aom_codec_err_t init_result = aom_codec_dec_init_ver(&codec, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
  
  // Exercise error reporting APIs early
  const char *init_error_str = aom_codec_error(&codec);
  (void)init_error_str; // Use the result to ensure coverage
  
  aom_codec_err_to_string(init_result);
  
  if (init_result != AOM_CODEC_OK) {
    // Even if init fails, we can still test error APIs
    aom_codec_err_to_string(init_result);
    const char *error_detail = aom_codec_error(&codec);
    (void)error_detail;
    return 0;
  }

  // Step 4: Set up external frame buffers if requested
  SimpleFrameBufferList fb_list = {0, nullptr};
  std::unique_ptr<SimpleFrameBuffer[]> buffers;
  
  if (use_external_buffers) {
    fb_list.num_buffers = num_external_buffers;
    buffers = std::make_unique<SimpleFrameBuffer[]>(num_external_buffers);
    fb_list.buffers = buffers.get();
    
    // Initialize buffers
    for (int i = 0; i < num_external_buffers; ++i) {
      size_t initial_size = 4096; // Reasonable starting size
      buffers[i].data = (uint8_t *)calloc(initial_size, sizeof(uint8_t));
      buffers[i].size = initial_size;
      buffers[i].in_use = 0;
    }
    
    // Step 5: Register external frame buffer functions
    aom_codec_err_t set_fb_result = aom_codec_set_frame_buffer_functions(
        &codec, fuzzer_get_frame_buffer_cb, fuzzer_release_frame_buffer_cb, &fb_list);
    
    // Exercise error reporting for this API
    aom_codec_err_to_string(set_fb_result);
    const char *fb_error_str = aom_codec_error(&codec);
    (void)fb_error_str;
    
    // Continue even if external buffers fail - decoder should still work with internal buffers
  }

  // Step 6: Decode the bitstream data
  // Try multiple decode attempts with different sizes to exercise various paths
  size_t decode_offset = 0;
  size_t decode_chunk_size = std::min<size_t>(bitstream_data.size(), 1024);
  
  while (decode_offset < bitstream_data.size()) {
    size_t remaining = bitstream_data.size() - decode_offset;
    size_t chunk_size = std::min(decode_chunk_size, remaining);
    
    aom_codec_err_t decode_result = aom_codec_decode(
        &codec, bitstream_data.data() + decode_offset, chunk_size, nullptr);
    
    // Exercise error reporting for decode
    aom_codec_err_to_string(decode_result);
    const char *decode_error_str = aom_codec_error(&codec);
    (void)decode_error_str;
    
    // Step 7: Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    int frames_retrieved = 0;
    
    while ((img = aom_codec_get_frame(&codec, &iter)) != nullptr) {
      frames_retrieved++;
      
      // Test helper API: aom_img_alloc and aom_img_free
      // Create a temporary image to test these APIs
      if (frames_retrieved == 1) { // Only do this once to avoid excessive allocation
        aom_image_t temp_img;
        aom_img_fmt_t format = AOM_IMG_FMT_I420;
        
        // Test aom_img_alloc
        aom_image_t *allocated_img = aom_img_alloc(&temp_img, format, 
                                                   img->d_w > 0 ? img->d_w : 16,
                                                   img->d_h > 0 ? img->d_h : 16,
                                                   1);
        if (allocated_img) {
          // Test aom_img_free
          aom_img_free(allocated_img);
        }
      }
    }
    
    decode_offset += chunk_size;
    
    // Consume more configuration from remaining fuzzer data for next iteration
    if (decode_offset < bitstream_data.size() && fdp.remaining_bytes() > 0) {
      // Update chunk size for next iteration
      decode_chunk_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
    }
  }

  // Step 8: Exercise error reporting APIs with various error codes
  // Test aom_codec_err_to_string with all possible error codes
  aom_codec_err_t error_codes[] = {
    AOM_CODEC_OK,
    AOM_CODEC_ERROR,
    AOM_CODEC_INVALID_PARAM,
    AOM_CODEC_UNSUP_BITSTREAM,
    AOM_CODEC_INCAPABLE,
    AOM_CODEC_MEM_ERROR
  };
  
  for (size_t i = 0; i < sizeof(error_codes) / sizeof(error_codes[0]); ++i) {
    const char *error_str = aom_codec_err_to_string(error_codes[i]);
    (void)error_str;
  }
  
  // Get final error state
  const char *final_error = aom_codec_error(&codec);
  (void)final_error;

  // Step 9: Clean up external frame buffers
  if (use_external_buffers && buffers) {
    for (int i = 0; i < num_external_buffers; ++i) {
      free(buffers[i].data);
    }
  }

  // Step 10: Destroy decoder
  aom_codec_err_t destroy_result = aom_codec_destroy(&codec);
  aom_codec_err_to_string(destroy_result);

  return 0;
}

} // namespace harness_005
