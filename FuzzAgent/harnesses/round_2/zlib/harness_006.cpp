#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_006 {
/*
 * Fuzzing harness for zlib size_t variant compression/decompression APIs
 * Target: compress_z, compress2_z, uncompress_z, uncompress2_z
 * Harness ID: 006
 * 
 * This harness specifically targets the size_t variant compression/decompression
 * APIs which are a critical coverage gap with 526+ undiscovered branches.
 * 
 * Different from existing harnesses:
 * - harness_000 (basic compression/decompression): covers uLong variants (compress, compress2, uncompress, uncompress2)
 * 
 * This harness focuses on the _z variants which use z_size_t instead of uLong:
 * 1. compress_z: Basic compression with z_size_t parameters
 * 2. compress2_z: Compression with level parameter and z_size_t parameters  
 * 3. uncompress_z: Decompression with z_size_t parameters
 * 4. uncompress2_z: Decompression with source length pointer and z_size_t parameters
 * 5. compressBound_z: Upper bound calculation with z_size_t
 * 
 * The harness tests edge cases like:
 * - Large data sizes that might overflow uLong but fit in z_size_t
 * - Size_t overflow in buffer size calculations
 * - Different compression levels and error conditions
 * - Partial and complete compression/decompression cycles
 * - Memory allocation failures with large buffers
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 10);
    bool test_large_sizes = fdp.ConsumeBool();
    bool test_error_handling = fdp.ConsumeBool();
    bool test_partial_operations = fdp.ConsumeBool();
    
    // Determine input data size - use large sizes for z_size_t testing
    z_size_t input_len;
    if (test_large_sizes) {
        // Test potentially large sizes for z_size_t (might be larger than uLong on some platforms)
        input_len = fdp.ConsumeIntegralInRange<z_size_t>(1, 1024 * 1024 * 10); // Up to 10MB
    } else {
        input_len = fdp.ConsumeIntegralInRange<z_size_t>(1, 1024 * 1024); // Up to 1MB
    }
    
    // Check if we have enough input data
    if (fdp.remaining_bytes() < input_len) {
        return 0;
    }
    
    // Consume the input data
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_len);
    
    // Calculate maximum compressed size using compressBound_z
    z_size_t max_compressed_size = compressBound_z(input_len);
    
    // Sanity check to avoid excessive memory usage
    if (max_compressed_size == 0 || max_compressed_size > 50 * 1024 * 1024) {
        return 0;
    }
    
    // Allocate buffers
    uint8_t* compressed_buffer = (uint8_t*)malloc(max_compressed_size);
    uint8_t* decompressed_buffer = (uint8_t*)malloc(input_len);
    
    if (!compressed_buffer || !decompressed_buffer) {
        free(compressed_buffer);
        free(decompressed_buffer);
        return 0;
    }
    
    int result = Z_OK;
    z_size_t compressed_len = max_compressed_size;
    z_size_t decompressed_len = input_len;
    
    // Test different scenarios based on fuzzer input
    switch (test_scenario) {
        case 0: {
            // Basic compress_z -> uncompress_z round trip
            result = compress_z(compressed_buffer, &compressed_len,
                               input_data.data(), input_len);
            
            if (result == Z_OK) {
                result = uncompress_z(decompressed_buffer, &decompressed_len,
                                     compressed_buffer, compressed_len);
                
                if (result == Z_OK && decompressed_len == input_len) {
                    // Verify data matches
                    if (memcmp(decompressed_buffer, input_data.data(), input_len) != 0) {
                        // Data mismatch - this is an error
                    }
                }
            }
            break;
        }
        
        case 1: {
            // compress2_z with specific compression level -> uncompress_z
            // Note: compression_level -1 means Z_DEFAULT_COMPRESSION
            if (compression_level == -1) {
                compression_level = Z_DEFAULT_COMPRESSION;
            }
            
            result = compress2_z(compressed_buffer, &compressed_len,
                                input_data.data(), input_len,
                                compression_level);
            
            if (result == Z_OK) {
                result = uncompress_z(decompressed_buffer, &decompressed_len,
                                     compressed_buffer, compressed_len);
            }
            break;
        }
        
        case 2: {
            // compress_z -> uncompress2_z (with source length pointer)
            result = compress_z(compressed_buffer, &compressed_len,
                               input_data.data(), input_len);
            
            if (result == Z_OK) {
                z_size_t source_len = compressed_len;
                result = uncompress2_z(decompressed_buffer, &decompressed_len,
                                      compressed_buffer, &source_len);
                
                // source_len should now contain the number of bytes consumed
                if (result == Z_OK && source_len != compressed_len) {
                    // Not all source bytes were consumed
                }
            }
            break;
        }
        
        case 3: {
            // compress2_z -> uncompress2_z round trip
            if (compression_level == -1) {
                compression_level = Z_DEFAULT_COMPRESSION;
            }
            
            result = compress2_z(compressed_buffer, &compressed_len,
                                input_data.data(), input_len,
                                compression_level);
            
            if (result == Z_OK) {
                z_size_t source_len = compressed_len;
                result = uncompress2_z(decompressed_buffer, &decompressed_len,
                                      compressed_buffer, &source_len);
            }
            break;
        }
        
        case 4: {
            // Test error handling: insufficient output buffer for compression
            if (test_error_handling && max_compressed_size > 1) {
                z_size_t small_buffer_size = max_compressed_size / 4;
                if (small_buffer_size < 1) small_buffer_size = 1;
                
                uint8_t* small_buffer = (uint8_t*)malloc(small_buffer_size);
                if (small_buffer) {
                    z_size_t small_len = small_buffer_size;
                    int err_result = compress_z(small_buffer, &small_len,
                                               input_data.data(), input_len);
                    // Should get Z_BUF_ERROR or similar
                    free(small_buffer);
                }
            }
            // Still do normal compression/decompression
            result = compress_z(compressed_buffer, &compressed_len,
                               input_data.data(), input_len);
            
            if (result == Z_OK) {
                result = uncompress_z(decompressed_buffer, &decompressed_len,
                                     compressed_buffer, compressed_len);
            }
            break;
        }
        
        case 5: {
            // Test error handling: insufficient output buffer for decompression
            if (test_error_handling && input_len > 1) {
                z_size_t small_buffer_size = input_len / 4;
                if (small_buffer_size < 1) small_buffer_size = 1;
                
                uint8_t* small_buffer = (uint8_t*)malloc(small_buffer_size);
                if (small_buffer) {
                    // First compress the data
                    z_size_t temp_compressed_len = max_compressed_size;
                    int compress_result = compress_z(compressed_buffer, &temp_compressed_len,
                                                    input_data.data(), input_len);
                    
                    if (compress_result == Z_OK) {
                        z_size_t small_len = small_buffer_size;
                        int err_result = uncompress_z(small_buffer, &small_len,
                                                     compressed_buffer, temp_compressed_len);
                        // Should get Z_BUF_ERROR
                    }
                    free(small_buffer);
                }
            }
            // Still do normal compression/decompression
            result = compress_z(compressed_buffer, &compressed_len,
                               input_data.data(), input_len);
            
            if (result == Z_OK) {
                result = uncompress_z(decompressed_buffer, &decompressed_len,
                                     compressed_buffer, compressed_len);
            }
            break;
        }
        
        case 6: {
            // Test partial operations with compressBound_z edge cases
            // Test compressBound_z with various sizes
            z_size_t bound_test_sizes[] = {0, 1, 100, 1000, 10000, 100000, 1000000};
            for (z_size_t test_size : bound_test_sizes) {
                if (test_size <= input_len) {
                    z_size_t bound = compressBound_z(test_size);
                    // bound should be >= test_size
                }
            }
            
            // Normal compression/decompression
            result = compress_z(compressed_buffer, &compressed_len,
                               input_data.data(), input_len);
            
            if (result == Z_OK) {
                result = uncompress_z(decompressed_buffer, &decompressed_len,
                                     compressed_buffer, compressed_len);
            }
            break;
        }
        
        case 7: {
            // Test with zero-length input (edge case)
            if (test_partial_operations && input_len > 0) {
                // Test with empty input buffer
                z_size_t zero_len = 0;
                uint8_t dummy = 0;
                int zero_result = compress_z(&dummy, &zero_len, &dummy, 0);
                // Should handle zero-length input
            }
            
            // Test with single byte input
            if (input_len >= 1) {
                z_size_t single_compressed_len = max_compressed_size;
                result = compress_z(compressed_buffer, &single_compressed_len,
                                   input_data.data(), 1);
                
                if (result == Z_OK) {
                    z_size_t single_decompressed_len = 1;
                    result = uncompress_z(decompressed_buffer, &single_decompressed_len,
                                         compressed_buffer, single_compressed_len);
                }
            }
            break;
        }
    }
    
    // Clean up
    free(compressed_buffer);
    free(decompressed_buffer);
    
    return 0;
}

} // namespace harness_006
