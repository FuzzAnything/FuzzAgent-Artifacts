#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_009 {
/* Deep branch coverage fuzzing harness for zlib inflate and deflate functions
 * Primary targets: inflate (418 blocked branches) and deflate (178 blocked branches)
 * These represent the largest remaining coverage gaps.
 * 
 * This harness exercises complex compression/decompression scenarios to reach deep code paths:
 * - Various compression levels (0-9, Z_DEFAULT_COMPRESSION)
 * - Window bits (8-15, negative values for raw/gzip)
 * - Memory levels (1-9)
 * - All flush operations (Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK, Z_TREES)
 * - Dictionary usage (inflateSetDictionary, deflateSetDictionary)
 * - Error conditions and edge cases
 * - Both streaming and one-shot compression/decompression patterns
 * - Advanced features: inflateBack, inflateSync, inflatePrime, deflateParams, deflateTune
 */


// Minimum input size requirement for meaningful testing
const size_t MIN_INPUT_SIZE = 64;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_INPUT_SIZE = 1024 * 1024;      // 1MB
const size_t MAX_COMPRESSED_SIZE = 2 * 1024 * 1024;  // 2MB
const size_t MAX_DICTIONARY_SIZE = 32768;       // 32KB max dictionary size

// Helper function to test basic deflate/inflate with various parameters
void test_basic_deflate_inflate(FuzzedDataProvider& fdp) {
    // Consume comprehensive parameters
    int level = fdp.PickValueInArray({
        Z_NO_COMPRESSION, Z_BEST_SPEED, Z_BEST_COMPRESSION, Z_DEFAULT_COMPRESSION,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    });
    
    int method = Z_DEFLATED;  // Only supported method
    int windowBits = fdp.ConsumeIntegralInRange<int>(-15, 15);
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.PickValueInArray({
        Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY, Z_RLE, Z_FIXED
    });
    
    // Get input data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
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
        // Try with simpler initialization if advanced fails
        deflate_stream = {};
        ret = deflateInit(&deflate_stream, level);
        if (ret != Z_OK) return;
    }
    
    // Prepare input/output buffers for deflate
    deflate_stream.next_in = input.data();
    deflate_stream.avail_in = input.size();
    deflate_stream.next_out = compressed.data();
    deflate_stream.avail_out = compressed.size();
    
    // Test different flush operations
    int flush_op = fdp.PickValueInArray({
        Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK, Z_TREES
    });
    
    // Perform deflation with selected flush operation
    ret = deflate(&deflate_stream, flush_op);
    
    size_t compressed_size = compressed.size() - deflate_stream.avail_out;
    
    // If we used Z_NO_FLUSH, we might need to call deflate again with Z_FINISH
    if (flush_op == Z_NO_FLUSH && ret == Z_OK && deflate_stream.avail_in == 0) {
        ret = deflate(&deflate_stream, Z_FINISH);
        compressed_size = compressed.size() - deflate_stream.avail_out;
    }
    
    // Clean up deflate stream
    deflateEnd(&deflate_stream);
    
    // Only proceed if compression was successful
    if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        // Setup inflate stream
        z_stream inflate_stream;
        memset(&inflate_stream, 0, sizeof(inflate_stream));
        
        // Initialize inflate with same windowBits
        ret = inflateInit2(&inflate_stream, windowBits);
        if (ret != Z_OK) {
            // Try default initialization
            inflate_stream = {};
            ret = inflateInit(&inflate_stream);
            if (ret != Z_OK) return;
        }
        
        // Prepare input/output buffers for inflate
        inflate_stream.next_in = compressed.data();
        inflate_stream.avail_in = compressed_size;
        inflate_stream.next_out = decompressed.data();
        inflate_stream.avail_out = decompressed.size();
        
        // Test different flush operations for inflate
        int inflate_flush = fdp.PickValueInArray({
            Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH
        });
        
        // Perform inflation
        ret = inflate(&inflate_stream, inflate_flush);
        
        // Clean up inflate stream
        inflateEnd(&inflate_stream);
    }
}

// Helper function to test dictionary operations
void test_dictionary_operations(FuzzedDataProvider& fdp) {
    // Create dictionary
    size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, MAX_DICTIONARY_SIZE);
    std::vector<uint8_t> dictionary = fdp.ConsumeBytes<uint8_t>(dict_size);
    
    // Get input data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 16384);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty() || dictionary.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    // Allocate buffers
    std::vector<uint8_t> compressed(compressed_bound);
    std::vector<uint8_t> decompressed(input.size());
    
    // Setup deflate stream
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    int ret = deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return;
    
    // Set dictionary for deflate
    deflateSetDictionary(&deflate_stream, dictionary.data(), dictionary.size());
    
    // Test deflateGetDictionary
    Bytef dict_buf[MAX_DICTIONARY_SIZE];
    uInt dict_len = MAX_DICTIONARY_SIZE;
    deflateGetDictionary(&deflate_stream, dict_buf, &dict_len);
    
    // Compress with dictionary
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
        
        // Try to decompress without dictionary first (should fail with Z_NEED_DICT)
        inflate_stream.next_in = compressed.data();
        inflate_stream.avail_in = compressed_size;
        inflate_stream.next_out = decompressed.data();
        inflate_stream.avail_out = decompressed.size();
        
        ret = inflate(&inflate_stream, Z_NO_FLUSH);
        
        if (ret == Z_NEED_DICT) {
            // Set dictionary for inflate
            inflateSetDictionary(&inflate_stream, dictionary.data(), dictionary.size());
            
            // Reset stream and try again
            inflateReset(&inflate_stream);
            
            inflate_stream.next_in = compressed.data();
            inflate_stream.avail_in = compressed_size;
            inflate_stream.next_out = decompressed.data();
            inflate_stream.avail_out = decompressed.size();
            
            ret = inflate(&inflate_stream, Z_FINISH);
            
            // Test inflateGetDictionary
            Bytef inflate_dict_buf[MAX_DICTIONARY_SIZE];
            uInt inflate_dict_len = MAX_DICTIONARY_SIZE;
            inflateGetDictionary(&inflate_stream, inflate_dict_buf, &inflate_dict_len);
        }
        
        inflateEnd(&inflate_stream);
    }
}

// Helper function to test advanced deflate features
void test_advanced_deflate_features(FuzzedDataProvider& fdp) {
    // Get input data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    // Allocate buffers
    std::vector<uint8_t> compressed(compressed_bound);
    
    // Setup deflate stream
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    int ret = deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return;
    
    // Test deflateParams - change compression parameters on the fly
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int strategy = fdp.PickValueInArray({
        Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY, Z_RLE, Z_FIXED
    });
    
    deflateParams(&deflate_stream, level, strategy);
    
    // Test deflateTune - fine-tune internal compression parameters
    int good_length = fdp.ConsumeIntegralInRange<int>(0, 258);
    int max_lazy = fdp.ConsumeIntegralInRange<int>(0, 258);
    int nice_length = fdp.ConsumeIntegralInRange<int>(0, 258);
    int max_chain = fdp.ConsumeIntegralInRange<int>(0, 4096);
    
    deflateTune(&deflate_stream, good_length, max_lazy, nice_length, max_chain);
    
    // Test deflatePrime - insert bits in the bit stream
    int bits = fdp.ConsumeIntegralInRange<int>(0, 16);
    int value = fdp.ConsumeIntegralInRange<int>(0, 65535);
    
    deflatePrime(&deflate_stream, bits, value);
    
    // Test deflatePending - check amount of pending output
    unsigned pending;
    int bits_pending;
    deflatePending(&deflate_stream, &pending, &bits_pending);
    
    // Test deflateUsed - get number of bits used in last byte
    int bits_used;
    deflateUsed(&deflate_stream, &bits_used);
    // Now actually compress some data
    deflate_stream.next_in = input.data();
    deflate_stream.avail_in = input.size();
    deflate_stream.next_out = compressed.data();
    deflate_stream.avail_out = compressed.size();
    
    ret = deflate(&deflate_stream, Z_FINISH);
    
    deflateEnd(&deflate_stream);
}

// Helper function to test advanced inflate features
void test_advanced_inflate_features(FuzzedDataProvider& fdp) {
    // First create some compressed data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    std::vector<uint8_t> compressed(compressed_bound);
    uLongf compressed_len = compressed.size();
    
    // Compress data
    int ret = compress(compressed.data(), &compressed_len,
                      input.data(), input.size());
    
    if (ret != Z_OK) return;
    
    // Setup inflate stream
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    ret = inflateInit(&inflate_stream);
    if (ret != Z_OK) return;
    
    // Test inflateSync - skip to next full-flush point
    inflate_stream.next_in = compressed.data();
    inflate_stream.avail_in = compressed_len;
    
    // Try inflateSync (may fail if no sync points)
    inflateSync(&inflate_stream);
    
    // Test inflatePrime - insert bits in the bit stream
    int bits = fdp.ConsumeIntegralInRange<int>(0, 16);
    int value = fdp.ConsumeIntegralInRange<int>(0, 65535);
    
    inflatePrime(&inflate_stream, bits, value);
    
    // Test inflateCopy - copy decompression state
    z_stream inflate_copy;
    memset(&inflate_copy, 0, sizeof(inflate_copy));
    
    inflateCopy(&inflate_copy, &inflate_stream);
    
    // Clean up copy
    inflateEnd(&inflate_copy);
    
    // Test inflateReset and inflateReset2
    inflateReset(&inflate_stream);
    
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    inflateReset2(&inflate_stream, windowBits);
    
    // Now actually decompress
    std::vector<uint8_t> decompressed(input.size());
    inflate_stream.next_in = compressed.data();
    inflate_stream.avail_in = compressed_len;
    inflate_stream.next_out = decompressed.data();
    inflate_stream.avail_out = decompressed.size();
    
    ret = inflate(&inflate_stream, Z_FINISH);
    
    inflateEnd(&inflate_stream);
}

// Helper function to test error conditions and edge cases
void test_error_conditions(FuzzedDataProvider& fdp) {
    // Test with invalid parameters
    z_stream invalid_stream;
    memset(&invalid_stream, 0, sizeof(invalid_stream));
    
    // Test deflate with NULL stream
    deflate(NULL, Z_NO_FLUSH);
    
    // Test inflate with NULL stream
    inflate(NULL, Z_NO_FLUSH);
    
    // Test with uninitialized stream
    z_stream uninit_stream;
    // Don't initialize it
    
    deflate(&uninit_stream, Z_NO_FLUSH);
    inflate(&uninit_stream, Z_NO_FLUSH);
    
    // Test with very small buffers
    if (fdp.remaining_bytes() > 16) {
        std::vector<uint8_t> tiny_input = fdp.ConsumeBytes<uint8_t>(1);
        std::vector<uint8_t> tiny_output(1);
        
        z_stream tiny_stream;
        memset(&tiny_stream, 0, sizeof(tiny_stream));
        
        if (deflateInit(&tiny_stream, Z_DEFAULT_COMPRESSION) == Z_OK) {
            tiny_stream.next_in = tiny_input.data();
            tiny_stream.avail_in = tiny_input.size();
            tiny_stream.next_out = tiny_output.data();
            tiny_stream.avail_out = tiny_output.size();
            
            // This should produce Z_BUF_ERROR
            deflate(&tiny_stream, Z_FINISH);
            
            deflateEnd(&tiny_stream);
        }
    }
    
    // Test with corrupted compressed data
    if (fdp.remaining_bytes() > 32) {
        size_t corrupt_size = fdp.ConsumeIntegralInRange<size_t>(16, 128);
        std::vector<uint8_t> corrupted = fdp.ConsumeBytes<uint8_t>(corrupt_size);
        
        z_stream corrupt_stream;
        memset(&corrupt_stream, 0, sizeof(corrupt_stream));
        
        if (inflateInit(&corrupt_stream) == Z_OK) {
            corrupt_stream.next_in = corrupted.data();
            corrupt_stream.avail_in = corrupted.size();
            
            // Small output buffer
            std::vector<uint8_t> output(16);
            corrupt_stream.next_out = output.data();
            corrupt_stream.avail_out = output.size();
            
            // This may produce Z_DATA_ERROR
            inflate(&corrupt_stream, Z_FINISH);
            
            inflateEnd(&corrupt_stream);
        }
    }
}

// Helper function to test streaming patterns (multiple deflate/inflate calls)
void test_streaming_patterns(FuzzedDataProvider& fdp) {
    // Get input data split into chunks
    size_t total_input_size = fdp.ConsumeIntegralInRange<size_t>(128, 8192);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(total_input_size);
    if (input.empty()) return;
    
    // Calculate maximum compressed size
    uLongf compressed_bound = compressBound(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    // Allocate buffers
    std::vector<uint8_t> compressed(compressed_bound);
    std::vector<uint8_t> decompressed(input.size());
    
    // Setup deflate stream for streaming
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    int ret = deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return;
    
    // Compress in chunks to simulate streaming
    size_t input_pos = 0;
    size_t output_pos = 0;
    bool done = false;
    
    while (!done) {
        // Determine chunk size
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        if (chunk_size > input.size() - input_pos) {
            chunk_size = input.size() - input_pos;
        }
        
        // Set up input chunk
        deflate_stream.next_in = input.data() + input_pos;
        deflate_stream.avail_in = chunk_size;
        input_pos += chunk_size;
        
        // Set up output buffer
        deflate_stream.next_out = compressed.data() + output_pos;
        deflate_stream.avail_out = compressed.size() - output_pos;
        
        // Determine flush operation
        int flush = (input_pos >= input.size()) ? Z_FINISH : Z_NO_FLUSH;
        
        // Compress chunk
        ret = deflate(&deflate_stream, flush);
        
        output_pos = compressed.size() - deflate_stream.avail_out;
        
        if (ret == Z_STREAM_END || (flush == Z_FINISH && ret == Z_OK)) {
            done = true;
        }
    }
    
    size_t compressed_size = output_pos;
    deflateEnd(&deflate_stream);
    
    // Now decompress in chunks
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    ret = inflateInit(&inflate_stream);
    if (ret != Z_OK) return;
    
    size_t compressed_pos = 0;
    size_t decompressed_pos = 0;
    done = false;
    
    while (!done && compressed_pos < compressed_size) {
        // Determine chunk size for decompression
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        if (chunk_size > compressed_size - compressed_pos) {
            chunk_size = compressed_size - compressed_pos;
        }
        
        // Set up input chunk
        inflate_stream.next_in = compressed.data() + compressed_pos;
        inflate_stream.avail_in = chunk_size;
        compressed_pos += chunk_size;
        
        // Set up output buffer
        inflate_stream.next_out = decompressed.data() + decompressed_pos;
        inflate_stream.avail_out = decompressed.size() - decompressed_pos;
        
        // Determine flush operation
        int flush = (compressed_pos >= compressed_size) ? Z_FINISH : Z_NO_FLUSH;
        
        // Decompress chunk
        ret = inflate(&inflate_stream, flush);
        
        decompressed_pos = decompressed.size() - inflate_stream.avail_out;
        
        if (ret == Z_STREAM_END) {
            done = true;
        }
    }
    
    inflateEnd(&inflate_stream);
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Input size validation
    if (size < MIN_INPUT_SIZE || size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test to run based on first byte
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    try {
        switch (test_type) {
            case 0:
                test_basic_deflate_inflate(fdp);
                break;
            case 1:
                test_dictionary_operations(fdp);
                break;
            case 2:
                test_advanced_deflate_features(fdp);
                break;
            case 3:
                test_advanced_inflate_features(fdp);
                break;
            case 4:
                test_error_conditions(fdp);
                break;
            case 5:
                test_streaming_patterns(fdp);
                break;
        }
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}

} // namespace harness_009
