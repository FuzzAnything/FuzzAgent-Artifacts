#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_003 {
/*
 * Fuzzing harness for zlib dictionary functions
 * Targets: deflateSetDictionary, inflateSetDictionary, 
 *          deflateGetDictionary, inflateGetDictionary
 * 
 * This harness exercises the complete dictionary function lifecycle:
 * 1. Initialize deflate stream with deflateInit2_
 * 2. Set dictionary with deflateSetDictionary
 * 3. Get dictionary with deflateGetDictionary
 * 4. Perform compression with deflate
 * 5. Cleanup with deflateEnd
 * 6. Initialize inflate stream with inflateInit2_
 * 7. Set dictionary with inflateSetDictionary
 * 8. Get dictionary with inflateGetDictionary
 * 9. Perform decompression with inflate
 * 10. Cleanup with inflateEnd
 * 
 * This targets 68 undiscovered branches across 4 dictionary APIs.
 * Tests various dictionary sizes (0, small, max) and edge cases.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough for parameters plus some dictionary data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);           // Compression level 0-9
    int method = Z_DEFLATED;                                     // Only deflate method
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);     // Valid windowBits range
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);        // Memory level 1-9
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);        // Strategy 0-4 (Z_DEFAULT_STRATEGY to Z_FIXED)
    
    // Determine maximum dictionary size based on windowBits
    size_t max_window_size = 1U << windowBits;  // 2^windowBits
    size_t max_dict_size = max_window_size - 262;  // Max usable dictionary per zlib docs
    
    // Consume dictionary size (test 0, small, max)
    size_t dict_size_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    size_t dict_size;
    
    switch (dict_size_choice) {
        case 0:
            dict_size = 0;  // Test zero-sized dictionary
            break;
        case 1:
            dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);  // Small dictionary
            break;
        case 2:
            dict_size = max_dict_size;  // Maximum dictionary
            if (dict_size > fdp.remaining_bytes()) {
                dict_size = fdp.remaining_bytes();
            }
            break;
        default:
            dict_size = fdp.ConsumeIntegralInRange<size_t>(1, max_dict_size);
    }
    
    // Ensure dict_size doesn't exceed available data
    if (dict_size > fdp.remaining_bytes()) {
        dict_size = fdp.remaining_bytes();
    }
    
    // Consume dictionary data
    std::vector<uint8_t> dictionary = fdp.ConsumeBytes<uint8_t>(dict_size);
    
    // Consume data to compress (remaining input after dictionary)
    size_t data_to_compress_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    if (data_to_compress_size > fdp.remaining_bytes()) {
        data_to_compress_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> data_to_compress = fdp.ConsumeBytes<uint8_t>(data_to_compress_size);
    
    // ===== DEFLATE (COMPRESSION) STREAM =====
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    
    // Initialize deflate stream
    int ret = deflateInit2_(&deflate_stream, level, method, windowBits, memLevel, strategy,
                           ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        // If initialization fails, skip further processing
        return 0;
    }
    
    // Test 1: deflateSetDictionary
    if (dictionary.size() > 0) {
        ret = deflateSetDictionary(&deflate_stream, dictionary.data(), dictionary.size());
        // Don't check result - just exercise the API, may fail if not at block boundary
    }
    
    // Test 2: deflateGetDictionary
    size_t dict_buffer_size = 32768;  // Always enough per zlib docs
    std::vector<uint8_t> dict_buffer(dict_buffer_size, 0);
    uInt dict_length = dict_buffer_size;
    
    ret = deflateGetDictionary(&deflate_stream, dict_buffer.data(), &dict_length);
    // Result may be Z_OK or Z_STREAM_ERROR depending on state
    
    // Test 3: Perform compression with dictionary
    // Allocate output buffer (compressBound gives worst-case size)
    uLongf compr_bound = compressBound(data_to_compress.size());
    if (compr_bound == 0) {
        deflateEnd(&deflate_stream);
        return 0;
    }
    
    std::vector<uint8_t> compr_buffer(compr_bound);
    
    deflate_stream.next_in = data_to_compress.data();
    deflate_stream.avail_in = data_to_compress.size();
    deflate_stream.next_out = compr_buffer.data();
    deflate_stream.avail_out = compr_buffer.size();
    
    ret = deflate(&deflate_stream, Z_FINISH);
    // Don't check result - may succeed or fail based on dictionary setup
    
    // Reset stream for potential further operations
    deflateReset(&deflate_stream);
    
    // Clean up deflate stream
    deflateEnd(&deflate_stream);
    
    // ===== INFLATE (DECOMPRESSION) STREAM =====
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    // Initialize inflate stream (use same windowBits for compatibility)
    ret = inflateInit2_(&inflate_stream, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Test 4: inflateSetDictionary
    if (dictionary.size() > 0) {
        // For inflate, we need to be in Z_NEED_DICT state to set dictionary
        // We'll try anyway to exercise the API (will likely return Z_STREAM_ERROR)
        ret = inflateSetDictionary(&inflate_stream, dictionary.data(), dictionary.size());
        // Result may be Z_OK, Z_STREAM_ERROR, or Z_DATA_ERROR
    }
    
    // Test 5: inflateGetDictionary
    std::vector<uint8_t> inflate_dict_buffer(dict_buffer_size, 0);
    uInt inflate_dict_length = dict_buffer_size;
    
    ret = inflateGetDictionary(&inflate_stream, inflate_dict_buffer.data(), &inflate_dict_length);
    // Result may be Z_OK or Z_STREAM_ERROR depending on state
    
    // Test 6: Perform decompression (if we have compressed data)
    if (compr_buffer.size() > 0 && deflate_stream.total_out > 0) {
        size_t actual_compr_size = deflate_stream.total_out;
        
        // Allocate decompression buffer (use original size as estimate)
        std::vector<uint8_t> uncompr_buffer(data_to_compress.size());
        
        inflate_stream.next_in = compr_buffer.data();
        inflate_stream.avail_in = actual_compr_size;
        inflate_stream.next_out = uncompr_buffer.data();
        inflate_stream.avail_out = uncompr_buffer.size();
        
        // Try to decompress (may fail without proper dictionary setup)
        ret = inflate(&inflate_stream, Z_FINISH);
        
        // Reset for potential retry with dictionary if needed
        if (ret == Z_NEED_DICT && dictionary.size() > 0) {
            // Reset stream and try again with dictionary
            inflateReset(&inflate_stream);
            
            // Set dictionary first
            inflateSetDictionary(&inflate_stream, dictionary.data(), dictionary.size());
            
            // Try decompression again
            inflate_stream.next_in = compr_buffer.data();
            inflate_stream.avail_in = actual_compr_size;
            inflate_stream.next_out = uncompr_buffer.data();
            inflate_stream.avail_out = uncompr_buffer.size();
            
            ret = inflate(&inflate_stream, Z_FINISH);
        }
    }
    
    // Test 7: Edge cases with NULL pointers
    // Test deflateGetDictionary with NULL dictionary pointer (should return length only)
    uInt dict_len_only = 0;
    deflateGetDictionary(&deflate_stream, NULL, &dict_len_only);
    
    // Test inflateGetDictionary with NULL dictionary pointer
    uInt inflate_dict_len_only = 0;
    inflateGetDictionary(&inflate_stream, NULL, &inflate_dict_len_only);
    
    // Test with NULL dictLength pointer
    deflateGetDictionary(&deflate_stream, dict_buffer.data(), NULL);
    inflateGetDictionary(&inflate_stream, inflate_dict_buffer.data(), NULL);
    
    // Clean up inflate stream
    inflateEnd(&inflate_stream);
    
    // Test 8: Additional edge case - dictionary larger than window
    if (dictionary.size() > max_dict_size && dictionary.size() > 0) {
        // Try with oversized dictionary (should work but only part will be used)
        z_stream test_stream;
        memset(&test_stream, 0, sizeof(z_stream));
        
        if (deflateInit2_(&test_stream, level, method, windowBits, memLevel, strategy,
                         ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
            // This should work but only window-262 bytes will be used
            deflateSetDictionary(&test_stream, dictionary.data(), dictionary.size());
            deflateEnd(&test_stream);
        }
    }
    
    // Test 9: Raw deflate/inflate with dictionary (windowBits negative)
    int raw_windowBits = -windowBits;  // Negative for raw deflate/inflate
    z_stream raw_deflate_stream;
    memset(&raw_deflate_stream, 0, sizeof(z_stream));
    
    if (deflateInit2_(&raw_deflate_stream, level, method, raw_windowBits, memLevel, strategy,
                     ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        // For raw deflate, dictionary can be set at any time
        if (dictionary.size() > 0) {
            deflateSetDictionary(&raw_deflate_stream, dictionary.data(), dictionary.size());
        }
        deflateEnd(&raw_deflate_stream);
    }
    
    z_stream raw_inflate_stream;
    memset(&raw_inflate_stream, 0, sizeof(z_stream));
    
    if (inflateInit2_(&raw_inflate_stream, raw_windowBits, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        // For raw inflate, dictionary can be set at any time
        if (dictionary.size() > 0) {
            inflateSetDictionary(&raw_inflate_stream, dictionary.data(), dictionary.size());
        }
        inflateEnd(&raw_inflate_stream);
    }
    
    return 0;
}

} // namespace harness_003
