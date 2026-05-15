#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <algorithm>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_006 {
/* Fuzzing harness for zlib CRC32 and Adler32 checksum combination operations
 * Targets checksum algebra functions that are mathematically distinct from compression:
 * - crc32_combine_op (Primary target - 267 undiscovered branches)
 * - crc32_combine, crc32_combine_gen
 * - adler32_combine
 * - get_crc_table (optional helper)
 * 
 * This harness focuses on checksum combination algebra, testing edge cases:
 * - Zero length combinations
 * - Maximum length combinations  
 * - Identical checksums
 * - Negative lengths for adler32_combine
 * - Both 32-bit and 64-bit variants where applicable
 */


// Minimum input size requirement for meaningful testing
const size_t MIN_INPUT_SIZE = 32;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_DATA_SIZE = 64 * 1024;  // 64KB

// Helper function to generate test data with various patterns
std::vector<uint8_t> generate_test_data(FuzzedDataProvider& fdp, size_t max_size) {
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size);
    if (data_size == 0) {
        return std::vector<uint8_t>();
    }
    
    // Use different patterns: random bytes, zeros, pattern, or mixed
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    std::vector<uint8_t> data(data_size);
    
    switch (pattern_type) {
        case 0: // Random bytes
            data = fdp.ConsumeBytes<uint8_t>(data_size);
            if (data.size() < data_size) {
                // Pad with zeros if not enough bytes
                data.resize(data_size, 0);
            }
            break;
            
        case 1: // All zeros
            std::fill(data.begin(), data.end(), 0);
            break;
            
        case 2: // Pattern (0, 1, 2, ...)
            for (size_t i = 0; i < data_size; i++) {
                data[i] = static_cast<uint8_t>(i & 0xFF);
            }
            break;
            
        case 3: // Alternating pattern with fuzzer-provided values
            uint8_t pattern_a = fdp.ConsumeIntegral<uint8_t>();
            uint8_t pattern_b = fdp.ConsumeIntegral<uint8_t>();
            for (size_t i = 0; i < data_size; i++) {
                data[i] = (i % 2 == 0) ? pattern_a : pattern_b;
            }
            break;
    }
    
    return data;
}

// Test CRC32 combination functions
void test_crc32_combinations(FuzzedDataProvider& fdp) {
    // Generate two data buffers for testing
    size_t max_size = fdp.ConsumeIntegralInRange<size_t>(0, MAX_DATA_SIZE);
    std::vector<uint8_t> data1 = generate_test_data(fdp, max_size);
    std::vector<uint8_t> data2 = generate_test_data(fdp, max_size);
    
    // Compute CRC32 checksums for the data
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    if (!data1.empty()) {
        crc1 = crc32(crc1, data1.data(), data1.size());
    }
    
    if (!data2.empty()) {
        crc2 = crc32(crc2, data2.data(), data2.size());
    }
    
    // Test edge cases: identical checksums
    uLong crc_same = crc1;
    
    // Generate length parameter for combination
    // Use both positive and zero lengths, testing both 32-bit and 64-bit variants
    z_off_t len32 = fdp.ConsumeIntegral<z_off_t>();
    z_off64_t len64 = static_cast<z_off64_t>(len32);
    
    // Test crc32_combine_gen() - generates operator from length
    uLong op = crc32_combine_gen(len32);
    
    // Test crc32_combine_gen64() - 64-bit variant
    uLong op64 = crc32_combine_gen64(len64);
    
    // Test crc32_combine_op() with generated operator
    uLong combined1 = crc32_combine_op(crc1, crc2, op);
    uLong combined2 = crc32_combine_op(crc2, crc1, op);  // Reverse order
    uLong combined_same = crc32_combine_op(crc_same, crc_same, op);  // Same checksums
    
    // Test with zero operator (edge case)
    uLong combined_zero_op = crc32_combine_op(crc1, crc2, 0);
    
    // Test crc32_combine() - standard combination
    uLong combined_std = crc32_combine(crc1, crc2, len32);
    
    // Test crc32_combine64() - 64-bit variant
    uLong combined_std64 = crc32_combine64(crc1, crc2, len64);
    
    // Verify that combine and combine_op give same result when using same operator
    // (Note: crc32_combine() internally calls crc32_combine_op() with generated op)
    // Test combination with zero length
    uLong combined_zero_len = crc32_combine(crc1, crc2, 0);
    
    // Test combination with negative length (should return 0 for CRC32)
    z_off_t neg_len = fdp.ConsumeIntegral<z_off_t>();
    uLong combined_neg_len = crc32_combine(crc1, crc2, neg_len);
    // Test get_crc_table() if dynamic tables enabled
    // This is optional and depends on DYNAMIC_CRC_TABLE define
    const z_crc_t* crc_table = get_crc_table();
    if (crc_table != nullptr) {
        // Table exists, can't do much with it except verify it's not null
    }
}

// Test Adler32 combination functions
void test_adler32_combinations(FuzzedDataProvider& fdp) {
    // Generate two data buffers for testing
    size_t max_size = fdp.ConsumeIntegralInRange<size_t>(0, MAX_DATA_SIZE);
    std::vector<uint8_t> data1 = generate_test_data(fdp, max_size);
    std::vector<uint8_t> data2 = generate_test_data(fdp, max_size);
    
    // Compute Adler32 checksums for the data
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    if (!data1.empty()) {
        adler1 = adler32(adler1, data1.data(), data1.size());
    }
    
    if (!data2.empty()) {
        adler2 = adler32(adler2, data2.data(), data2.size());
    }
    
    // Test edge cases: identical checksums
    uLong adler_same = adler1;
    
    // Generate length parameter for combination
    // Adler32_combine accepts negative lengths (returns 0xffffffff for debugging)
    z_off_t len32 = fdp.ConsumeIntegral<z_off_t>();
    z_off64_t len64 = static_cast<z_off64_t>(len32);
    
    // Test adler32_combine() - standard combination
    uLong combined_std = adler32_combine(adler1, adler2, len32);
    
    // Test adler32_combine64() - 64-bit variant
    uLong combined_std64 = adler32_combine64(adler1, adler2, len64);
    
    // Test combination with zero length
    uLong combined_zero_len = adler32_combine(adler1, adler2, 0);
    
    // Test combination with identical checksums
    uLong combined_same = adler32_combine(adler_same, adler_same, len32);
    // Test with very large positive length (use fuzzer input)
    z_off_t large_len = fdp.ConsumeIntegralInRange<z_off_t>(1000000, 1000000000);
    uLong combined_large_len = adler32_combine(adler1, adler2, large_len);
    
    // Test with maximum positive value (use fuzzer input)
    z_off_t max_len = fdp.ConsumeIntegral<z_off_t>();
    uLong combined_max_len = adler32_combine(adler1, adler2, max_len);
}

// Test mixed CRC32 and Adler32 operations
void test_mixed_checksum_operations(FuzzedDataProvider& fdp) {
    // Generate test data
    size_t max_size = fdp.ConsumeIntegralInRange<size_t>(0, MAX_DATA_SIZE);
    std::vector<uint8_t> data = generate_test_data(fdp, max_size);
    
    if (data.empty()) {
        return;
    }
    
    // Compute both CRC32 and Adler32 on same data
    uLong crc = crc32(0L, Z_NULL, 0);
    uLong adler = adler32(0L, Z_NULL, 0);
    
    crc = crc32(crc, data.data(), data.size());
    adler = adler32(adler, data.data(), data.size());
    
    // Generate another checksum for combination testing
    std::vector<uint8_t> data2 = generate_test_data(fdp, max_size);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    if (!data2.empty()) {
        crc2 = crc32(crc2, data2.data(), data2.size());
        adler2 = adler32(adler2, data2.data(), data2.size());
    }
    
    // Test various length parameters
    z_off_t len = fdp.ConsumeIntegral<z_off_t>();
    
    // Test CRC32 combinations
    uLong crc_op = crc32_combine_gen(len);
    uLong crc_combined = crc32_combine_op(crc, crc2, crc_op);
    uLong crc_combined_std = crc32_combine(crc, crc2, len);
    
    // Test Adler32 combinations
    uLong adler_combined = adler32_combine(adler, adler2, len);
    
    // Test edge case: combining checksum of empty data with non-empty
    uLong crc_empty = crc32(0L, Z_NULL, 0);
    uLong adler_empty = adler32(0L, Z_NULL, 0);
    
    uLong crc_combined_empty = crc32_combine(crc_empty, crc, len);
    uLong adler_combined_empty = adler32_combine(adler_empty, adler, len);
    
    // Test with zero as second checksum
    uLong crc_combined_zero = crc32_combine(crc, 0L, len);
    uLong adler_combined_zero = adler32_combine(adler, 0L, len);
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider
    FuzzedDataProvider fdp(data, size);
    
    // Determine which tests to run based on input
    uint8_t test_selector = fdp.ConsumeIntegral<uint8_t>();
    
    // Run selected tests (can run multiple based on remaining input)
    if (test_selector & 0x01) {
        test_crc32_combinations(fdp);
    }
    
    if (test_selector & 0x02) {
        test_adler32_combinations(fdp);
    }
    
    if (test_selector & 0x04) {
        test_mixed_checksum_operations(fdp);
    }
    
    // If we have enough remaining input, run all tests
    if (fdp.remaining_bytes() > MIN_INPUT_SIZE) {
        // Re-initialize with remaining data for more thorough testing
        std::vector<uint8_t> remaining = fdp.ConsumeRemainingBytes<uint8_t>();
        if (remaining.size() > MIN_INPUT_SIZE) {
            FuzzedDataProvider fdp2(remaining.data(), remaining.size());
            
            // Run a different combination of tests
            uint8_t test_selector2 = fdp2.ConsumeIntegral<uint8_t>();
            
            if (test_selector2 & 0x01) {
                test_crc32_combinations(fdp2);
            }
            
            if (test_selector2 & 0x02) {
                test_adler32_combinations(fdp2);
            }
        }
    }
    
    return 0;
}

} // namespace harness_006
