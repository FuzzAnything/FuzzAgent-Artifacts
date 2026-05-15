#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "aom/aom_codec.h"
#include "aom/aom_integer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_006 {
/*
 * Fuzzing harness for libaom AV1 OBU utility functions and error handling APIs
 * Targets high-impact uncovered APIs identified in coverage analysis:
 * 1. aom_obu_type_to_string - 20 branches (converts OBU_TYPE enum to string)
 * 2. aom_uleb_encode_fixed_size - 16 branches (ULEB128 encoding with fixed size)
 * 3. aom_codec_error_detail - 6 branches (retrieves detailed error messages)
 * 4. aom_codec_iface_name - 2 branches (gets interface name)
 * 5. aom_codec_get_caps - 2 branches (checks codec capabilities)
 * 
 * Target API sequence per coverage guidance:
 * 1. Initialize decoder/encoder using aom_codec_dec_init_ver/aom_codec_enc_init_ver
 * 2. Call aom_codec_get_caps to check codec capabilities
 * 3. Call aom_codec_iface_name to get interface name
 * 4. Process test bitstreams with aom_codec_decode/aom_codec_encode:
 *    - Include various OBU types (SEQUENCE_HEADER, FRAME_HEADER, FRAME, etc.)
 *    - Trigger error conditions (corrupt data, invalid parameters)
 * 5. For each OBU type encountered, call aom_obu_type_to_string to convert to string
 * 6. Test ULEB encoding with aom_uleb_encode_fixed_size on various values
 * 7. Check for errors and call aom_codec_error_detail when errors occur
 * 8. Use aom_codec_err_to_string and aom_codec_error for error reporting
 * 9. Clean up with aom_codec_destroy
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for basic configuration and testing
  if (size < 128) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Determine whether to test encoder or decoder
  bool test_encoder = fdp.ConsumeBool();
  
  // Step 2: Consume configuration parameters
  unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
  unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
  unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
  unsigned int allow_lowbitdepth = fdp.ConsumeBool() ? 1 : 0;
  
  // Step 3: Get codec interface
  aom_codec_iface_t *codec_iface = nullptr;
  if (test_encoder) {
    codec_iface = aom_codec_av1_cx();
  } else {
    codec_iface = aom_codec_av1_dx();
  }
  
  if (!codec_iface) {
    return 0;
  }

  // Step 4: Call aom_codec_iface_name (Target API #4)
  const char *iface_name = aom_codec_iface_name(codec_iface);
  (void)iface_name; // Use result to ensure coverage

  // Step 5: Call aom_codec_get_caps (Target API #5)
  unsigned int caps = aom_codec_get_caps(codec_iface);
  (void)caps; // Use result to ensure coverage

  // Step 6: Initialize codec context
  aom_codec_ctx_t codec;
  aom_codec_err_t init_result = AOM_CODEC_ERROR;
  
  if (test_encoder) {
    // Initialize encoder
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(codec_iface, &cfg, 0);
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<int>(100, 10000);
    
    init_result = aom_codec_enc_init(&codec, codec_iface, &cfg, 0);
  } else {
    // Initialize decoder
    aom_codec_dec_cfg_t cfg = { threads, width, height, allow_lowbitdepth };
    init_result = aom_codec_dec_init_ver(&codec, codec_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
  }

  // Step 7: Test error reporting APIs
  aom_codec_err_to_string(init_result);
  const char *init_error_str = aom_codec_error(&codec);
  (void)init_error_str;
  
  // Step 8: Call aom_codec_error_detail (Target API #3) if there's an error
  if (init_result != AOM_CODEC_OK) {
    const char *error_detail = aom_codec_error_detail(&codec);
    (void)error_detail;
    return 0;
  }

  // Step 9: Test aom_obu_type_to_string for all OBU types (Target API #1)
  // Test all possible OBU_TYPE enum values
  OBU_TYPE obu_types[] = {
    OBU_SEQUENCE_HEADER,
    OBU_TEMPORAL_DELIMITER,
    OBU_FRAME_HEADER,
    OBU_TILE_GROUP,
    OBU_METADATA,
    OBU_FRAME,
    OBU_REDUNDANT_FRAME_HEADER,
    OBU_TILE_LIST,
    OBU_PADDING
  };
  
  for (size_t i = 0; i < sizeof(obu_types) / sizeof(obu_types[0]); ++i) {
    const char *obu_type_str = aom_obu_type_to_string(obu_types[i]);
    (void)obu_type_str;
  }

  // Step 10: Test aom_uleb_encode_fixed_size (Target API #2)
  // Consume test values for ULEB encoding
  uint64_t uleb_value = fdp.ConsumeIntegral<uint64_t>();
  size_t pad_to_size = fdp.ConsumeIntegralInRange<size_t>(1, 10);
  size_t available_buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
  
  // Allocate buffer for encoded output
  std::vector<uint8_t> encoded_buffer(available_buffer_size);
  size_t coded_size = 0;
  
  // Call aom_uleb_encode_fixed_size
  int encode_result = aom_uleb_encode_fixed_size(
    uleb_value,
    available_buffer_size,
    pad_to_size,
    encoded_buffer.data(),
    &coded_size
  );
  (void)encode_result;

  // Step 11: Process bitstream data to trigger various OBU types and errors
  std::vector<uint8_t> bitstream_data = fdp.ConsumeRemainingBytes<uint8_t>();
  
  if (bitstream_data.size() > 0) {
    if (test_encoder) {
      // Test encoder path
      // Create a simple test image for encoding
      aom_image_t img;
      if (aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1)) {
        // Fill with some data
        size_t y_plane_size = width * height;
        size_t uv_plane_size = (width / 2) * (height / 2);
        
        // Use bitstream data to fill image planes
        size_t offset = 0;
        if (offset + y_plane_size <= bitstream_data.size()) {
          memcpy(img.planes[0], bitstream_data.data() + offset, y_plane_size);
          offset += y_plane_size;
        }
        if (offset + uv_plane_size <= bitstream_data.size()) {
          memcpy(img.planes[1], bitstream_data.data() + offset, uv_plane_size);
          offset += uv_plane_size;
        }
        if (offset + uv_plane_size <= bitstream_data.size()) {
          memcpy(img.planes[2], bitstream_data.data() + offset, uv_plane_size);
        }
        
        // Encode the image
        aom_codec_err_t encode_result = aom_codec_encode(&codec, &img, 0, 1, 0);
        aom_codec_err_to_string(encode_result);
        
        // Check for errors and call error detail
        if (encode_result != AOM_CODEC_OK) {
          const char *encode_error_detail = aom_codec_error_detail(&codec);
          (void)encode_error_detail;
        }
        
        aom_img_free(&img);
      }
    } else {
      // Test decoder path
      // Try to decode the bitstream data
      size_t decode_offset = 0;
      while (decode_offset < bitstream_data.size()) {
        size_t remaining = bitstream_data.size() - decode_offset;
        size_t chunk_size = std::min<size_t>(remaining, 1024);
        
        aom_codec_err_t decode_result = aom_codec_decode(
          &codec, 
          bitstream_data.data() + decode_offset, 
          chunk_size, 
          nullptr
        );
        
        aom_codec_err_to_string(decode_result);
        
        // Check for errors and call error detail
        if (decode_result != AOM_CODEC_OK) {
          const char *decode_error_detail = aom_codec_error_detail(&codec);
          (void)decode_error_detail;
        }
        
        // Try to get decoded frames to exercise more code paths
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img = nullptr;
        while ((img = aom_codec_get_frame(&codec, &iter)) != nullptr) {
          // Just iterate through frames
          (void)img;
        }
        
        decode_offset += chunk_size;
      }
    }
  }

  // Step 12: Test additional error conditions
  // Test aom_codec_error_detail with various states
  const char *final_error_detail = aom_codec_error_detail(&codec);
  (void)final_error_detail;
  
  // Test error string conversion for all error codes
  aom_codec_err_t error_codes[] = {
    AOM_CODEC_OK,
    AOM_CODEC_ERROR,
    AOM_CODEC_INVALID_PARAM,
    AOM_CODEC_UNSUP_BITSTREAM,
    AOM_CODEC_INCAPABLE,
    AOM_CODEC_MEM_ERROR
  };
  
  for (size_t i = 0; i < sizeof(error_codes) / sizeof(error_codes[0]); ++i) {
    aom_codec_err_to_string(error_codes[i]);
  }
  
  // Get final error state
  const char *final_error = aom_codec_error(&codec);
  (void)final_error;

  // Step 13: Clean up
  aom_codec_err_t destroy_result = aom_codec_destroy(&codec);
  aom_codec_err_to_string(destroy_result);

  return 0;
}

} // namespace harness_006
