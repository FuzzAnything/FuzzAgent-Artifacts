#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_014 {
/*
 * Fuzzing harness for dictionary-based decompression in zlib
 * Target: inflateSetDictionary (0% coverage, 16 undiscovered branches)
 * Harness ID: 014
 * 
 * This harness specifically targets the critical coverage gap in inflateSetDictionary
 * which shows 0% coverage despite being reached by fuzzers 007, 012, 013.
 * 
 * Root cause analysis: inflateSetDictionary requires the inflate stream to be in 
 * DICT state (after inflate returns Z_NEED_DICT). Current harnesses call the function 
 * without proper state setup, causing immediate return without executing dictionary 
 * processing logic (16 undiscovered branches).
 * 
 * Different from existing harnesses:
 * - harness_000 (basic compression/decompression): covers basic compress/uncompress
 * - harness_001 (gzip file I/O): covers gz* APIs  
 * - harness_002 (inflateBack callback-based decompression): covers inflateBack APIs
 * - harness_003 (deflate parameter control): covers deflate* APIs
 * - harness_004 (advanced gzip I/O with dynamic parameter control): covers gz* with params
 * - harness_005 (gzip formatted I/O): covers gzip-specific APIs
 * - harness_006 (size_t variant compression/decompression): covers _z variants
 * - harness_007 (advanced inflate state manipulation): covers some inflate validation APIs
 * - harness_011 (header validation): covers basic header validation paths
 * - harness_012 (inflate validation and header manipulation): covers inflate validation APIs
 * - harness_013 (gzip header processing): covers GZIP header and some dictionary scenarios
 * 
 * This harness follows the exact recommended invocation sequence:
 * 1. deflateInit2_ and deflateSetDictionary to create dictionary-compressed data
 * 2. inflateInit2_ to initialize decompression stream with dictionary support
 * 3. Provide compressed data engineered to require a dictionary
 * 4. inflate(strm, Z_NO_FLUSH) should return Z_NEED_DICT
 * 5. inflateSetDictionary(strm, dictionary, dictLength) with fuzzed dictionary
 * 6. inflate(strm, Z_FINISH) to complete decompression using dictionary
 * 7. inflateEnd(strm) cleanup
 * 
 * Specialized test scenarios:
 * 1. Dictionary-based compression/decompression cycle
 * 2. Valid dictionary matching (Adler-32 validation)
 * 3. Invalid dictionary (wrong Adler-32) triggering Z_DATA_ERROR
 * 4. Empty dictionary edge cases
 * 5. Maximum dictionary length (32KB) testing
 * 6. Corrupted dictionary data
 * 7. Multiple dictionary updates
 * 8. Raw deflate format with dictionaries
 * 9. State transition: NORMAL → DICT → NORMAL
 * 
 * Expected coverage gain:
 * - Achieve 100% coverage of inflateSetDictionary (16 branches)
 * - Reduce blocked branches in inflate function
 * - Test specialized dictionary decompression feature
 * - Exercise dictionary validation and processing logic
 * 
 * Memory Lifetime Fix: Custom allocator callbacks to test zalloc/zfree paths
 */


// Custom memory allocation callbacks to test zalloc/zfree functionality
static void* custom_zalloc(void* opaque, unsigned items, unsigned size) {
    (void)opaque;
    return calloc(items, size);
}

static void custom_zfree(void* opaque, void* address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful dictionary testing
    // We need: input data + dictionary data + configuration parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Test scenario selection from fuzzer input
    bool use_raw_deflate = fdp.ConsumeBool();
    bool use_gzip_format = fdp.ConsumeBool();
    bool test_valid_dictionary = fdp.ConsumeBool();
    bool test_invalid_dictionary = fdp.ConsumeBool();
    bool test_empty_dictionary = fdp.ConsumeBool();
    bool test_max_dictionary = fdp.ConsumeBool();
    bool test_multiple_dictionaries = fdp.ConsumeBool();
    bool test_corrupted_dictionary = fdp.ConsumeBool();
    
    // Determine actual window bits based on format
    int actual_window_bits = window_bits;
    if (use_raw_deflate) {
        actual_window_bits = -window_bits;  // Negative for raw deflate
    } else if (use_gzip_format) {
        actual_window_bits = window_bits + 16;  // Add 16 for gzip header
    }
    
    // Get input data for compression
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }
    
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_size);
    
    // Get dictionary data - vary size based on test scenario
    std::vector<uint8_t> dictionary_data;
    size_t dict_size = 0;
    
    if (test_empty_dictionary) {
        dict_size = 0;  // Test empty dictionary
    } else if (test_max_dictionary) {
        dict_size = 32768;  // Maximum dictionary size
        if (fdp.remaining_bytes() < dict_size) {
            dict_size = fdp.remaining_bytes();
        }
    } else {
        // Regular dictionary size
        dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
        if (fdp.remaining_bytes() < dict_size) {
            dict_size = fdp.remaining_bytes();
        }
    }
    
    if (dict_size > 0) {
        dictionary_data = fdp.ConsumeBytes<uint8_t>(dict_size);
        
        // Corrupt dictionary data if testing corrupted dictionary scenario
        if (test_corrupted_dictionary && dictionary_data.size() > 0) {
            // Flip some bits to corrupt the dictionary
            size_t corruption_count = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            for (size_t i = 0; i < corruption_count && i < dictionary_data.size(); i++) {
                dictionary_data[i] ^= 0xFF;  // Flip all bits
            }
        }
    }
    
    // === PHASE 1: Create dictionary-compressed data ===
    
    // Compression stream
    z_stream c_strm;
    memset(&c_strm, 0, sizeof(c_strm));
    
    // Use custom allocator callbacks
    c_strm.zalloc = custom_zalloc;
    c_strm.zfree = custom_zfree;
    c_strm.opaque = Z_NULL;
    
    // Initialize compression with dictionary support
    int result = deflateInit2(&c_strm, compression_level, Z_DEFLATED,
                             actual_window_bits, mem_level, strategy);
    if (result != Z_OK) {
        return 0;
    }
    
    // Set dictionary for compression (if we have dictionary data)
    if (!dictionary_data.empty()) {
        result = deflateSetDictionary(&c_strm, dictionary_data.data(),
                                     static_cast<uInt>(dictionary_data.size()));
        // Note: We continue even if dictionary setup fails
    }
    
    // Create compressed output buffer
    size_t compressed_size = input_buffer.size() * 2 + 1024;  // Safe upper bound
    std::vector<uint8_t> compressed_buffer(compressed_size);
    
    c_strm.next_in = input_buffer.data();
    c_strm.avail_in = static_cast<uInt>(input_buffer.size());
    c_strm.next_out = compressed_buffer.data();
    c_strm.avail_out = static_cast<uInt>(compressed_size);
    
    // Compress the data
    result = deflate(&c_strm, Z_FINISH);
    if (result != Z_STREAM_END && result != Z_OK) {
        deflateEnd(&c_strm);
        return 0;
    }
    
    size_t actual_compressed_size = compressed_size - c_strm.avail_out;
    deflateEnd(&c_strm);
    
    // Save the dictionary ID (Adler-32) for later validation
    uLong dict_id = c_strm.adler;
    
    // === PHASE 2: Test dictionary-based decompression ===
    
    // Decompression stream
    z_stream d_strm;
    memset(&d_strm, 0, sizeof(d_strm));
    
    // Use custom allocator callbacks
    d_strm.zalloc = custom_zalloc;
    d_strm.zfree = custom_zfree;
    d_strm.opaque = Z_NULL;
    
    // Initialize decompression with same parameters
    result = inflateInit2(&d_strm, actual_window_bits);
    if (result != Z_OK) {
        return 0;
    }
    
    // Prepare decompression buffers
    std::vector<uint8_t> decompressed_buffer(input_buffer.size() * 2);
    d_strm.next_out = decompressed_buffer.data();
    d_strm.avail_out = static_cast<uInt>(decompressed_buffer.size());
    d_strm.next_in = compressed_buffer.data();
    d_strm.avail_in = static_cast<uInt>(actual_compressed_size);
    
    // === KEY TEST: Create Z_NEED_DICT scenario ===
    // For raw deflate with dictionary, inflate should return Z_NEED_DICT
    // when it encounters dictionary-compressed data without a dictionary set
    
    // Perform initial inflation - should return Z_NEED_DICT if we used a dictionary
    // and didn't provide one to inflate
    result = inflate(&d_strm, Z_NO_FLUSH);
    
    // Handle different scenarios based on fuzzer input
    if (result == Z_NEED_DICT) {
        // This is the ideal scenario we're targeting!
        
        // Test various dictionary scenarios
        if (test_valid_dictionary && !dictionary_data.empty()) {
            // Provide the correct dictionary
            result = inflateSetDictionary(&d_strm, dictionary_data.data(),
                                         static_cast<uInt>(dictionary_data.size()));
            // Should return Z_OK if dictionary matches
            
            if (result == Z_OK) {
                // Continue decompression with the dictionary
                result = inflate(&d_strm, Z_FINISH);
            }
        } else if (test_invalid_dictionary && fdp.remaining_bytes() > 0) {
            // Create an invalid dictionary (different from compression dictionary)
            size_t invalid_dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            if (fdp.remaining_bytes() >= invalid_dict_size) {
                std::vector<uint8_t> invalid_dict = fdp.ConsumeBytes<uint8_t>(invalid_dict_size);
                result = inflateSetDictionary(&d_strm, invalid_dict.data(),
                                             static_cast<uInt>(invalid_dict.size()));
                // Should return Z_DATA_ERROR due to Adler-32 mismatch
            }
        } else if (test_empty_dictionary) {
            // Test empty dictionary edge case
            result = inflateSetDictionary(&d_strm, NULL, 0);
            // Should return Z_STREAM_ERROR or Z_DATA_ERROR
        }
        
        // Test multiple dictionary updates (if scenario enabled)
        if (test_multiple_dictionaries && result == Z_OK && fdp.remaining_bytes() > 0) {
            // Try to set dictionary again (should fail in most states)
            result = inflateSetDictionary(&d_strm, dictionary_data.data(),
                                         static_cast<uInt>(dictionary_data.size()));
        }
    } else if (result == Z_OK || result == Z_STREAM_END) {
        // Data didn't require a dictionary - still test inflateSetDictionary
        // in wrong state to exercise error paths
        
        if (!dictionary_data.empty()) {
            // Try to set dictionary in wrong state (not DICT state)
            result = inflateSetDictionary(&d_strm, dictionary_data.data(),
                                         static_cast<uInt>(dictionary_data.size()));
            // Should return Z_STREAM_ERROR (state->mode != DICT)
        }
        
        // Complete decompression
        if (d_strm.avail_in > 0 && d_strm.avail_out > 0) {
            result = inflate(&d_strm, Z_FINISH);
        }
    } else if (result == Z_DATA_ERROR || result == Z_STREAM_ERROR) {
        // Handle error cases gracefully
        // Try to set dictionary anyway to test error recovery
        if (!dictionary_data.empty()) {
            inflateSetDictionary(&d_strm, dictionary_data.data(),
                                static_cast<uInt>(dictionary_data.size()));
        }
    }
    
    // === PHASE 3: Test additional dictionary scenarios ===
    
    // Test inflateGetDictionary API if we have a valid stream
    if (d_strm.total_in > 0 || d_strm.total_out > 0) {
        uInt retrieved_dict_len = 0;
        std::vector<uint8_t> retrieved_dict(32768);
        
        // First get the dictionary length
        result = inflateGetDictionary(&d_strm, NULL, &retrieved_dict_len);
        
        if (result == Z_OK && retrieved_dict_len > 0 && retrieved_dict_len <= retrieved_dict.size()) {
            // Then get the actual dictionary contents
            result = inflateGetDictionary(&d_strm, retrieved_dict.data(), &retrieved_dict_len);
        }
    }
    
    // Test reset and dictionary re-use
    if (test_multiple_dictionaries && !dictionary_data.empty()) {
        // Reset the stream and try again
        result = inflateReset(&d_strm);
        if (result == Z_OK) {
            // Reset input/output pointers
            d_strm.next_in = compressed_buffer.data();
            d_strm.avail_in = static_cast<uInt>(actual_compressed_size);
            d_strm.next_out = decompressed_buffer.data();
            d_strm.avail_out = static_cast<uInt>(decompressed_buffer.size());
            
            // Try to decompress again
            result = inflate(&d_strm, Z_NO_FLUSH);
            
            if (result == Z_NEED_DICT) {
                // Set dictionary again
                result = inflateSetDictionary(&d_strm, dictionary_data.data(),
                                             static_cast<uInt>(dictionary_data.size()));
                
                if (result == Z_OK) {
                    inflate(&d_strm, Z_FINISH);
                }
            }
        }
    }
    
    // Cleanup
    result = inflateEnd(&d_strm);
    
    return 0;
}

} // namespace harness_014
