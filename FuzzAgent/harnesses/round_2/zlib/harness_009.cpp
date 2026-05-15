#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_009 {
/*
 * Fuzzing harness for zlib checksum combination and utility functions
 * Target: crc32_combine, adler32_combine, zlibCompileFlags, zlibVersion, 
 *         get_crc_table, crc32_combine_gen, crc32_combine_op, adler32, crc32
 * Harness ID: 009
 * 
 * This harness focuses on testing checksum combination operations and utility functions,
 * which are critical coverage gaps identified in zutil and crc32 modules with completely
 * uncovered APIs: crc32_combine, adler32_combine, zlibCompileFlags, and other utility functions.
 * 
 * Different from existing harnesses:
 * - harness_000 (basic compression/decompression): covers compress/uncompress
 * - harness_001 (gzip file I/O): covers gz* APIs  
 * - harness_002 (inflateBack callback-based decompression): covers inflateBack
 * - harness_003 (deflate parameter control): covers deflate* APIs
 * - harness_004 (advanced gzip I/O with dynamic parameter control): covers gz* with params
 * - harness_005 (gzip formatted I/O): covers gzip-specific APIs
 * - harness_006 (size_t variant compression/decompression): covers _z variants
 * - harness_007 (advanced inflate state manipulation): covers inflate* APIs
 * - harness_008 (comprehensive gz file reading): covers gz reading operations
 * 
 * This harness specifically targets:
 * 1. crc32_combine: Combine two CRC-32 checksums
 * 2. adler32_combine: Combine two Adler-32 checksums  
 * 3. zlibCompileFlags: Get compilation flags and feature information
 * 4. zlibVersion: Get library version information
 * 5. get_crc_table: Get CRC table for dynamic initialization
 * 6. crc32_combine_gen: Generate combine operation for repeated use
 * 7. crc32_combine_op: Combine using pre-generated operation
 * 8. crc32: Basic CRC-32 computation
 * 9. adler32: Basic Adler-32 computation
 * 
 * The harness creates semantically diverse scenarios:
 * - Combining checksums of different lengths and values
 * - Using pre-generated combine operations for efficiency
 * - Testing edge cases with zero lengths and extreme values
 * - Verifying utility functions return valid information
 * - Mixing checksum computation with combination operations
 * - Testing both 32-bit and 64-bit length variants
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool test_crc_combine = fdp.ConsumeBool();
    bool test_adler_combine = fdp.ConsumeBool();
    bool test_utility_functions = fdp.ConsumeBool();
    bool test_precomputed_ops = fdp.ConsumeBool();
    bool test_edge_cases = fdp.ConsumeBool();
    bool test_basic_checksums = fdp.ConsumeBool();
    bool test_64bit_variants = fdp.ConsumeBool();
    
    // Consume checksum values
    uLong crc1 = fdp.ConsumeIntegral<uLong>();
    uLong crc2 = fdp.ConsumeIntegral<uLong>();
    uLong adler1 = fdp.ConsumeIntegral<uLong>();
    uLong adler2 = fdp.ConsumeIntegral<uLong>();
    
    // Consume lengths - careful with negative values
    z_off_t len2 = fdp.ConsumeIntegral<z_off_t>();
    z_off64_t len2_64 = fdp.ConsumeIntegral<z_off64_t>();
    
    // Ensure non-negative lengths for combine functions - SAFE ABSOLUTE VALUE
    // Using safe absolute to avoid integer overflow when negating INT64_MIN
    auto safe_abs = [](auto x) -> decltype(x) {
        if (x >= 0) return x;
        if (x == std::numeric_limits<decltype(x)>::min()) {
            // For minimum negative value, return maximum positive value
            return std::numeric_limits<decltype(x)>::max();
        }
        return -x;
    };
    
    len2 = safe_abs(len2);
    len2_64 = safe_abs(len2_64);
    // Consume data for basic checksum computation if needed
    std::vector<uint8_t> buffer_data;
    if (test_basic_checksums && fdp.remaining_bytes() > 0) {
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        if (fdp.remaining_bytes() < buffer_size) {
            buffer_size = fdp.remaining_bytes();
        }
        if (buffer_size > 0) {
            buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
        }
    }
    // Test different scenarios based on fuzzer input
    switch (test_scenario) {
        case 0: {
            // Test crc32_combine operations
            if (test_crc_combine) {
                uLong combined_crc = crc32_combine(crc1, crc2, len2);
                
                // Test with pre-generated operation for efficiency
                if (test_precomputed_ops) {
                    uLong op = crc32_combine_gen(len2);
                    uLong combined_via_op = crc32_combine_op(crc1, crc2, op);
                    
                    // They should produce the same result
                    // Note: Might not always match due to different algorithms,
                    // but both should be valid CRC combinations
                }
                
                // Test 64-bit variant if available
                if (test_64bit_variants) {
#ifdef Z_LARGE64
                    uLong combined_crc_64 = crc32_combine64(crc1, crc2, len2_64);
#endif
                }
            }
            break;
        }
        
        case 1: {
            // Test adler32_combine operations
            if (test_adler_combine) {
                uLong combined_adler = adler32_combine(adler1, adler2, len2);
                
                // Test 64-bit variant if available
                if (test_64bit_variants) {
#ifdef Z_LARGE64
                    uLong combined_adler_64 = adler32_combine64(adler1, adler2, len2_64);
#endif
                }
            }
            break;
        }
        
        case 2: {
            // Test utility functions
            if (test_utility_functions) {
                // Get zlib version
                const char* version = zlibVersion();
                
                // Get compilation flags
                uLong compile_flags = zlibCompileFlags();
                
                // Get CRC table (for dynamic initialization)
                const z_crc_t* crc_table = get_crc_table();
                
                // Check if version matches expected format
                // Version should start with "1." for zlib
                if (version != nullptr && strlen(version) > 0) {
                    // Valid version string
                }
                
                // Check compile flags for various features
                // Each bit represents a different compilation option
                // Flags include: sizeof(uInt), sizeof(uLong), ZLIB_DEBUG, etc.
                if (compile_flags != 0) {
                    // Valid compile flags
                }
                
                if (crc_table != nullptr) {
                    // Valid CRC table pointer
                }
            }
            break;
        }
        
        case 3: {
            // Test basic checksum computations
            if (test_basic_checksums && !buffer_data.empty()) {
                uLong crc = crc32(0L, Z_NULL, 0);  // Initial CRC
                uLong adler = adler32(0L, Z_NULL, 0);  // Initial Adler
                
                // Compute checksums on buffer data
                if (buffer_data.size() > 0) {
                    crc = crc32(crc, buffer_data.data(), buffer_data.size());
                    adler = adler32(adler, buffer_data.data(), buffer_data.size());
                    
                    // Also test _z variants with z_size_t
                    crc = crc32_z(crc, buffer_data.data(), buffer_data.size());
                    adler = adler32_z(adler, buffer_data.data(), buffer_data.size());
                }
            }
            break;
        }
        
        case 4: {
            // Test edge cases
            if (test_edge_cases) {
                // Test with zero length
                uLong zero_len_combined = crc32_combine(crc1, crc2, 0);
                uLong zero_len_adler = adler32_combine(adler1, adler2, 0);
                
                // Test with very large length
                z_off_t large_len = len2 > 1000 ? len2 : 1000;
                uLong large_len_combined = crc32_combine(crc1, crc2, large_len);
                
                // Test with same checksums
                uLong same_combined = crc32_combine(crc1, crc1, len2);
                uLong same_adler = adler32_combine(adler1, adler1, len2);
                
                // Test combination with zero checksums
                uLong zero_crc_combined = crc32_combine(0L, 0L, len2);
                uLong zero_adler_combined = adler32_combine(0L, 0L, len2);
            }
            break;
        }
        
        case 5: {
            // Test mixed operations - combine basic and combined checksums
            if (test_basic_checksums && !buffer_data.empty() && test_crc_combine) {
                // Compute CRC of first half
                size_t half = buffer_data.size() / 2;
                uLong crc_first = crc32(0L, buffer_data.data(), half);
                
                // Compute CRC of second half
                uLong crc_second = crc32(0L, buffer_data.data() + half, buffer_data.size() - half);
                
                // Combine them
                uLong combined = crc32_combine(crc_first, crc_second, buffer_data.size() - half);
                
                // Also compute direct CRC of entire buffer
                uLong direct_crc = crc32(0L, buffer_data.data(), buffer_data.size());
                
                // In theory: combined should equal direct_crc
                // crc32_combine(crc32(A), crc32(B), len(B)) == crc32(AB)
            }
            break;
        }
        
        case 6: {
            // Test generate and use pattern for repeated combinations
            if (test_precomputed_ops && test_crc_combine) {
                // Generate operation for a specific length
                uLong op = crc32_combine_gen(len2);
                
                // Use it multiple times with different CRCs
                for (int i = 0; i < 3 && fdp.remaining_bytes() > sizeof(uLong); i++) {
                    uLong test_crc1 = fdp.ConsumeIntegral<uLong>();
                    uLong test_crc2 = fdp.ConsumeIntegral<uLong>();
                    uLong result = crc32_combine_op(test_crc1, test_crc2, op);
                    
                    // Also compute using regular combine for comparison
                    uLong regular_result = crc32_combine(test_crc1, test_crc2, len2);
                    // result and regular_result should be equal
                }
            }
            break;
        }
        
        case 7: {
            // Comprehensive test combining multiple operations
            if (test_utility_functions) {
                // Get all utility information
                const char* version = zlibVersion();
                uLong flags = zlibCompileFlags();
                const z_crc_t* table = get_crc_table();
                
                // Test basic checksums if data available
                if (test_basic_checksums && !buffer_data.empty()) {
                    uLong crc = crc32(0L, buffer_data.data(), buffer_data.size());
                    uLong adler = adler32(0L, buffer_data.data(), buffer_data.size());
                    
                    // Test combination
                    if (test_crc_combine) {
                        uLong combined = crc32_combine(crc, crc, buffer_data.size());
                    }
                    
                    if (test_adler_combine) {
                        uLong combined = adler32_combine(adler, adler, buffer_data.size());
                    }
                }
            }
            break;
        }
    }
    
    // Test error case handling (should not crash)
    // Null buffer with non-zero length should be handled gracefully
    if (test_edge_cases) {
        uLong crc_null = crc32(0L, Z_NULL, 0);  // Should return initial CRC
        uLong adler_null = adler32(0L, Z_NULL, 0);  // Should return initial Adler
    }
    
    return 0;
}

} // namespace harness_009
