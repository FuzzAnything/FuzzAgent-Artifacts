#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"

namespace harness_002 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Targeted fuzzing harness for libaom common utilities and error handling
 * Focuses on common module with 0% coverage and error handling/config APIs
 * Targets: aom_codec_error, aom_codec_err_to_string, aom_codec_error_detail,
 *          aom_codec_set_option, aom_obu_type_to_string, aom_uleb_encode_fixed_size
 * Semantic differentiation from previous harnesses (000: decoder/encoder, 001: encoder operations)
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum input size needed for meaningful testing
  const size_t MIN_INPUT_SIZE = 64;
  if (size < MIN_INPUT_SIZE) {
    return 0;  // Insufficient input for meaningful testing
  }

  FuzzedDataProvider fdp(data, size);

  // Step 1: Test error handling APIs with codec contexts
  
  // First, test aom_codec_err_to_string with various error codes
  // Consume an error code from fuzzed input
  int err_code = fdp.ConsumeIntegralInRange<int>(0, 20);
  const char* err_str = aom_codec_err_to_string(static_cast<aom_codec_err_t>(err_code));
  // err_str may be NULL for invalid error codes, that's OK
  
  // Step 2: Create codec contexts to trigger errors and test error APIs
  
  aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
  aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
  
  // Test with invalid initialization to trigger errors
  aom_codec_ctx_t decoder;
  aom_codec_ctx_t encoder;
  
  // Try to initialize decoder with NULL config to trigger error
  aom_codec_err_t dec_init_err = aom_codec_dec_init(&decoder, decoder_iface, NULL, 0);
  
  // Get error information if initialization failed
  if (dec_init_err != AOM_CODEC_OK) {
    const char* decoder_error = aom_codec_error(&decoder);
    const char* decoder_error_detail = aom_codec_error_detail(&decoder);
    // These may be NULL or empty, that's fine for fuzzing
  }
  
  // Clean up if initialization succeeded (unlikely with NULL config)
  if (dec_init_err == AOM_CODEC_OK) {
    aom_codec_destroy(&decoder);
  }
  
  // Step 3: Test aom_codec_set_option with fuzzed key-value pairs
  
  // Initialize encoder properly to test set_option
  aom_codec_enc_cfg_t enc_cfg;
  if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
    aom_codec_flags_t flags = 0;
    if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, flags) == AOM_CODEC_OK) {
      // Consume option name and value from fuzzed input
      std::string option_name = fdp.ConsumeRandomLengthString(32);
      std::string option_value = fdp.ConsumeRandomLengthString(64);
      
      // Try to set the option
      aom_codec_err_t set_option_err = aom_codec_set_option(&encoder, 
                                                           option_name.c_str(), 
                                                           option_value.c_str());
      
      // Test error APIs after set_option attempt
      const char* encoder_error = aom_codec_error(&encoder);
      const char* encoder_error_detail = aom_codec_error_detail(&encoder);
      
      aom_codec_destroy(&encoder);
    }
  }
  
  // Step 4: Test OBU utilities
  
  // Test aom_obu_type_to_string with fuzzed OBU type
  int obu_type_value = fdp.ConsumeIntegralInRange<int>(0, 20);
  const char* obu_type_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(obu_type_value));
  
  // Step 5: Test aom_uleb_encode_fixed_size with fuzzed values
  
  // Consume parameters for uleb encoding
  uint64_t uleb_value = fdp.ConsumeIntegral<uint64_t>();
  size_t available_space = fdp.ConsumeIntegralInRange<size_t>(1, 256);
  size_t pad_to_size = fdp.ConsumeIntegralInRange<size_t>(1, available_space);
  
  // Allocate buffer for encoded output
  std::vector<uint8_t> encoded_buffer(pad_to_size);
  size_t coded_size = 0;
  
  // Try to encode
  int encode_result = aom_uleb_encode_fixed_size(uleb_value, available_space,
                                                pad_to_size, encoded_buffer.data(),
                                                &coded_size);
  
  // Step 6: Test additional error scenarios
  
  // Test with partially consumed data to trigger edge cases
  if (fdp.remaining_bytes() > 16) {
    // Create another codec context with some valid config
    aom_codec_dec_cfg_t dec_cfg_valid = {1, 0, 0, 1};
    aom_codec_ctx_t decoder2;
    
    if (aom_codec_dec_init(&decoder2, decoder_iface, &dec_cfg_valid, 0) == AOM_CODEC_OK) {
      // Test error APIs on valid context (should return empty strings or NULL)
      const char* valid_error = aom_codec_error(&decoder2);
      const char* valid_error_detail = aom_codec_error_detail(&decoder2);
      
      // Test set_option with empty strings
      aom_codec_set_option(&decoder2, "", "");
      
      aom_codec_destroy(&decoder2);
    }
  }
  
  return 0;
}

} // namespace harness_002
