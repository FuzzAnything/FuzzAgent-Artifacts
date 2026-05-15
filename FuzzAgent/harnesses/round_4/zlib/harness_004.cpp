#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_004 {
/* Comprehensive fuzzing harness for advanced zlib stream state inspection and manipulation
 * Targets advanced stream state functions that are currently 0% covered:
 * - deflateGetDictionary: Retrieve compression dictionary
 * - deflatePending: Check pending output in compression stream
 * - deflatePrime: Insert bits into compression stream
 * - deflateCopy: Duplicate compression stream state
 * - inflateGetDictionary: Retrieve decompression dictionary
 * - inflatePrime: Insert bits into decompression stream
 * - inflateCopy: Duplicate decompression stream state
 * - inflateSync: Synchronize decompression stream
 * - inflateMark: Mark decompression point
 * - inflateSyncPoint: Check sync point in decompression stream
 * - inflateValidate: Validate decompression stream
 * - deflateBound: Calculate maximum compressed size (32-bit)
 * - deflateBound_z: Calculate maximum compressed size (size_t)
 * 
 * This harness targets 200+ undiscovered branches in deflate.c and inflate.c
 * focusing on stream state inspection and control functions.
 * 
 * Distinct from existing harnesses:
 * - harness_000: Basic compression/decompression APIs
 * - harness_001: Gzip file I/O operations
 * - harness_002: Callback-based inflation with custom I/O
 * - harness_003: Advanced gzip I/O operations
 * - harness_004: Advanced stream state inspection/control (this harness)
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 64;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_DICTIONARY_SIZE = 32768;  // Maximum dictionary size (32KB)
const size_t MAX_INPUT_DATA = 1024 * 1024;  // 1MB
const size_t MAX_WINDOW_BITS = 15;  // Maximum window bits for deflate/inflate

// Main fuzzing function for advanced stream state APIs
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for stream initialization
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, MAX_WINDOW_BITS);
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.PickValueInArray({
        Z_DEFAULT_STRATEGY,
        Z_FILTERED,
        Z_HUFFMAN_ONLY,
        Z_RLE,
        Z_FIXED
    });
    
    // Consume data for compression
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input_data.empty()) {
        return 0;
    }
    
    // Consume dictionary data
    size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, MAX_DICTIONARY_SIZE);
    std::vector<uint8_t> dictionary_data = fdp.ConsumeBytes<uint8_t>(dict_size);
    
    // Calculate maximum compressed size
    size_t compressed_bound = compressBound(input_data.size());
    if (compressed_bound > MAX_INPUT_DATA) {
        return 0;
    }
    
    // Allocate buffers
    std::vector<uint8_t> compressed_buffer(compressed_bound);
    std::vector<uint8_t> decompressed_buffer(input_data.size());
    
    // Step 1: Initialize compression stream
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    
    int ret = deflateInit2(&deflate_stream,
                          compression_level,
                          Z_DEFLATED,
                          window_bits,
                          mem_level,
                          strategy);
    if (ret != Z_OK) {
        return 0;
    }
    
    // Step 2: Initialize decompression stream
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    ret = inflateInit2(&inflate_stream, window_bits);
    if (ret != Z_OK) {
        deflateEnd(&deflate_stream);
        return 0;
    }
    
    // Step 3: Set dictionary for compression stream
    if (!dictionary_data.empty()) {
        deflateSetDictionary(&deflate_stream, dictionary_data.data(), dictionary_data.size());
    }
    
    // Step 4: Set dictionary for decompression stream (must match)
    if (!dictionary_data.empty()) {
        inflateSetDictionary(&inflate_stream, dictionary_data.data(), dictionary_data.size());
    }
    
    // Step 5: Perform some compression to advance stream state
    deflate_stream.next_in = input_data.data();
    deflate_stream.avail_in = input_data.size();
    deflate_stream.next_out = compressed_buffer.data();
    deflate_stream.avail_out = compressed_buffer.size();
    
    // Compress with Z_NO_FLUSH to leave data pending
    ret = deflate(&deflate_stream, Z_NO_FLUSH);
    // Don't check return value - we want to test APIs even if compression fails
    
    // Step 6: Perform some decompression to advance stream state
    inflate_stream.next_in = compressed_buffer.data();
    inflate_stream.avail_in = deflate_stream.total_out;
    inflate_stream.next_out = decompressed_buffer.data();
    inflate_stream.avail_out = decompressed_buffer.size();
    
    ret = inflate(&inflate_stream, Z_NO_FLUSH);
    // Don't check return value - we want to test APIs even if decompression fails
    
    // Step 7: Test deflateGetDictionary
    {
        std::vector<uint8_t> retrieved_dict(MAX_DICTIONARY_SIZE);
        uInt dict_len = retrieved_dict.size();
        deflateGetDictionary(&deflate_stream, retrieved_dict.data(), &dict_len);
    }
    
    // Step 8: Test deflatePending
    {
        unsigned pending = 0;
        int bits = 0;
        deflatePending(&deflate_stream, &pending, &bits);
    }
    
    // Step 9: Test deflatePrime
    {
        int bits = fdp.ConsumeIntegralInRange<int>(0, 16);
        int value = fdp.ConsumeIntegral<int>();
        deflatePrime(&deflate_stream, bits, value);
    }
    
    // Step 10: Test deflateCopy
    {
        z_stream dest_stream;
        memset(&dest_stream, 0, sizeof(z_stream));
        deflateCopy(&dest_stream, &deflate_stream);
        // Clean up copied stream if successful
        if (dest_stream.state != NULL) {
            deflateEnd(&dest_stream);
        }
    }
    
    // Step 11: Test inflateGetDictionary
    {
        std::vector<uint8_t> retrieved_dict(MAX_DICTIONARY_SIZE);
        uInt dict_len = retrieved_dict.size();
        inflateGetDictionary(&inflate_stream, retrieved_dict.data(), &dict_len);
    }
    
    // Step 12: Test inflatePrime
    {
        int bits = fdp.ConsumeIntegralInRange<int>(0, 16);
        int value = fdp.ConsumeIntegral<int>();
        inflatePrime(&inflate_stream, bits, value);
    }
    
    // Step 13: Test inflateCopy
    {
        z_stream dest_stream;
        memset(&dest_stream, 0, sizeof(z_stream));
        inflateCopy(&dest_stream, &inflate_stream);
        // Clean up copied stream if successful
        if (dest_stream.state != NULL) {
            inflateEnd(&dest_stream);
        }
    }
    
    // Step 14: Test inflateSync
    {
        inflateSync(&inflate_stream);
    }
    
    // Step 15: Test inflateMark
    {
        inflateMark(&inflate_stream);
    }
    
    // Step 16: Test inflateSyncPoint
    {
        inflateSyncPoint(&inflate_stream);
    }
    
    // Step 17: Test inflateValidate
    {
        inflateValidate(&inflate_stream, fdp.ConsumeIntegralInRange<int>(0, 1));
    }
    
    // Step 18: Test deflateBound
    {
        uLong source_len = fdp.ConsumeIntegralInRange<uLong>(1, 65536);
        deflateBound(&deflate_stream, source_len);
    }
    
    // Step 19: Test deflateBound_z
    {
        z_size_t source_len = fdp.ConsumeIntegralInRange<z_size_t>(1, 65536);
        deflateBound_z(&deflate_stream, source_len);
    }
    
    // Step 20: Clean up
    deflateEnd(&deflate_stream);
    inflateEnd(&inflate_stream);
    
    return 0;
}

} // namespace harness_004
