#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_000 {
/*
 * Fuzzing harness for zlib compression library
 * Targets: compress(), compress2(), uncompress(), compressBound()
 * 
 * This harness exercises basic compression and decompression workflows
 * using the zlib in-memory compression functions.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);  // Compression level 0-9
    
    // Get source data from remaining input
    size_t source_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024 * 1024);
    if (source_len > fdp.remaining_bytes()) {
        source_len = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> source_data = fdp.ConsumeBytes<uint8_t>(source_len);
    
    // Calculate bound for compressed data
    uLongf bound = compressBound((uLong)source_len);
    if (bound == 0) {
        return 0; // Invalid bound
    }
    
    // Allocate buffers
    Bytef* compr = (Bytef*)malloc(bound);
    Bytef* uncompr = (Bytef*)malloc(source_len);
    
    if (!compr || !uncompr) {
        free(compr);
        free(uncompr);
        return 0;
    }
    
    uLongf compr_len = bound;
    uLongf uncompr_len = source_len;
    
    // Test 1: compress() and uncompress()
    int err = compress(compr, &compr_len, source_data.data(), (uLong)source_len);
    if (err == Z_OK) {
        // Try to decompress
        uLongf test_uncompr_len = (uLongf)source_len;
        Bytef* test_uncompr = (Bytef*)malloc(source_len);
        if (test_uncompr) {
            int uncompress_err = uncompress(test_uncompr, &test_uncompr_len, 
                                           compr, compr_len);
            // Don't check result - just exercise the API
            free(test_uncompr);
        }
    }
    
    // Test 2: compress2() with specific level
    compr_len = bound; // Reset length
    err = compress2(compr, &compr_len, source_data.data(), (uLong)source_len, level);
    if (err == Z_OK) {
        // Try to decompress
        uLongf test_uncompr_len = (uLongf)source_len;
        Bytef* test_uncompr = (Bytef*)malloc(source_len);
        if (test_uncompr) {
            int uncompress_err = uncompress(test_uncompr, &test_uncompr_len,
                                           compr, compr_len);
            // Don't check result - just exercise the API
            free(test_uncompr);
        }
    }
    
    // Test 3: Direct uncompress of source data (may fail, but that's OK)
    if (source_len > 0) {
        uLongf test_uncompr_len = (uLongf)(source_len * 2); // Give some room
        Bytef* test_uncompr = (Bytef*)malloc(test_uncompr_len);
        if (test_uncompr) {
            // This may fail if source data isn't valid compressed data
            int uncompress_err = uncompress(test_uncompr, &test_uncompr_len,
                                           source_data.data(), (uLong)source_len);
            free(test_uncompr);
        }
    }
    
    // Cleanup
    free(compr);
    free(uncompr);
    
    return 0;
}

} // namespace harness_000
