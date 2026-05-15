#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_000 {
/* Comprehensive fuzzing harness for zlib library
 * Targets core compression/decompression APIs:
 * - compress(), compress2(), uncompress(), uncompress2()
 * - deflateInit(), deflate(), deflateEnd()
 * - inflateInit(), inflate(), inflateEnd()
 * - Various compression levels, window bits, and memory strategies
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 16;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_INPUT_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_COMPRESSED_SIZE = 2 * 1024 * 1024;  // 2MB

// Helper function to test compress/uncompress APIs
void test_compress_uncompress(FuzzedDataProvider& fdp) {
    // Consume parameters for compression
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    
    // Get input data
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    // Allocate buffers
    std::vector<uint8_t> compressed(compressed_bound);
    std::vector<uint8_t> decompressed(input.size());
    
    uLongf compressed_len = compressed.size();
    uLongf decompressed_len = decompressed.size();
    
    // Test compress2() and uncompress()
    int ret = compress2(compressed.data(), &compressed_len,
                        input.data(), input.size(), level);
    if (ret == Z_OK) {
        // Try to decompress
        decompressed_len = decompressed.size();
        int uncompress_ret = uncompress(decompressed.data(), &decompressed_len,
                                        compressed.data(), compressed_len);
        
        // If decompression succeeded, verify data matches
        if (uncompress_ret == Z_OK && decompressed_len == input.size()) {
            // Data should match (but we don't assert in fuzzing harness)
        }
    }
    
    // Test compress() (default compression level)
    compressed_len = compressed.size();
    ret = compress(compressed.data(), &compressed_len,
                   input.data(), input.size());
    if (ret == Z_OK) {
        decompressed_len = decompressed.size();
        uncompress(decompressed.data(), &decompressed_len,
                   compressed.data(), compressed_len);
    }
}

// Helper function to test stream-based deflate/inflate APIs
void test_deflate_inflate(FuzzedDataProvider& fdp) {
    // Consume parameters for deflate/inflate
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int method = Z_DEFLATED;  // Only supported method
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.PickValueInArray({
        Z_DEFAULT_STRATEGY,
        Z_FILTERED,
        Z_HUFFMAN_ONLY,
        Z_RLE,
        Z_FIXED
    });
    
    // Get input data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    // Allocate buffers
    std::vector<uint8_t> compressed(compressed_bound);
    std::vector<uint8_t> decompressed(input.size());
    
    // Setup deflate stream
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    // Initialize deflate with advanced parameters
    int ret = deflateInit2(&deflate_stream, level, method, windowBits,
                           memLevel, strategy);
    if (ret != Z_OK) {
        return;  // Initialization failed
    }
    
    // Prepare input/output buffers for deflate
    deflate_stream.next_in = input.data();
    deflate_stream.avail_in = input.size();
    deflate_stream.next_out = compressed.data();
    deflate_stream.avail_out = compressed.size();
    
    // Perform deflation
    int flush = fdp.ConsumeBool() ? Z_FINISH : Z_NO_FLUSH;
    ret = deflate(&deflate_stream, flush);
    
    size_t compressed_size = compressed.size() - deflate_stream.avail_out;
    
    // Clean up deflate stream
    deflateEnd(&deflate_stream);
    
    // Only proceed if compression was successful
    if (ret == Z_OK || ret == Z_STREAM_END) {
        // Setup inflate stream
        z_stream inflate_stream;
        memset(&inflate_stream, 0, sizeof(inflate_stream));
        
        // Initialize inflate
        ret = inflateInit2(&inflate_stream, windowBits);
        if (ret != Z_OK) {
            return;  // Initialization failed
        }
        
        // Prepare input/output buffers for inflate
        inflate_stream.next_in = compressed.data();
        inflate_stream.avail_in = compressed_size;
        inflate_stream.next_out = decompressed.data();
        inflate_stream.avail_out = decompressed.size();
        
        // Perform inflation
        ret = inflate(&inflate_stream, Z_NO_FLUSH);
        
        // Clean up inflate stream
        inflateEnd(&inflate_stream);
    }
}

// Helper function to test uncompress2() API
void test_uncompress2(FuzzedDataProvider& fdp) {
    // First create some compressed data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    std::vector<uint8_t> compressed(compressed_bound);
    uLongf compressed_len = compressed.size();
    
    // Compress data with default level
    int ret = compress(compressed.data(), &compressed_len,
                       input.data(), input.size());
    
    if (ret == Z_OK) {
        // Prepare for uncompress2
        std::vector<uint8_t> decompressed(input.size());
        uLongf decompressed_len = decompressed.size();
        uLong source_len = compressed_len;
        
        // Test uncompress2
        ret = uncompress2(decompressed.data(), &decompressed_len,
                          compressed.data(), &source_len);
    }
}

// Helper function to test deflate/inflate with dictionary
void test_with_dictionary(FuzzedDataProvider& fdp) {
    // Create dictionary
    size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::vector<uint8_t> dictionary = fdp.ConsumeBytes<uint8_t>(dict_size);
    
    // Get input data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty() || dictionary.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    // Allocate buffers
    std::vector<uint8_t> compressed(compressed_bound);
    std::vector<uint8_t> decompressed(input.size());
    
    // Setup deflate stream with dictionary
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    int ret = deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return;
    
    // Set dictionary for deflate
    deflateSetDictionary(&deflate_stream, dictionary.data(), dictionary.size());
    
    // Compress
    deflate_stream.next_in = input.data();
    deflate_stream.avail_in = input.size();
    deflate_stream.next_out = compressed.data();
    deflate_stream.avail_out = compressed.size();
    
    ret = deflate(&deflate_stream, Z_FINISH);
    size_t compressed_size = compressed.size() - deflate_stream.avail_out;
    
    deflateEnd(&deflate_stream);
    
    if (ret == Z_STREAM_END) {
        // Setup inflate stream
        z_stream inflate_stream;
        memset(&inflate_stream, 0, sizeof(inflate_stream));
        
        ret = inflateInit(&inflate_stream);
        if (ret != Z_OK) return;
        
        // Set same dictionary for inflate
        inflateSetDictionary(&inflate_stream, dictionary.data(), dictionary.size());
        
        // Decompress
        inflate_stream.next_in = compressed.data();
        inflate_stream.avail_in = compressed_size;
        inflate_stream.next_out = decompressed.data();
        inflate_stream.avail_out = decompressed.size();
        
        ret = inflate(&inflate_stream, Z_FINISH);
        
        inflateEnd(&inflate_stream);
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Input size validation
    if (size < MIN_INPUT_SIZE || size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test to run based on first byte
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    try {
        switch (test_type) {
            case 0:
                test_compress_uncompress(fdp);
                break;
            case 1:
                test_deflate_inflate(fdp);
                break;
            case 2:
                test_uncompress2(fdp);
                break;
            case 3:
                test_with_dictionary(fdp);
                break;
        }
    } catch (...) {
        // Catch any exceptions from FuzzedDataProvider
        // and continue without crashing
    }
    
    return 0;
}

} // namespace harness_000
