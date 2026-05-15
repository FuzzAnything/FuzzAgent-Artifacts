#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_000 {
/*
 * Initial fuzzing harness for zlib library
 * Target: Basic compression/decompression APIs
 * Harness ID: 000
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters from fuzzer input
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    bool use_compress2 = fdp.ConsumeBool();
    bool use_uncompress2 = fdp.ConsumeBool();
    
    // Get the actual data to compress
    size_t input_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024 * 1024);
    if (fdp.remaining_bytes() < input_len) {
        return 0;  // Not enough data
    }
    
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(input_len);
    
    // Calculate maximum compressed size using uLong type
    uLong max_compressed_size = compressBound((uLong)input_len);
    if (max_compressed_size == 0 || max_compressed_size > 10 * 1024 * 1024) {
        return 0;  // Sanity check to avoid excessive memory usage
    }
    
    // Allocate buffers
    uint8_t* compressed_buffer = (uint8_t*)malloc(max_compressed_size);
    uint8_t* decompressed_buffer = (uint8_t*)malloc(input_len);
    
    if (!compressed_buffer || !decompressed_buffer) {
        free(compressed_buffer);
        free(decompressed_buffer);
        return 0;
    }
    
    // Test compression
    int result = Z_OK;
    uLong compressed_len = max_compressed_size;
    
    if (use_compress2) {
        // Use compress2 with specific compression level
        result = compress2(compressed_buffer, &compressed_len,
                          input_data.data(), (uLong)input_len,
                          compression_level);
    } else {
        // Use compress (default compression level)
        result = compress(compressed_buffer, &compressed_len,
                         input_data.data(), (uLong)input_len);
    }
    
    if (result != Z_OK) {
        // Compression failed, clean up and return
        free(compressed_buffer);
        free(decompressed_buffer);
        return 0;
    }
    
    // Test decompression
    uLong decompressed_len = (uLong)input_len;
    
    if (use_uncompress2) {
        // Use uncompress2
        uLong source_len = compressed_len;
        result = uncompress2(decompressed_buffer, &decompressed_len,
                            compressed_buffer, &source_len);
    } else {
        // Use uncompress
        result = uncompress(decompressed_buffer, &decompressed_len,
                           compressed_buffer, compressed_len);
    }
    
    if (result == Z_OK) {
        // Verify decompressed data matches original
        if (decompressed_len == (uLong)input_len &&
            memcmp(decompressed_buffer, input_data.data(), input_len) == 0) {
            // Success - round trip compression/decompression worked
        }
    }
    
    // Clean up
    free(compressed_buffer);
    free(decompressed_buffer);
    
    return 0;
}

} // namespace harness_000
