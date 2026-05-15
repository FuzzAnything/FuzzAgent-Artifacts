#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_016 {
/*
 * Fuzzing harness for zlib inflate error condition testing
 * Targets blocked branches and critical error condition at line 499 in inflate.c
 * Specifically triggers Z_STREAM_ERROR return paths by creating invalid stream states
 * Focuses on error handling paths rather than valid operations (semantic diversity from harnesses 000-015)
 * 
 * Error condition targeting:
 * - inflateStateCheck() validation failures
 * - strm->next_out == Z_NULL check
 * - strm->next_in == Z_NULL && strm->avail_in != 0 check
 * 
 * Based on coverage analysis: inflate has 435 blocked branches with critical error condition at line 499 completely uncovered
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and test data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Window bits for inflate
    int error_type = fdp.ConsumeIntegralInRange<int>(0, 5);   // Type of error to inject
    
    // Consume test data for compression (to have valid data to work with)
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (fdp.remaining_bytes() < test_data_size) {
        return 0;
    }
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // Create output buffer for valid operations
    size_t output_size = test_data_size * 2;  // Safe size for decompression
    Bytef* output_buffer = (Bytef*)malloc(output_size);
    if (!output_buffer) {
        return 0;
    }
    
    // 1. Call inflateInit_ to create a stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(output_buffer);
        return 0;  // Failed to initialize, can't proceed
    }
    
    // Based on error_type, manipulate stream state to create invalid conditions
    switch (error_type % 4) {
        case 0:
            // Test case a: Set strm->next_out = Z_NULL
            // Valid input but null output pointer should trigger Z_STREAM_ERROR
            strm.next_in = test_data.data();
            strm.avail_in = (uInt)test_data_size;
            strm.next_out = Z_NULL;  // Invalid: null output pointer
            strm.avail_out = (uInt)output_size;
            break;
            
        case 1:
            // Test case b: Set strm->next_in = Z_NULL while strm->avail_in > 0
            // Null input pointer with available input should trigger Z_STREAM_ERROR
            strm.next_in = Z_NULL;  // Invalid: null input pointer
            strm.avail_in = (uInt)test_data_size;  // But we have available input
            strm.next_out = output_buffer;
            strm.avail_out = (uInt)output_size;
            break;
            
        case 2:
            // Test case c: Corrupt stream state to fail inflateStateCheck
            // Corrupt the state pointer or other stream fields
            strm.next_in = test_data.data();
            strm.avail_in = (uInt)test_data_size;
            strm.next_out = output_buffer;
            strm.avail_out = (uInt)output_size;
            
            // Corrupt stream state to fail inflateStateCheck validation
            // Option 1: Set strm->state to NULL
            if (fdp.ConsumeBool()) {
                strm.state = Z_NULL;
            }
            // Option 2: Set strm->zalloc or strm->zfree to NULL (invalid alloc/free functions)
            else if (fdp.ConsumeBool()) {
                strm.zalloc = Z_NULL;
                strm.zfree = Z_NULL;
            }
            // Option 3: Set strm->strm pointer in state to NULL (corrupt internal state)
            else {
                // We can't access state->strm directly without including inflate.h
                // So we'll just set multiple fields to NULL
                strm.state = Z_NULL;
                strm.zalloc = Z_NULL;
                strm.zfree = Z_NULL;
            }
            break;
            
        case 3:
            // Test mixed invalid conditions
            strm.next_in = Z_NULL;
            strm.avail_in = (uInt)test_data_size;
            strm.next_out = Z_NULL;
            strm.avail_out = (uInt)output_size;
            break;
    }
    
    // 3. Call inflate to trigger Z_STREAM_ERROR
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // 4. Validate error return code
    // inflate should return Z_STREAM_ERROR for invalid stream states
    // Note: We don't assert specific return code as different invalid states
    // may produce different error codes
    
    // 5. Call inflateReset to reuse stream for next test case (if stream is still valid)
    // Only attempt reset if stream state might still be valid
    if (error_type % 4 != 2) {  // Don't reset if we corrupted state structure
        inflateReset(&strm);
        
        // Test with valid parameters after reset
        if (test_data_size > 0 && output_size > 0) {
            // Set up valid stream state
            strm.next_in = test_data.data();
            strm.avail_in = (uInt)test_data_size;
            strm.next_out = output_buffer;
            strm.avail_out = (uInt)output_size;
            
            // Try inflate again (may succeed or fail depending on test_data)
            inflate(&strm, Z_NO_FLUSH);
        }
    }
    
    // 6. Call inflateEnd for cleanup
    inflateEnd(&strm);
    
    free(output_buffer);
    return 0;
}

} // namespace harness_016
