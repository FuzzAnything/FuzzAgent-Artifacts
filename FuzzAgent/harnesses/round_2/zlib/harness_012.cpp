#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_012 {
/*
 * Fuzzing harness for zlib inflate validation and header manipulation APIs
 * Target: inflateValidate (6 undiscovered branches), inflateGetHeader (4 branches), 
 *         inflateCodesUsed (2 branches), inflateUndermine (2 branches), 
 *         inflateSetDictionary (16 branches), inflateGetDictionary (8 branches)
 * Harness ID: 012
 * 
 * This harness specifically targets the 6 APIs with 0% coverage representing 34+ points 
 * of undiscovered complexity in inflate validation and header manipulation.
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
 * 
 * This harness follows the exact recommended invocation sequence:
 * 1. deflateInit2_ and deflate to generate compressed test data
 * 2. inflateInit2_ to initialize decompression stream  
 * 3. inflate to create valid state
 * 4. inflateSetDictionary
 * 5. inflateGetDictionary
 * 6. inflateValidate
 * 7. inflateGetHeader
 * 8. inflateCodesUsed
 * 9. inflateUndermine
 * 10. inflateEnd
 * 11. deflateEnd
 * 
 * The harness creates semantically diverse stream validation lifecycle scenarios:
 * - Testing inflateValidate with different validation modes
 * - Testing inflateGetHeader with gzip header retrieval
 * - Testing inflateSetDictionary/GetDictionary with various dictionary sizes
 * - Testing inflateUndermine with different undermine modes
 * - Testing inflateCodesUsed at different decompression states
 * - Testing both raw deflate and zlib/gzip formats
 * - Testing with and without dictionary usage
 * - Testing error conditions and edge cases
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing of validation APIs
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    bool use_raw_deflate = fdp.ConsumeBool();
    bool use_gzip_header = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_validation = fdp.ConsumeBool();
    bool test_header_retrieval = fdp.ConsumeBool();
    bool test_codes_used = fdp.ConsumeBool();
    bool test_undermine = fdp.ConsumeBool();
    
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Determine actual window bits based on format
    int actual_window_bits = window_bits;
    if (use_raw_deflate) {
        actual_window_bits = -window_bits;  // Negative for raw deflate
    } else if (use_gzip_header) {
        actual_window_bits = window_bits + 16;  // Add 16 for gzip header
    }
    
    // Get input data for compression
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(128, 8192);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }
    
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_size);
    
    // Get dictionary data if testing dictionary APIs
    std::vector<uint8_t> dictionary_data;
    if (test_dictionary && fdp.remaining_bytes() > 0) {
        size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 32768);
        if (fdp.remaining_bytes() < dict_size) {
            dict_size = fdp.remaining_bytes();
        }
        if (dict_size > 0) {
            dictionary_data = fdp.ConsumeBytes<uint8_t>(dict_size);
        }
    }
    
    // Step 1: deflateInit2_ and deflate to generate compressed test data
    z_stream c_strm;
    memset(&c_strm, 0, sizeof(c_strm));
    c_strm.zalloc = Z_NULL;
    c_strm.zfree = Z_NULL;
    c_strm.opaque = Z_NULL;
    
    int result = deflateInit2_(&c_strm, compression_level, Z_DEFLATED, 
                              actual_window_bits, mem_level, strategy,
                              ZLIB_VERSION, sizeof(z_stream));
    if (result != Z_OK) {
        return 0;  // Initialization failed
    }
    
    // Set dictionary if provided and testing dictionary
    if (test_dictionary && !dictionary_data.empty()) {
        result = deflateSetDictionary(&c_strm, dictionary_data.data(), 
                                     (uInt)dictionary_data.size());
        if (result != Z_OK) {
            // Dictionary setup failed, but continue without dictionary
            test_dictionary = false;
        }
    }
    
    // Allocate compression output buffer
    uLong max_compressed_size = deflateBound(&c_strm, (uLong)input_size);
    if (max_compressed_size == 0 || max_compressed_size > 10 * 1024 * 1024) {
        deflateEnd(&c_strm);
        return 0;  // Sanity check
    }
    
    std::vector<uint8_t> compressed_buffer(max_compressed_size);
    c_strm.next_out = compressed_buffer.data();
    c_strm.avail_out = (uInt)max_compressed_size;
    c_strm.next_in = input_buffer.data();
    c_strm.avail_in = (uInt)input_size;
    
    result = deflate(&c_strm, Z_FINISH);
    if (result != Z_STREAM_END && result != Z_OK) {
        deflateEnd(&c_strm);
        return 0;  // Compression failed
    }
    
    size_t compressed_size = max_compressed_size - c_strm.avail_out;
    
    // Step 2: inflateInit2_ to initialize decompression stream
    z_stream d_strm;
    memset(&d_strm, 0, sizeof(d_strm));
    d_strm.zalloc = Z_NULL;
    d_strm.zfree = Z_NULL;
    d_strm.opaque = Z_NULL;
    
    // For inflation, we need to use the same window bits as compression
    // but with appropriate sign for the format
    int inflate_window_bits = window_bits;
    if (use_raw_deflate) {
        inflate_window_bits = -window_bits;
    } else if (use_gzip_header) {
        inflate_window_bits = window_bits + 16;
    }
    
    result = inflateInit2_(&d_strm, inflate_window_bits, 
                          ZLIB_VERSION, sizeof(z_stream));
    if (result != Z_OK) {
        deflateEnd(&c_strm);
        return 0;  // Inflation initialization failed
    }
    
    // Step 7: Setup gz_header for inflateGetHeader if testing header retrieval
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    
    if (test_header_retrieval && use_gzip_header) {
        // Allocate space for gzip header fields
        gzheader.extra = (Bytef*)malloc(1024);
        gzheader.extra_max = 1024;
        gzheader.name = (Bytef*)malloc(256);
        gzheader.name_max = 256;
        gzheader.comment = (Bytef*)malloc(512);
        gzheader.comm_max = 512;
        
        result = inflateGetHeader(&d_strm, &gzheader);
        // inflateGetHeader returns Z_OK on success, but we continue regardless
    }
    
    // Prepare decompression buffers
    std::vector<uint8_t> decompressed_buffer(input_size);
    d_strm.next_out = decompressed_buffer.data();
    d_strm.avail_out = (uInt)input_size;
    d_strm.next_in = compressed_buffer.data();
    d_strm.avail_in = (uInt)compressed_size;
    
    // Step 3: inflate to create valid state (partial decompression)
    result = inflate(&d_strm, Z_NO_FLUSH);
    
    // Only continue if we have a valid state
    if (result != Z_OK && result != Z_STREAM_END && result != Z_BUF_ERROR) {
        // Clean up
        if (test_header_retrieval && use_gzip_header) {
            free(gzheader.extra);
            free(gzheader.name);
            free(gzheader.comment);
        }
        inflateEnd(&d_strm);
        deflateEnd(&c_strm);
        return 0;
    }
    
    // Step 4: inflateSetDictionary (if dictionary testing enabled)
    if (test_dictionary && !dictionary_data.empty() && d_strm.avail_in > 0) {
        // Only set dictionary if we're at a point where it might be needed
        // or allowed by the stream state
        result = inflateSetDictionary(&d_strm, dictionary_data.data(), 
                                     (uInt)dictionary_data.size());
        // Continue regardless of result (Z_OK, Z_STREAM_ERROR, or Z_DATA_ERROR)
    }
    
    // Step 5: inflateGetDictionary
    if (test_dictionary) {
        uInt dict_len = 0;
        std::vector<uint8_t> retrieved_dict(32768);  // Maximum dictionary size
        
        // First get the dictionary length
        result = inflateGetDictionary(&d_strm, NULL, &dict_len);
        
        if (result == Z_OK && dict_len > 0 && dict_len <= retrieved_dict.size()) {
            // Then get the actual dictionary contents
            result = inflateGetDictionary(&d_strm, retrieved_dict.data(), &dict_len);
        }
    }
    
    // Step 6: inflateValidate
    if (test_validation && d_strm.avail_in > 0) {
        // Consume validation mode from remaining fuzzer data
        int validate_mode = 0;
        if (fdp.remaining_bytes() > 0) {
            validate_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
        }
        
        result = inflateValidate(&d_strm, validate_mode);
        // Continue regardless of result
    }
    
    // Step 8: inflateCodesUsed (if testing codes used API)
    if (test_codes_used) {
        unsigned long codes_used = inflateCodesUsed(&d_strm);
        // Use the result somehow to avoid compiler optimization
        if (codes_used > 0xFFFFFFFF) {
            // This should never happen, but prevents dead code elimination
        }
    }
    
    // Step 9: inflateUndermine
    if (test_undermine && d_strm.avail_in > 0) {
        // Consume undermine parameter from remaining fuzzer data
        int undermine_param = 0;
        if (fdp.remaining_bytes() > 0) {
            undermine_param = fdp.ConsumeIntegral<uint8_t>() % 4;
        }
        
        result = inflateUndermine(&d_strm, undermine_param);
        // Continue regardless of result
    }
    
    // Continue decompression to consume remaining data
    if (d_strm.avail_in > 0 && d_strm.avail_out > 0) {
        result = inflate(&d_strm, Z_FINISH);
    }
    
    // Step 10: inflateEnd
    result = inflateEnd(&d_strm);
    
    // Clean up gzip header allocations if used
    if (test_header_retrieval && use_gzip_header) {
        free(gzheader.extra);
        free(gzheader.name);
        free(gzheader.comment);
    }
    
    // Step 11: deflateEnd
    result = deflateEnd(&c_strm);
    
    return 0;
}

} // namespace harness_012
