#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_codec.h"
#include "aom/aom_integer.h"

namespace harness_006 {
/*
 * Fuzzing harness for libaom AV1 decoder initialization, OBU processing, and ULEB encoding
 * Targets coverage gaps identified in decoder initialization, OBU utilities, and ULEB encoding
 * APIs: aom_codec_dec_init_ver, aom_obu_type_to_string, aom_codec_set_option, aom_uleb_encode_fixed_size
 * Helper APIs: aom_codec_av1_dx, aom_codec_decode, aom_codec_destroy, aom_uleb_encode, aom_uleb_decode
 * Follows exact invocation sequence from coverage guidance
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for meaningful testing of all targeted APIs
  // We need: decoder config (4 integers) + option strings + ULEB values + bitstream
  if (size < 256) {
    return 0;  // Need enough data for comprehensive testing
  }

  FuzzedDataProvider fdp(data, size);

  // ====================================================================
  // PHASE 1: Initialize decoder using aom_codec_av1_dx and aom_codec_dec_init_ver
  // ====================================================================
  
  // Get AV1 decoder interface
  aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
  if (!decoder_iface) {
    return 0;
  }

  // Initialize decoder configuration from fuzzer input
  aom_codec_dec_cfg_t cfg;
  cfg.threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);  // Threads [1, 64]
  cfg.w = 0;  // Width will be determined from stream
  cfg.h = 0;  // Height will be determined from stream
  cfg.allow_lowbitdepth = fdp.ConsumeBool() ? 1 : 0;  // Allow low bitdepth decoding

  // Initialize decoder context with aom_codec_dec_init_ver (direct API call)
  aom_codec_ctx_t codec;
  aom_codec_err_t res = aom_codec_dec_init_ver(&codec, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
  
  // If decoder initialization fails, we can still test some APIs but not all
  bool decoder_initialized = (res == AOM_CODEC_OK);
  
  if (decoder_initialized) {
    // ====================================================================
    // PHASE 2: Test configuration options with aom_codec_set_option
    // ====================================================================
    
    // Generate random option names and values from fuzzer input
    std::string option_name1 = fdp.ConsumeRandomLengthString(32);
    std::string option_value1 = fdp.ConsumeRandomLengthString(64);
    
    // Try to set random options (may fail, but that's OK - we're testing the API)
    aom_codec_set_option(&codec, option_name1.c_str(), option_value1.c_str());
    
    // Try some known valid options
    if (fdp.ConsumeBool()) {
      aom_codec_set_option(&codec, "threads", std::to_string(cfg.threads).c_str());
    }
    if (fdp.ConsumeBool()) {
      aom_codec_set_option(&codec, "error-resilient", fdp.ConsumeBool() ? "1" : "0");
    }

    // ====================================================================
    // PHASE 3: Process OBU data with aom_obu_type_to_string
    // ====================================================================
    
    // Test all possible OBU_TYPE values
    for (int obu_type_val = OBU_SEQUENCE_HEADER; obu_type_val <= OBU_PADDING; obu_type_val++) {
      // Skip invalid values in the enum range
      if (obu_type_val == 0 || (obu_type_val > 8 && obu_type_val < 15)) {
        continue;
      }
      OBU_TYPE obu_type = static_cast<OBU_TYPE>(obu_type_val);
      const char *obu_type_str = aom_obu_type_to_string(obu_type);
      // Just call the function - we don't need to use the return value
      (void)obu_type_str;
    }
    
    // Also test random OBU types from fuzzer input
    uint8_t random_obu_type = fdp.ConsumeIntegral<uint8_t>() % 16;
    // Only use valid OBU type values
    if (random_obu_type == OBU_SEQUENCE_HEADER || random_obu_type == OBU_TEMPORAL_DELIMITER ||
        random_obu_type == OBU_FRAME_HEADER || random_obu_type == OBU_TILE_GROUP ||
        random_obu_type == OBU_METADATA || random_obu_type == OBU_FRAME ||
        random_obu_type == OBU_REDUNDANT_FRAME_HEADER || random_obu_type == OBU_TILE_LIST ||
        random_obu_type == OBU_PADDING) {
      OBU_TYPE random_type = static_cast<OBU_TYPE>(random_obu_type);
      const char *random_obu_str = aom_obu_type_to_string(random_type);
      (void)random_obu_str;
    }

    // ====================================================================
    // PHASE 5: Decode sample AV1 streams with aom_codec_decode
    // ====================================================================
    
    // Consume a portion of remaining input for bitstream testing
    size_t bitstream_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    std::vector<uint8_t> bitstream_data = fdp.ConsumeBytes<uint8_t>(bitstream_size);
    
    if (!bitstream_data.empty()) {
      const uint8_t *bitstream = bitstream_data.data();
      
      // Try to decode the bitstream
      aom_codec_decode(&codec, bitstream, bitstream_data.size(), nullptr);
      
      // Note: We don't retrieve frames to keep the harness lightweight
    }
    
    // ====================================================================
    // PHASE 6: Cleanup with aom_codec_destroy
    // ====================================================================
    aom_codec_destroy(&codec);
  }
  
  // ====================================================================
  // PHASE 4: Test ULEB encoding/decoding functions
  // (Can be tested regardless of decoder initialization status)
  // ====================================================================
  
  // We need separate buffers for ULEB testing
  // Allocate from remaining fuzzer input
  
  // Test aom_uleb_encode_fixed_size
  uint64_t uleb_value = fdp.ConsumeIntegral<uint64_t>();
  size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 16);
  size_t pad_to_size = fdp.ConsumeIntegralInRange<size_t>(1, buffer_size);
  
  std::vector<uint8_t> encode_buffer(buffer_size, 0);
  size_t coded_size = 0;
  
  // Test aom_uleb_encode_fixed_size
  int encode_result = aom_uleb_encode_fixed_size(uleb_value, buffer_size, pad_to_size, 
                                                 encode_buffer.data(), &coded_size);
  (void)encode_result; // Result may be success or failure - both are valid tests
  
  // Test aom_uleb_encode (regular encoding)
  uint64_t uleb_value2 = fdp.ConsumeIntegral<uint64_t>();
  size_t buffer_size2 = fdp.ConsumeIntegralInRange<size_t>(1, 16);
  
  std::vector<uint8_t> encode_buffer2(buffer_size2, 0);
  size_t coded_size2 = 0;
  
  int encode_result2 = aom_uleb_encode(uleb_value2, buffer_size2, encode_buffer2.data(), &coded_size2);
  (void)encode_result2;
  
  // Test aom_uleb_decode
  if (coded_size2 > 0 && encode_result2 == 0) {
    uint64_t decoded_value = 0;
    size_t decoded_length = 0;
    
    int decode_result = aom_uleb_decode(encode_buffer2.data(), coded_size2, 
                                       &decoded_value, &decoded_length);
    (void)decode_result;
  }
  
  // Also test aom_uleb_size_in_bytes
  uint64_t size_test_value = fdp.ConsumeIntegral<uint64_t>();
  size_t uleb_size = aom_uleb_size_in_bytes(size_test_value);
  (void)uleb_size;
  
  // Test decode on random data from fuzzer input
  size_t decode_buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
  std::vector<uint8_t> decode_buffer = fdp.ConsumeBytes<uint8_t>(decode_buffer_size);
  if (!decode_buffer.empty()) {
    uint64_t decoded_value2 = 0;
    size_t decoded_length2 = 0;
    
    int decode_result2 = aom_uleb_decode(decode_buffer.data(), decode_buffer.size(),
                                        &decoded_value2, &decoded_length2);
    (void)decode_result2;
  }

  return 0;
}

} // namespace harness_006
