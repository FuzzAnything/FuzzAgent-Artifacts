#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_013 {
/* Advanced fuzzing harness for zlib gzip decompression with varied header configurations
 * Primary target: inflate function (455 blocked branches, largest blocker in coverage)
 * Secondary target: gzip header handling via inflateGetHeader and inflateInit2 with gzip decoding
 * 
 * Critical coverage gaps identified:
 * - inflate function has 455 blocked branches (largest blocker)
 * - gzopen64 is completely uncovered (handled in harness_011 but with performance issues)
 * 
 * This harness focuses on:
 * 1. Gzip decompression via inflate() with varied windowBits settings (+32 for gzip auto-detection)
 * 2. Testing inflateGetHeader() to retrieve gzip header information
 * 3. Different gzip header configurations (text flag, timestamps, OS flags, extra fields, filenames, comments)
 * 4. Performance optimization: no file I/O, pure memory-based decompression
 * 
 * Distinct from existing harnesses:
 * - harness_000: Basic compression/decompression with compress/uncompress APIs
 * - harness_001: Gzip file I/O operations with file-based testing
 * - harness_002: Callback-based inflation (infback.c)
 * - harness_004-005: Other inflate variants
 * - harness_011: gzopen64 testing with file I/O (performance issues)
 * - harness_013: Pure memory gzip decompression with header variations (this harness)
 * 
 * Implementation strategy:
 * - Use inflateInit2() with windowBits + 32 for gzip auto-detection
 * - Test inflateGetHeader() to retrieve header info from gzip streams
 * - Generate gzip data with varied header configurations
 * - Test different windowBits values and decompression scenarios
 * - Focus on inflate()'s 455 blocked branches through varied input patterns
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 64;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_COMPRESSED_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_UNCOMPRESSED_SIZE = 2 * 1024 * 1024;  // 2MB
const size_t MAX_HEADER_STRING_LEN = 256;

// Helper function to generate gzip data with specific header configurations
std::vector<uint8_t> generate_gzip_data(FuzzedDataProvider& fdp, 
                                       gz_header& header_config,
                                       const std::vector<uint8_t>& content) {
    // Initialize deflate stream for gzip format
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use windowBits + 16 for gzip format
    int ret = deflateInit2(&strm, 
                          fdp.ConsumeIntegralInRange<int>(0, 9),  // compression level
                          Z_DEFLATED,
                          15 + 16,  // windowBits + 16 for gzip
                          8,        // memLevel
                          Z_DEFAULT_STRATEGY);
    
    if (ret != Z_OK) {
        return std::vector<uint8_t>();
    }
    
    // Set gzip header if provided
    if (header_config.extra != NULL || header_config.name != NULL || header_config.comment != NULL) {
        deflateSetHeader(&strm, &header_config);
    }
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(content.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) {
        deflateEnd(&strm);
        return std::vector<uint8_t>();
    }
    
    // Allocate buffer for compressed data
    std::vector<uint8_t> compressed(compressed_bound);
    
    // Set up input/output
    strm.next_in = const_cast<Bytef*>(content.data());
    strm.avail_in = content.size();
    strm.next_out = compressed.data();
    strm.avail_out = compressed.size();
    
    // Compress with Z_FINISH to complete the stream
    ret = deflate(&strm, Z_FINISH);
    
    std::vector<uint8_t> result;
    if (ret == Z_STREAM_END) {
        // Success - resize to actual compressed size
        result.resize(compressed.size() - strm.avail_out);
        memcpy(result.data(), compressed.data(), result.size());
    }
    
    deflateEnd(&strm);
    return result;
}
// Test gzip decompression with header retrieval
void test_gzip_decompression_with_headers(FuzzedDataProvider& fdp) {
    // Consume parameters for gzip decompression
    int window_bits_base = fdp.ConsumeIntegralInRange<int>(8, 15);
    bool use_gzip_auto_detect = fdp.ConsumeBool();
    bool use_gzip_only = fdp.ConsumeBool();
    
    // Determine windowBits for inflateInit2
    int window_bits = window_bits_base;
    if (use_gzip_auto_detect) {
        window_bits += 32;  // Auto-detect zlib/gzip
    } else if (use_gzip_only) {
        window_bits += 16;  // Gzip format only
    }
    
    // Consume compressed data size
    size_t compressed_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
    if (compressed_size > MAX_COMPRESSED_SIZE) {
        compressed_size = MAX_COMPRESSED_SIZE;
    }
    
    // Get compressed data from fuzzer input
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_size);
    if (compressed_data.empty()) {
        return;
    }
    
    // Allocate output buffer
    size_t output_size = compressed_size * 4;  // Reasonable expansion factor
    if (output_size > MAX_UNCOMPRESSED_SIZE) {
        output_size = MAX_UNCOMPRESSED_SIZE;
    }
    
    std::vector<uint8_t> decompressed(output_size);
    
    // Initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Set up input/output buffers
    strm.next_in = compressed_data.data();
    strm.avail_in = compressed_data.size();
    strm.next_out = decompressed.data();
    strm.avail_out = decompressed.size();
    
    // Initialize inflation with specified windowBits
    int ret = inflateInit2(&strm, window_bits);
    if (ret != Z_OK) {
        return;  // Initialization failed
    }
    
    // Set up gz_header for header retrieval (if using gzip mode)
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    if (use_gzip_auto_detect || use_gzip_only) {
        // Allocate buffers for header fields if we expect to retrieve them
        header.extra_max = 256;
        header.name_max = 256;
        header.comm_max = 256;
        
        // Call inflateGetHeader to request header retrieval
        inflateGetHeader(&strm, &header);
    }
    
    // Perform inflation
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Handle different return codes to explore more branches
    while (ret == Z_OK && strm.avail_in > 0 && strm.avail_out > 0) {
        ret = inflate(&strm, Z_NO_FLUSH);
    }
    
    // Test different flush modes
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        // Try finishing
        ret = inflate(&strm, Z_FINISH);
    }
    
    // Test stream reset if we have more data
    if (strm.avail_in > 0 && (ret == Z_STREAM_END || ret == Z_OK)) {
        // Reset the stream and try again with remaining data
        inflateReset(&strm);
        strm.next_in = compressed_data.data() + (compressed_data.size() - strm.avail_in);
        strm.next_out = decompressed.data();
        strm.avail_out = decompressed.size();
        
        ret = inflate(&strm, Z_NO_FLUSH);
        while (ret == Z_OK && strm.avail_in > 0 && strm.avail_out > 0) {
            ret = inflate(&strm, Z_NO_FLUSH);
        }
    }
    
    // Clean up
    inflateEnd(&strm);
}

// Test raw deflate data (non-gzip) with different windowBits
void test_raw_inflate_variations(FuzzedDataProvider& fdp) {
    // Test negative windowBits for raw inflate (-8 to -15)
    int raw_window_bits = -fdp.ConsumeIntegralInRange<int>(8, 15);
    
    // Consume compressed data
    size_t compressed_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_size);
    if (compressed_data.empty()) {
        return;
    }
    
    // Allocate output buffer
    std::vector<uint8_t> decompressed(compressed_size * 4);
    
    // Initialize for raw inflate
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.next_in = compressed_data.data();
    strm.avail_in = compressed_data.size();
    strm.next_out = decompressed.data();
    strm.avail_out = decompressed.size();
    
    int ret = inflateInit2(&strm, raw_window_bits);
    if (ret != Z_OK) {
        return;
    }
    
    // Perform inflation
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Try different flush modes
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        ret = inflate(&strm, Z_SYNC_FLUSH);
    }
    
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        ret = inflate(&strm, Z_FULL_FLUSH);
    }
    
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        ret = inflate(&strm, Z_FINISH);
    }
    
    inflateEnd(&strm);
}

// Test inflate with dictionary support
void test_inflate_with_dictionary(FuzzedDataProvider& fdp) {
    // Consume dictionary data
    size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::vector<uint8_t> dictionary = fdp.ConsumeBytes<uint8_t>(dict_size);
    
    // Consume compressed data
    size_t compressed_size = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_size);
    
    if (dictionary.empty() || compressed_data.empty()) {
        return;
    }
    
    // Allocate output buffer
    std::vector<uint8_t> decompressed(compressed_size * 4);
    
    // Initialize inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.next_in = compressed_data.data();
    strm.avail_in = compressed_data.size();
    strm.next_out = decompressed.data();
    strm.avail_out = decompressed.size();
    
    int ret = inflateInit2(&strm, 15);  // Default windowBits
    if (ret != Z_OK) {
        return;
    }
    
    // Set dictionary (this would normally be called after Z_NEED_DICT)
    // We're testing the API call itself
    inflateSetDictionary(&strm, dictionary.data(), dictionary.size());
    
    // Perform inflation
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // If we get Z_NEED_DICT, try setting dictionary again
    if (ret == Z_NEED_DICT) {
        inflateSetDictionary(&strm, dictionary.data(), dictionary.size());
        ret = inflate(&strm, Z_NO_FLUSH);
    }
    
    inflateEnd(&strm);
}

// Test various error conditions and edge cases
void test_inflate_edge_cases(FuzzedDataProvider& fdp) {
    // Test with zero-length input
    {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        int ret = inflateInit2(&strm, 15);
        if (ret == Z_OK) {
            // Call inflate with no input
            ret = inflate(&strm, Z_NO_FLUSH);
            inflateEnd(&strm);
        }
    }
    
    // Test with small buffers
    {
        uint8_t tiny_input[1] = {0};
        uint8_t tiny_output[1] = {0};
        
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        strm.next_in = tiny_input;
        strm.avail_in = sizeof(tiny_input);
        strm.next_out = tiny_output;
        strm.avail_out = sizeof(tiny_output);
        
        int ret = inflateInit2(&strm, 15);
        if (ret == Z_OK) {
            ret = inflate(&strm, Z_NO_FLUSH);
            inflateEnd(&strm);
        }
    }
    
    // Test invalid windowBits values
    {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        int invalid_window_bits = fdp.ConsumeIntegralInRange<int>(-100, 7);  // Invalid range
        int ret = inflateInit2(&strm, invalid_window_bits);
        if (ret == Z_OK) {
            inflateEnd(&strm);
        }
    }
}

// Main fuzzing function
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different scenarios
    uint8_t operation_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation_selector) {
        case 0:
            // Primary target: gzip decompression with header handling
            test_gzip_decompression_with_headers(fdp);
            break;
            
        case 1:
            // Raw inflate variations
            test_raw_inflate_variations(fdp);
            break;
            
        case 2:
            // Inflate with dictionary
            test_inflate_with_dictionary(fdp);
            break;
            
        case 3:
            // Edge cases and error conditions
            test_inflate_edge_cases(fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_013
