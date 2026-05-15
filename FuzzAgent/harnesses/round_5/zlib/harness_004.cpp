#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_004 {
/*
 * Fuzzing harness for zlib CRC32 checksum combination operations
 * Targets: crc32_combine, crc32_combine_op, crc32_combine_gen, get_crc_table
 * Helper APIs: crc32, crc32_z
 * 
 * This harness exercises the complete CRC32 combination workflow:
 * 1. Generate random data segments from fuzzer input
 * 2. Compute CRC32 for segment A using crc32() or crc32_z()
 * 3. Compute CRC32 for segment B using crc32() or crc32_z()
 * 4. Combine the CRCs using crc32_combine()
 * 5. Optionally test crc32_combine_gen() to generate combination operator
 * 6. Optionally test crc32_combine_op() for direct combination
 * 7. Test get_crc_table() to retrieve CRC table
 * 
 * This targets 4 currently uncovered public APIs with 0% coverage:
 * - crc32_combine: 2 undiscovered branches
 * - crc32_combine_op: 2 undiscovered branches
 * - crc32_combine_gen: 0 undiscovered branches
 * - get_crc_table: 0 undiscovered branches
 * 
 * Also exercises internal helper functions: crc32_combine64, x2nmodp, multmodp
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough for parameters plus data segments
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t crc_function_choice = fdp.ConsumeIntegral<uint8_t>() % 2;
    z_off_t len2 = static_cast<z_off_t>(fdp.ConsumeIntegral<int64_t>());
    
    // Consume two data segments
    size_t segment_a_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    if (segment_a_size > fdp.remaining_bytes()) {
        segment_a_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> segment_a = fdp.ConsumeBytes<uint8_t>(segment_a_size);
    
    size_t segment_b_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    if (segment_b_size > fdp.remaining_bytes()) {
        segment_b_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> segment_b = fdp.ConsumeBytes<uint8_t>(segment_b_size);
    
    // Test 1: get_crc_table() - retrieve CRC table
    const z_crc_t* crc_table = get_crc_table();
    if (crc_table == nullptr) {
        // If table is null, we can still proceed with other tests
        // but this might indicate an issue with initialization
    }
    
    // Initialize CRC values
    unsigned long crc_a = 0;
    unsigned long crc_b = 0;
    
    // Compute CRC for segment A
    if (crc_function_choice == 0) {
        // Use crc32()
        crc_a = crc32(0L, segment_a.data(), static_cast<uInt>(segment_a_size));
    } else {
        // Use crc32_z()
        crc_a = crc32_z(0L, segment_a.data(), static_cast<z_size_t>(segment_a_size));
    }
    
    // Compute CRC for segment B
    if (crc_function_choice == 0) {
        // Use crc32()
        crc_b = crc32(0L, segment_b.data(), static_cast<uInt>(segment_b_size));
    } else {
        // Use crc32_z()
        crc_b = crc32_z(0L, segment_b.data(), static_cast<z_size_t>(segment_b_size));
    }
    
    // Operation mode determines which combination functions to test
    switch (operation_mode) {
        case 0: {
            // Test basic crc32_combine()
            unsigned long combined_crc = crc32_combine(crc_a, crc_b, len2);
            // Don't validate result - just exercise the API
            break;
        }
        
        case 1: {
            // Test crc32_combine_gen() + crc32_combine_op()
            unsigned long op = crc32_combine_gen(len2);
            if (op != 0) {
                unsigned long combined_crc = crc32_combine_op(crc_a, crc_b, op);
                // Verify it matches direct combine for same len2
                unsigned long direct_combined = crc32_combine(crc_a, crc_b, len2);
                // Note: In real usage, these should match, but for fuzzing
                // we just exercise both paths
            }
            break;
        }
        
        case 2: {
            // Test negative length cases (should return 0)
            z_off_t negative_len = -1;
            unsigned long combined_neg = crc32_combine(crc_a, crc_b, negative_len);
            unsigned long op_neg = crc32_combine_gen(negative_len);
            if (op_neg == 0) {
                unsigned long combined_op_neg = crc32_combine_op(crc_a, crc_b, op_neg);
            }
            break;
        }
        
        case 3: {
            // Test multiple combination operations
            // Combine A+B, then combine result with A again
            unsigned long combined_ab = crc32_combine(crc_a, crc_b, len2);
            // Use a different length for second combination
            z_off_t len3 = static_cast<z_off_t>(fdp.ConsumeIntegral<int64_t>());
            unsigned long combined_final = crc32_combine(combined_ab, crc_a, len3);
            
            // Also test with generated operator
            unsigned long op_ab = crc32_combine_gen(len2);
            unsigned long op_final = crc32_combine_gen(len3);
            if (op_ab != 0 && op_final != 0) {
                unsigned long combined_op_ab = crc32_combine_op(crc_a, crc_b, op_ab);
                unsigned long combined_op_final = crc32_combine_op(combined_op_ab, crc_a, op_final);
            }
            break;
        }
    }
    
    // Test edge cases with zero-length segments
    if (segment_a_size > 0 && segment_b_size > 0) {
        // Test with zero initial CRC
        unsigned long zero_crc_a = crc32(0L, segment_a.data(), static_cast<uInt>(segment_a_size));
        unsigned long zero_crc_b = crc32(0L, segment_b.data(), static_cast<uInt>(segment_b_size));
        unsigned long combined_zero = crc32_combine(zero_crc_a, zero_crc_b, len2);
        
        // Test with same CRC values
        unsigned long combined_same = crc32_combine(crc_a, crc_a, len2);
    }
    
    // Test CRC computation on combined data (for verification)
    if (segment_a_size > 0 && segment_b_size > 0) {
        // Create combined buffer
        std::vector<uint8_t> combined_data;
        combined_data.reserve(segment_a_size + segment_b_size);
        combined_data.insert(combined_data.end(), segment_a.begin(), segment_a.end());
        combined_data.insert(combined_data.end(), segment_b.begin(), segment_b.end());
        
        // Compute CRC directly on combined data
        unsigned long direct_crc;
        if (crc_function_choice == 0) {
            direct_crc = crc32(0L, combined_data.data(), static_cast<uInt>(combined_data.size()));
        } else {
            direct_crc = crc32_z(0L, combined_data.data(), static_cast<z_size_t>(combined_data.size()));
        }
        
        // Theoretically, crc32_combine(crc_a, crc_b, segment_b_size) should equal
        // crc32(crc_a, segment_b, segment_b_size), but we just exercise the APIs
        unsigned long recalculated_b = crc32(crc_a, segment_b.data(), static_cast<uInt>(segment_b_size));
    }
    
    return 0;
}

} // namespace harness_004
