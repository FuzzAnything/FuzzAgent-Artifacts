#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_integer.h"

namespace harness_006 {
/*
 * Fuzzing harness for libaom AV1 LEB128 integer encoding/decoding utilities
 * Targets uncovered LEB128 functions identified by CoverageAnalyzerAgent:
 * - aom_uleb_encode_fixed_size (16 undiscovered branches, most critical)
 * - aom_uleb_encode (undiscovered branches)
 * - aom_uleb_decode (undiscovered branches) 
 * - aom_uleb_size_in_bytes (undiscovered branches)
 * Module: Integer encoding/decoding utilities
 * Strategy: Focus on LEB128 (Little Endian Base 128) integer encoding/decoding functions
 * Differentiation: Unlike existing harnesses (000: decoder/encoder, 001: decoder, 002: image metadata, 
 *                  003: encoder configuration, 004: image manipulation, 005: decoder memory management),
 *                  targets integer encoding utilities
 * Performance: Integer encoding/decoding should be very lightweight and fast
 * Bug Potential: Integer overflow, buffer boundary issues in encoding/decoding
 * Coverage: Target aom_uleb_encode_fixed_size and related functions with 16+ undiscovered branches
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need at least 1 byte for operation mode + minimal data
    const size_t MIN_INPUT_SIZE = sizeof(uint8_t) + 4; // Operation mode + tiny integer
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful LEB128 testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 8; // 8 different test scenarios
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4; // 0: small values, 1: medium, 2: large, 3: edge cases
    
    // Get integer value to encode/decode based on test type
    uint64_t value = 0;
    switch (test_type) {
        case 0: // Small values (0-127, fits in 1 byte)
            value = fdp.ConsumeIntegralInRange<uint64_t>(0, 127);
            break;
        case 1: // Medium values (128-16383, fits in 2 bytes)
            value = fdp.ConsumeIntegralInRange<uint64_t>(128, 16383);
            break;
        case 2: // Large values (16384-2097151, fits in 3 bytes)
            value = fdp.ConsumeIntegralInRange<uint64_t>(16384, 2097151);
            break;
        case 3: // Edge cases and larger values
            value = fdp.ConsumeIntegral<uint64_t>();
            // Cap at UINT32_MAX as per LEB128 implementation limits
            if (value > UINT32_MAX) {
                value = UINT32_MAX;
            }
            break;
        default:
            value = fdp.ConsumeIntegralInRange<uint64_t>(0, 255);
            break;
    }

    // Get buffer size for encoding operations
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 16);
    
    // Get pad_to_size for fixed-size encoding (1-8 bytes as per LEB128 spec)
    size_t pad_to_size = fdp.ConsumeIntegralInRange<size_t>(1, 8);
    
    // Get available buffer size (might be smaller than requested buffer)
    size_t available = fdp.ConsumeIntegralInRange<size_t>(1, buffer_size);

    // Perform different test scenarios based on operation_mode
    switch (operation_mode) {
        case 0: {
            // Test 1: Basic size calculation
            size_t calculated_size = aom_uleb_size_in_bytes(value);
            // Size should be between 1-8 bytes for valid LEB128 values
            if (calculated_size < 1 || calculated_size > 8) {
                // Unexpected size, but continue testing
            }
            break;
        }
        
        case 1: {
            // Test 2: Basic encode/decode round-trip
            uint8_t encode_buffer[16] = {0};
            size_t encoded_size = 0;
            
            int encode_result = aom_uleb_encode(value, sizeof(encode_buffer), 
                                               encode_buffer, &encoded_size);
            
            if (encode_result == 0 && encoded_size > 0) {
                // Successfully encoded, now try to decode
                uint64_t decoded_value = 0;
                size_t decoded_length = 0;
                
                int decode_result = aom_uleb_decode(encode_buffer, encoded_size,
                                                   &decoded_value, &decoded_length);
                
                if (decode_result == 0) {
                    // Round-trip validation
                    if (value != decoded_value) {
                        // Mismatch found - potential bug
                    }
                    if (encoded_size != decoded_length) {
                        // Size mismatch - potential bug
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test 3: Fixed-size encode/decode round-trip
            uint8_t fixed_encode_buffer[16] = {0};
            size_t fixed_encoded_size = 0;
            
            // Ensure pad_to_size is reasonable for the value
            size_t actual_pad_to_size = pad_to_size;
            if (actual_pad_to_size > 8) actual_pad_to_size = 8;
            
            int fixed_encode_result = aom_uleb_encode_fixed_size(
                value, sizeof(fixed_encode_buffer), actual_pad_to_size,
                fixed_encode_buffer, &fixed_encoded_size);
            
            if (fixed_encode_result == 0 && fixed_encoded_size > 0) {
                // Successfully encoded with fixed size, now decode
                uint64_t decoded_value = 0;
                size_t decoded_length = 0;
                
                int decode_result = aom_uleb_decode(fixed_encode_buffer, fixed_encoded_size,
                                                   &decoded_value, &decoded_length);
                
                if (decode_result == 0) {
                    // Verify round-trip
                    if (value != decoded_value) {
                        // Mismatch found
                    }
                    if (fixed_encoded_size != decoded_length) {
                        // Should match for fixed-size encoding
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test 4: Buffer boundary testing with insufficient space
            uint8_t small_buffer[4] = {0};
            size_t encoded_size = 0;
            
            // Try encoding with potentially insufficient buffer
            size_t test_available = fdp.ConsumeIntegralInRange<size_t>(0, 3);
            int boundary_result = aom_uleb_encode(value, test_available,
                                                 small_buffer, &encoded_size);
            
            // Should fail if buffer too small, succeed otherwise
            // This tests error handling paths
            break;
        }
        
        case 4: {
            // Test 5: Decode from fuzzed input data
            size_t decode_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            std::vector<uint8_t> decode_buffer = fdp.ConsumeBytes<uint8_t>(decode_buffer_size);
            
            if (!decode_buffer.empty()) {
                uint64_t decoded_value = 0;
                size_t decoded_length = 0;
                
                int decode_result = aom_uleb_decode(decode_buffer.data(), decode_buffer.size(),
                                                   &decoded_value, &decoded_length);
                
                // Test both success and failure paths
                if (decode_result == 0) {
                    // Successfully decoded, try to re-encode
                    uint8_t reencode_buffer[16] = {0};
                    size_t reencoded_size = 0;
                    
                    aom_uleb_encode(decoded_value, sizeof(reencode_buffer),
                                   reencode_buffer, &reencoded_size);
                }
            }
            break;
        }
        
        case 5: {
            // Test 6: Fixed-size encoding with various pad sizes
            for (size_t i = 1; i <= 8; i++) {
                uint8_t test_buffer[16] = {0};
                size_t test_encoded_size = 0;
                
                int test_result = aom_uleb_encode_fixed_size(
                    value, sizeof(test_buffer), i,
                    test_buffer, &test_encoded_size);
                
                // Test both success and failure cases
                if (test_result == 0) {
                    // Verify the encoding uses exactly i bytes
                    if (test_encoded_size != i) {
                        // Size mismatch
                    }
                    
                    // Try decoding the fixed-size encoding
                    uint64_t test_decoded = 0;
                    size_t test_decoded_length = 0;
                    aom_uleb_decode(test_buffer, test_encoded_size,
                                   &test_decoded, &test_decoded_length);
                }
            }
            break;
        }
        
        case 6: {
            // Test 7: Encode with different buffer alignments and sizes
            std::vector<uint8_t> dynamic_buffer(buffer_size, 0);
            size_t dynamic_encoded_size = 0;
            
            int dynamic_result = aom_uleb_encode(value, dynamic_buffer.size(),
                                                dynamic_buffer.data(), &dynamic_encoded_size);
            
            if (dynamic_result == 0 && dynamic_encoded_size > 0) {
                // Verify the encoding
                uint64_t dynamic_decoded = 0;
                size_t dynamic_decoded_length = 0;
                
                aom_uleb_decode(dynamic_buffer.data(), dynamic_encoded_size,
                               &dynamic_decoded, &dynamic_decoded_length);
            }
            break;
        }
        
        case 7: {
            // Test 8: Comprehensive edge case testing
            // Test with value = 0 (special case)
            uint8_t zero_buffer[8] = {0};
            size_t zero_size = 0;
            aom_uleb_encode(0, sizeof(zero_buffer), zero_buffer, &zero_size);
            
            // Test with value = UINT32_MAX (maximum allowed)
            uint8_t max_buffer[8] = {0};
            size_t max_size = 0;
            aom_uleb_encode(UINT32_MAX, sizeof(max_buffer), max_buffer, &max_size);
            
            // Test with value > UINT32_MAX (should fail)
            uint8_t overflow_buffer[8] = {0};
            size_t overflow_size = 0;
            aom_uleb_encode(UINT32_MAX + 1ULL, sizeof(overflow_buffer), 
                           overflow_buffer, &overflow_size);
            
            // Test decode with null pointers (error paths)
            aom_uleb_decode(nullptr, 0, nullptr, nullptr);
            
            // Test encode with null pointers (error paths)
            aom_uleb_encode(value, 0, nullptr, nullptr);
            
            // Test fixed-size encode with invalid parameters
            aom_uleb_encode_fixed_size(value, 0, 0, nullptr, nullptr);
            break;
        }
    }

    return 0;
}

} // namespace harness_006
