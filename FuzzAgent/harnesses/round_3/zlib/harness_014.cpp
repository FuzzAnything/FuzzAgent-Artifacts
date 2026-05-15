#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_014 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful CRC testing
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    uLong crc1 = fdp.ConsumeIntegral<uLong>();
    uLong crc2 = fdp.ConsumeIntegral<uLong>();
    z_off_t len2 = fdp.ConsumeIntegral<z_off_t>();
    bool test_null_buffer = fdp.ConsumeBool();
    bool test_zero_length = fdp.ConsumeBool();
    
    // Determine buffer size for test data
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    if (buffer_size > fdp.remaining_bytes()) {
        buffer_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> test_buffer;
    if (buffer_size > 0) {
        test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    }
    
    const unsigned char* buf_ptr = test_buffer.data();
    uInt buf_len = static_cast<uInt>(test_buffer.size());
    z_size_t z_buf_len = static_cast<z_size_t>(test_buffer.size());
    
    // Test different scenarios based on fuzzed input
    switch (test_scenario) {
        case 0: {
            // Basic crc32() test
            uLong result = crc32(crc1, buf_ptr, buf_len);
            // Also test with NULL buffer (should return initial CRC)
            if (test_null_buffer) {
                uLong null_result = crc32(crc1, Z_NULL, 0);
                // The result should be crc1 when buf is NULL
            }
            break;
        }
        
        case 1: {
            // crc32_z() test (size_t length version)
            uLong result = crc32_z(crc1, buf_ptr, z_buf_len);
            // Test with zero length
            if (test_zero_length) {
                uLong zero_result = crc32_z(crc1, buf_ptr, 0);
                // Should return crc1 unchanged
            }
            break;
        }
        
        case 2: {
            // crc32_combine() test
            uLong result = crc32_combine(crc1, crc2, len2);
            // Test with negative len2 (should return 0)
            if (len2 < 0) {
                uLong neg_result = crc32_combine(crc1, crc2, len2);
                // Should return 0 according to documentation
            }
            break;
        }
        
        case 3: {
            // crc32_combine_gen() and crc32_combine_op() test
            uLong op = crc32_combine_gen(len2);
            if (op != 0) {  // Only valid if len2 >= 0
                uLong result = crc32_combine_op(crc1, crc2, op);
                // Should be equivalent to crc32_combine()
                uLong combine_result = crc32_combine(crc1, crc2, len2);
            }
            break;
        }
        
        case 4: {
            // Test crc32() with incremental computation
            if (buf_len > 4) {
                // Compute CRC in two parts
                uInt part1_len = buf_len / 2;
                uInt part2_len = buf_len - part1_len;
                
                uLong intermediate = crc32(crc1, buf_ptr, part1_len);
                uLong final_result = crc32(intermediate, buf_ptr + part1_len, part2_len);
                
                // Compare with single computation
                uLong single_result = crc32(crc1, buf_ptr, buf_len);
                // Should be equal
            }
            break;
        }
        
        case 5: {
            // Test with various initial CRC values from fuzzed input
            uLong test_crc1 = fdp.ConsumeIntegral<uLong>();
            uLong test_crc2 = fdp.ConsumeIntegral<uLong>();
            
            uLong crc_result1 = crc32(test_crc1, buf_ptr, buf_len);
            uLong crc_result2 = crc32(test_crc2, buf_ptr, buf_len);
            
            // Test with different lengths
            for (uInt i = 0; i < 4 && i < buf_len; i++) {
                uLong partial_result = crc32(test_crc1, buf_ptr, i);
            }
            break;
        }
        case 6: {
            // Test crc32_combine64() if available
            #ifdef Z_LARGE64
            z_off64_t len2_64_combine = static_cast<z_off64_t>(len2);
            uLong result64 = crc32_combine64(crc1, crc2, len2_64_combine);
            #endif
            
            // Test crc32_combine_gen64() if available
            #ifdef Z_LARGE64
            z_off64_t len2_64_gen = static_cast<z_off64_t>(len2);
            uLong op64 = crc32_combine_gen64(len2_64_gen);
            if (op64 != 0) {
                uLong result_op64 = crc32_combine_op(crc1, crc2, op64);
            }
            #endif
            break;
        }
        
        case 7: {
            // Comprehensive test of all CRC functions
            // Test basic crc32
            uLong basic_result = crc32(crc1, buf_ptr, buf_len);
            
            // Test crc32_z
            uLong z_result = crc32_z(crc1, buf_ptr, z_buf_len);
            
            // Test combine functions if len2 is non-negative
            if (len2 >= 0) {
                uLong combine_result = crc32_combine(crc1, crc2, len2);
                uLong gen_op = crc32_combine_gen(len2);
                uLong combine_op_result = crc32_combine_op(crc1, crc2, gen_op);
                
                // These should be equivalent
            }
            
            // Test with empty buffer
            if (test_zero_length) {
                uLong empty_result = crc32(crc1, Z_NULL, 0);
                // Should return crc1
            }
            
            // Test with single byte
            if (buf_len > 0) {
                uLong single_byte_result = crc32(crc1, buf_ptr, 1);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_014
