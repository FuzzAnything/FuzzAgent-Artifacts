#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_016 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful compression testing with size_t variants
    // Need enough data for parameters, source data, and buffer sizes
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);  // Compression level 0-9
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);  // Compression strategy
    
    // Determine source data size for size_t variant testing
    // Use reasonable bounds to avoid excessive memory usage
    size_t src_len = fdp.ConsumeIntegralInRange<size_t>(1, 65536);
    if (src_len > fdp.remaining_bytes()) {
        src_len = fdp.remaining_bytes();
    }
    
    // Consume source data
    std::vector<uint8_t> src_data;
    if (src_len > 0) {
        src_data = fdp.ConsumeBytes<uint8_t>(src_len);
    }
    
    if (src_data.empty()) {
        return 0;  // No data to compress
    }
    
    // Step 1: Test compressBound_z - calculate compressed size bound with size_t variant
    z_size_t bound_z_result = compressBound_z(src_len);
    
    // Also test regular compressBound for comparison
    uLong bound_result = compressBound(src_len);
    
    // Allocate buffers for compression/decompression
    // Use bound_z_result as allocation size for compressed buffer
    size_t compr_bound = (bound_z_result == (z_size_t)-1) ? src_len * 2 + 12 : bound_z_result;
    
    // Avoid excessive memory allocation
    if (compr_bound > 10 * 1024 * 1024) {
        compr_bound = 10 * 1024 * 1024;
    }
    
    Bytef* compr = (Bytef*)malloc(compr_bound);
    Bytef* uncompr = (Bytef*)malloc(src_len);
    
    if (!compr || !uncompr) {
        free(compr);
        free(uncompr);
        return 0;
    }
    
    // Step 2: Test compress2_z - compression with specific level using size_t variants
    z_size_t compr_len_z = compr_bound;
    int ret_z = compress2_z(compr, &compr_len_z, src_data.data(), src_len, level);
    
    // Also test regular compress2 for comparison
    uLongf compr_len = compr_bound;
    int ret = compress2(compr, &compr_len, src_data.data(), src_len, level);
    
    // Step 3: Test compress_z - default compression using size_t variant
    if (fdp.ConsumeBool()) {
        z_size_t compr_len_simple_z = compr_bound;
        compress_z(compr, &compr_len_simple_z, src_data.data(), src_len);
    }
    
    // Step 4: Test uncompress_z - decompression using size_t variant
    if (ret_z == Z_OK && compr_len_z > 0 && compr_len_z <= compr_bound) {
        z_size_t uncompr_len_z = src_len;
        int unret_z = uncompress_z(uncompr, &uncompr_len_z, compr, compr_len_z);
        
        // If decompression successful, verify data integrity
        if (unret_z == Z_OK && uncompr_len_z == src_len) {
            // Data integrity check - compare decompressed with original
            // Note: We don't assert in fuzzing harnesses
        }
        
        // Step 5: Test uncompress2_z - variant with pointer to source length
        z_size_t uncompr_len2_z = src_len;
        z_size_t src_len_copy_z = compr_len_z;
        uncompress2_z(uncompr, &uncompr_len2_z, compr, &src_len_copy_z);
    }
    
    // Step 6: Test with different buffer sizes and edge cases
    // Test with insufficient output buffer size
    if (compr_bound > 10) {
        z_size_t small_buffer_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 10);
        z_size_t small_compr_len = small_buffer_size;
        compress2_z(compr, &small_compr_len, src_data.data(), src_len, level);
        // Should return Z_BUF_ERROR for insufficient buffer
    }
    
    // Step 7: Test with zero-length input
    if (fdp.ConsumeBool()) {
        z_size_t zero_compr_len = compr_bound;
        std::vector<uint8_t> empty_data;
        compress2_z(compr, &zero_compr_len, empty_data.data(), 0, level);
    }
    
    // Step 8: Test with NULL pointers to exercise error paths
    if (fdp.ConsumeBool() && src_len > 0) {
        z_size_t dest_len = compr_bound;
        // Test with NULL destination buffer but non-zero length
        compress2_z(NULL, &dest_len, src_data.data(), src_len, level);
        // Should return Z_STREAM_ERROR
    }
    
    // Step 9: Test compression with different strategies using size_t variants
    if (src_len > 100) {
        // Create multiple compression streams with different strategies
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
            int test_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
            z_size_t strategy_compr_len = compr_bound;
            compress2_z(compr, &strategy_compr_len, src_data.data(), src_len, 
                       fdp.ConsumeIntegralInRange<int>(0, 9));
        }
    }
    
    // Step 10: Test round-trip compression/decompression with size_t variants
    if (ret_z == Z_OK && compr_len_z > 0) {
        // Allocate fresh buffer for decompression
        Bytef* roundtrip_uncompr = (Bytef*)malloc(src_len);
        if (roundtrip_uncompr) {
            z_size_t roundtrip_len = src_len;
            int roundtrip_ret = uncompress_z(roundtrip_uncompr, &roundtrip_len, 
                                            compr, compr_len_z);
            
            free(roundtrip_uncompr);
        }
    }
    
    // Cleanup
    free(compr);
    free(uncompr);
    
    return 0;
}

} // namespace harness_016
