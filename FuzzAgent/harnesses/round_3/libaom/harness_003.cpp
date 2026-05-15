#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"

namespace harness_003 {
/*
 * Fuzzing harness for libaom AV1 decoder error handling, OBU utilities, and ULEB encoding
 * Targets decoder error reporting APIs and utility functions with high undiscovered branch counts:
 * - aom_codec_error (24 undiscovered branches)
 * - aom_codec_err_to_string (22 undiscovered branches) 
 * - aom_codec_error_detail (20 undiscovered branches)
 * - aom_obu_type_to_string (16 undiscovered branches)
 * - aom_uleb_encode_fixed_size (14 undiscovered branches)
 * - aom_codec_set_frame_buffer_functions (10 undiscovered branches)
 * - aom_codec_get_stream_info (8 undiscovered branches)
 * - aom_codec_set_option (6 undiscovered branches)
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_003_guidance.md
 * Semantic differentiation: harness_000 targets basic decoder/encoder,
 * harness_001 targets image metadata, harness_002 targets encoder configuration.
 * This harness focuses on decoder error handling and utility functions.
 */



// Frame buffer callback functions for aom_codec_set_frame_buffer_functions
static int get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    // Always fail to trigger error paths
    return -1;
}

static int release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Basic decoder setup (config, flags): ~20 bytes
    // - Option name/value strings: ~64 bytes  
    // - Invalid/corrupted bitstream data: ~100 bytes
    // - OBU type values: ~4 bytes
    // - ULEB values: ~8 bytes
    // Total minimum: ~196 bytes
    if (size < 200) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Get AV1 decoder interface
    aom_codec_iface_t* iface = aom_codec_av1_dx();
    if (iface == nullptr) {
        return 0;
    }

    // Step 2: Initialize decoder context
    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    // Consume decoder configuration parameters
    aom_codec_dec_cfg_t cfg;
    cfg.threads = fdp.ConsumeIntegralInRange<int>(0, 16);
    cfg.w = fdp.ConsumeIntegralInRange<unsigned int>(0, 4096);
    cfg.h = fdp.ConsumeIntegralInRange<unsigned int>(0, 4096);
    cfg.allow_lowbitdepth = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume initialization flags
    aom_codec_flags_t flags = static_cast<aom_codec_flags_t>(
        fdp.ConsumeIntegral<uint32_t>());
    
    // Initialize decoder
    aom_codec_err_t init_res = aom_codec_dec_init_ver(&codec, iface, &cfg, flags, AOM_DECODER_ABI_VERSION);
    
    // Check initialization result - continue even if it fails to test error paths
    if (init_res != AOM_CODEC_OK) {
        // Test error reporting functions even on failed initialization
        (void)aom_codec_error(&codec);
        (void)aom_codec_error_detail(&codec);
        
        // Try to get error string from error code
        const char* err_to_str = aom_codec_err_to_string(init_res);
        
        // Cleanup if partially initialized
        if (codec.name != nullptr) {
            aom_codec_destroy(&codec);
        }
        
        // Still test OBU and ULEB utilities even if decoder init failed
        // Step 10: Test OBU utilities
        for (int i = 0; i < 10 && fdp.remaining_bytes() > 1; i++) {
            uint8_t obu_type_val = fdp.ConsumeIntegral<uint8_t>();
            OBU_TYPE obu_type = static_cast<OBU_TYPE>(obu_type_val & 0x0F); // 4-bit OBU type
            const char* obu_str = aom_obu_type_to_string(obu_type);
        }
        
        // Step 11: Test ULEB encoding
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 4; i++) {
            uint64_t value = fdp.ConsumeIntegral<uint64_t>();
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            std::vector<uint8_t> buffer(buffer_size);
            
            size_t encoded_size = 0;
            size_t coded_size = 0;
            int encode_result = aom_uleb_encode_fixed_size(value, buffer_size, buffer_size, buffer.data(), &coded_size);
            if (encode_result == 0) {
                encoded_size = coded_size;
            }
        }
        
        return 0;
    }
    // Consume option name and value from fuzzer input
    std::string option_name = fdp.ConsumeRandomLengthString(32);
    std::string option_value = fdp.ConsumeRandomLengthString(64);
    
    aom_codec_set_option(&codec, option_name.c_str(), option_value.c_str());
    
    // Set another option with different values
    if (fdp.remaining_bytes() > 50) {
        std::string option_name2 = fdp.ConsumeRandomLengthString(16);
        std::string option_value2 = fdp.ConsumeRandomLengthString(32);
        aom_codec_set_option(&codec, option_name2.c_str(), option_value2.c_str());
    }
    
    // Step 4: Set custom frame buffer callbacks
    void* cb_priv = nullptr;
    aom_codec_set_frame_buffer_functions(&codec, get_frame_buffer, release_frame_buffer, cb_priv);
    
    // Step 5: Decode corrupted/invalid data to trigger errors
    // Use remaining fuzzer input as "bitstream" data
    size_t decode_data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(4096, fdp.remaining_bytes()));
    std::vector<uint8_t> decode_data = fdp.ConsumeBytes<uint8_t>(decode_data_size);
    
    // Call decode with potentially invalid data
    aom_codec_decode(&codec, decode_data.data(), decode_data.size(), nullptr);
    
    // Step 6: Retrieve error code
    (void)aom_codec_error(&codec);
    
    // Step 7: Convert error to string (using the error from context)
    // Also test with specific error codes from fuzzer input
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 1; i++) {
        uint8_t err_code_val = fdp.ConsumeIntegral<uint8_t>();
        aom_codec_err_t test_err = static_cast<aom_codec_err_t>(err_code_val % 10); // Map to valid range
        const char* err_str = aom_codec_err_to_string(test_err);
    }
    
    // Step 8: Get detailed error information
    (void)aom_codec_error_detail(&codec);
    
    // Step 9: Attempt to get stream info (may fail due to errors)
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(iface, decode_data.data(), decode_data.size(), &stream_info);
    
    // Step 10: Test OBU utilities
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 1; i++) {
        uint8_t obu_type_val = fdp.ConsumeIntegral<uint8_t>();
        OBU_TYPE obu_type = static_cast<OBU_TYPE>(obu_type_val & 0x0F); // 4-bit OBU type
        const char* obu_str = aom_obu_type_to_string(obu_type);
    }
    
    // Step 11: Test ULEB encoding with various values
    for (int i = 0; i < 8 && fdp.remaining_bytes() > 4; i++) {
        uint64_t value = fdp.ConsumeIntegral<uint64_t>();
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        
        // Allocate buffer
        std::vector<uint8_t> buffer(buffer_size);
        
        // Encode ULEB value
        size_t encoded_size = 0;
        size_t coded_size = 0;
        int encode_result = aom_uleb_encode_fixed_size(value, buffer_size, buffer_size, buffer.data(), &coded_size);
        if (encode_result == 0) {
            encoded_size = coded_size;
        }
        
        // If encoding succeeded and we have space, test encoding again with different size
        if (encoded_size > 0 && encoded_size <= buffer_size && fdp.remaining_bytes() > 2) {
            size_t new_buffer_size = fdp.ConsumeIntegralInRange<size_t>(encoded_size, encoded_size * 2);
            std::vector<uint8_t> buffer2(new_buffer_size);
            size_t encoded_size2 = 0;
            size_t coded_size2 = 0;
            int encode_result2 = aom_uleb_encode_fixed_size(value, new_buffer_size, new_buffer_size, buffer2.data(), &coded_size2);
            if (encode_result2 == 0) {
                encoded_size2 = coded_size2;
            }
        }
    }
    
    // Step 12: Clean up decoder context
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_003
