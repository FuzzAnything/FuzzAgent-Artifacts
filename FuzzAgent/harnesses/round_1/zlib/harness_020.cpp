#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_020 {
/*
 * Fuzzing harness for zlib comprehensive inflate state machine testing
 * Target: Reach 442 blocked branches in inflate function - the largest coverage gap
 * 
 * Focus: Systematic exercise of the entire inflate state machine including:
 * - inflateInit2_ with various windowBits (zlib, raw, gzip modes)
 * - inflateSetDictionary and inflateGetDictionary for dictionary handling
 * - inflate with different flush parameters and state transitions
 * - inflateSync for corrupted data recovery
 * - inflateReset and inflateReset2 for state resetting
 * - inflatePrime for bit buffer edge cases  
 * - inflateCopy for state duplication testing
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_019: Traditional inflate API with multiple entry points
 * - This harness: Full state machine exercise with emphasis on inflatePrime, inflateCopy, inflateSync
 *   and systematic testing of state transitions between all related APIs
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration parameters first
    int windowBits_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 4);
    bool test_prime = fdp.ConsumeBool();
    bool test_copy = fdp.ConsumeBool();
    bool test_sync = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_reset = fdp.ConsumeBool();
    
    // Set windowBits based on mode
    int windowBits = 15;  // Default zlib
    switch (windowBits_mode) {
        case 0:  // Raw mode (negative windowBits)
            windowBits = -fdp.ConsumeIntegralInRange<int>(8, 15);
            break;
        case 1:  // Zlib mode (positive windowBits)
            windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
            break;
        case 2:  // Gzip mode (windowBits + 16)
            windowBits = fdp.ConsumeIntegralInRange<int>(8, 15) + 16;
            break;
        case 3:  // Auto detect gzip/zlib (windowBits + 32)
            windowBits = fdp.ConsumeIntegralInRange<int>(8, 15) + 32;
            break;
    }
    
    // Set flush parameter
    int flush = Z_NO_FLUSH;
    switch (flush_mode) {
        case 0: flush = Z_NO_FLUSH; break;
        case 1: flush = Z_SYNC_FLUSH; break;
        case 2: flush = Z_FULL_FLUSH; break;
        case 3: flush = Z_FINISH; break;
        case 4: flush = Z_BLOCK; break;
    }
    
    // Split remaining data: first part for compressed data, second for dictionary
    size_t compressed_data_size = fdp.ConsumeIntegralInRange<size_t>(256, 4096);
    if (fdp.remaining_bytes() < compressed_data_size) {
        return 0;
    }
    
    std::vector<uint8_t> compressed_data = fdp.ConsumeBytes<uint8_t>(compressed_data_size);
    
    // Prepare dictionary if needed
    std::vector<uint8_t> dictionary_data;
    if (test_dictionary && fdp.remaining_bytes() > 32) {
        size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(32, 1024);
        if (fdp.remaining_bytes() >= dict_size) {
            dictionary_data = fdp.ConsumeBytes<uint8_t>(dict_size);
        }
    }
    
    // Main test: Initialize and exercise the inflate state machine
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    // Step 1: Initialize with inflateInit2_
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;  // Initialization failed
    }
    
    // Set up input/output buffers
    std::vector<uint8_t> output_buffer(8192);  // 8KB output buffer
    strm.next_in = compressed_data.data();
    strm.avail_in = compressed_data.size();
    strm.next_out = output_buffer.data();
    strm.avail_out = output_buffer.size();
    
    // Step 2: Test inflatePrime if requested
    if (test_prime && compressed_data.size() > 0) {
        // Prime with some bits from the input
        int bits = fdp.ConsumeIntegralInRange<int>(1, 16);
        int value = fdp.ConsumeIntegralInRange<int>(0, (1 << bits) - 1);
        inflatePrime(&strm, bits, value);
    }
    
    // Step 3: Perform initial inflation
    int inflate_ret = inflate(&strm, Z_NO_FLUSH);
    
    // Step 4: Test dictionary operations if applicable
    if (test_dictionary && !dictionary_data.empty() && inflate_ret == Z_NEED_DICT) {
        // Set dictionary when inflate requests it
        inflateSetDictionary(&strm, dictionary_data.data(), dictionary_data.size());
        
        // Continue inflation after setting dictionary
        inflate_ret = inflate(&strm, flush);
    } else if (test_dictionary && !dictionary_data.empty()) {
        // Try to set dictionary even if not requested (for raw mode)
        inflateSetDictionary(&strm, dictionary_data.data(), dictionary_data.size());
    }
    
    // Step 5: Test inflateGetDictionary
    if (test_dictionary) {
        uInt dict_len = 0;
        Bytef dict_buffer[32768];  // Maximum dictionary size
        
        // First get the length
        inflateGetDictionary(&strm, Z_NULL, &dict_len);
        
        // Then get the actual dictionary if there is one
        if (dict_len > 0 && dict_len <= sizeof(dict_buffer)) {
            inflateGetDictionary(&strm, dict_buffer, &dict_len);
        }
    }
    
    // Step 6: Test inflateSync for recovery
    if (test_sync && inflate_ret == Z_DATA_ERROR) {
        // Try to sync after data error
        inflateSync(&strm);
        
        // Continue inflation after sync
        if (strm.avail_in > 0) {
            inflate(&strm, flush);
        }
    }
    
    // Step 7: Test inflateCopy if requested
    z_stream strm_copy;
    if (test_copy) {
        strm_copy.zalloc = Z_NULL;
        strm_copy.zfree = Z_NULL;
        strm_copy.opaque = Z_NULL;
        
        int copy_ret = inflateCopy(&strm_copy, &strm);
        if (copy_ret == Z_OK) {
            // Exercise the copied stream
            if (strm_copy.avail_in > 0) {
                inflate(&strm_copy, flush);
            }
            
            // Clean up the copy
            inflateEnd(&strm_copy);
        }
    }
    
    // Step 8: Test reset operations
    if (test_reset) {
        // Test inflateReset
        inflateReset(&strm);
        
        // Reset input/output pointers
        strm.next_in = compressed_data.data();
        strm.avail_in = compressed_data.size() / 2;  // Use half the data
        strm.next_out = output_buffer.data();
        strm.avail_out = output_buffer.size();
        
        // Inflate again after reset
        inflate(&strm, flush);
        
        // Test inflateReset2 with different windowBits
        if (windowBits > 0) {
            inflateReset2(&strm, windowBits - 1);  // Try different window size
            
            // Inflate with new window size
            strm.next_in = compressed_data.data() + compressed_data.size() / 2;
            strm.avail_in = compressed_data.size() / 2;
            strm.next_out = output_buffer.data();
            strm.avail_out = output_buffer.size();
            
            inflate(&strm, flush);
        }
    }
    
    // Step 9: Continue inflation with different flush parameters
    if (strm.avail_in > 0) {
        // Try different flush modes
        int extra_flush = fdp.ConsumeIntegralInRange<int>(0, 2);
        switch (extra_flush) {
            case 0: inflate(&strm, Z_SYNC_FLUSH); break;
            case 1: inflate(&strm, Z_FULL_FLUSH); break;
            case 2: inflate(&strm, Z_FINISH); break;
        }
    }
    
    // Step 10: Cleanup
    inflateEnd(&strm);
    
    // Additional test: Create a new stream for one-shot testing
    if (fdp.remaining_bytes() > 100) {
        z_stream strm2;
        strm2.zalloc = Z_NULL;
        strm2.zfree = Z_NULL;
        strm2.opaque = Z_NULL;
        
        size_t remaining_data_size = fdp.ConsumeIntegralInRange<size_t>(100, 1024);
        if (fdp.remaining_bytes() >= remaining_data_size) {
            std::vector<uint8_t> remaining_data = fdp.ConsumeBytes<uint8_t>(remaining_data_size);
            
            inflateInit2(&strm2, windowBits);
            strm2.next_in = remaining_data.data();
            strm2.avail_in = remaining_data.size();
            strm2.next_out = output_buffer.data();
            strm2.avail_out = output_buffer.size();
            
            // Test with Z_BLOCK flush mode (less commonly used)
            inflate(&strm2, Z_BLOCK);
            
            inflateEnd(&strm2);
        }
    }
    
    return 0;
}

} // namespace harness_020
