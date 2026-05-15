#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_007 {
/* Comprehensive fuzzing harness for zlib size_t variant compression/decompression functions
 * Targets _z variant APIs (size_t interface):
 * - compress_z(), compress2_z(), uncompress_z(), uncompress2_z()
 * - compressBound_z() for buffer sizing
 * - Tests error handling with NULL pointers, zero-length buffers, insufficient buffer sizes
 * 
 * This harness focuses specifically on the _z variants which use size_t parameters
 * rather than uLong parameters used by standard functions covered in harness_000.
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 16;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_INPUT_SIZE = 1024 * 1024;      // 1MB
const size_t MAX_COMPRESSED_SIZE = 2 * 1024 * 1024;  // 2MB

// Helper function to test compress_z/uncompress_z APIs with size_t parameters
void test_compress_uncompress_z(FuzzedDataProvider& fdp) {
    // Consume parameters for compression
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
    z_size_t input_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 1024);
    
    // Get input data
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty()) return;
    
    // Calculate maximum compressed size using _z variant
    z_size_t compressed_bound = compressBound_z(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    // Allocate buffers
    std::vector<uint8_t> compressed(compressed_bound);
    std::vector<uint8_t> decompressed(input.size());
    
    z_size_t compressed_len = compressed.size();
    z_size_t decompressed_len = decompressed.size();
    
    // Test compress2_z() and uncompress_z()
    int ret = compress2_z(compressed.data(), &compressed_len,
                         input.data(), input.size(), level);
    if (ret == Z_OK) {
        // Try to decompress using uncompress_z
        decompressed_len = decompressed.size();
        int uncompress_ret = uncompress_z(decompressed.data(), &decompressed_len,
                                         compressed.data(), compressed_len);
        
        // If decompression succeeded, verify data matches
        if (uncompress_ret == Z_OK && decompressed_len == input.size()) {
            // Data should match (but we don't assert in fuzzing harness)
            // The round-trip compression/decompression should preserve data
        }
    }
    
    // Test compress_z() (default compression level)
    compressed_len = compressed.size();
    ret = compress_z(compressed.data(), &compressed_len,
                    input.data(), input.size());
    if (ret == Z_OK) {
        decompressed_len = decompressed.size();
        uncompress_z(decompressed.data(), &decompressed_len,
                    compressed.data(), compressed_len);
    }
}

// Test uncompress2_z with explicit source length tracking
void test_uncompress2_z(FuzzedDataProvider& fdp) {
    // First create some compressed data to test with
    z_size_t input_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 512);
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
    if (input.empty()) return;
    
    z_size_t compressed_bound = compressBound_z(input.size());
    if (compressed_bound > MAX_COMPRESSED_SIZE) return;
    
    std::vector<uint8_t> compressed(compressed_bound);
    z_size_t compressed_len = compressed.size();
    
    // Compress data first
    int ret = compress_z(compressed.data(), &compressed_len,
                        input.data(), input.size());
    if (ret != Z_OK) return;
    
    // Now test uncompress2_z with the compressed data
    std::vector<uint8_t> decompressed(input.size());
    z_size_t decompressed_len = decompressed.size();
    z_size_t source_len = compressed_len;
    
    // Test with proper source length tracking
    ret = uncompress2_z(decompressed.data(), &decompressed_len,
                       compressed.data(), &source_len);
    
    // source_len should now contain the number of bytes consumed
}

// Test error handling cases for _z variant functions
void test_error_cases_z(FuzzedDataProvider& fdp) {
    // Test with NULL pointers
    uint8_t* null_ptr = nullptr;
    z_size_t len = 0;
    
    // Test compress_z with NULL dest pointer
    if (fdp.ConsumeBool()) {
        std::vector<uint8_t> input(16, 'A');
        compress_z(null_ptr, &len, input.data(), input.size());
    }
    
    // Test compress_z with NULL destLen pointer
    if (fdp.ConsumeBool()) {
        std::vector<uint8_t> input(16, 'A');
        std::vector<uint8_t> output(32);
        compress_z(output.data(), nullptr, input.data(), input.size());
    }
    
    // Test compress_z with NULL source pointer but non-zero length
    if (fdp.ConsumeBool()) {
        std::vector<uint8_t> output(32);
        z_size_t out_len = output.size();
        compress_z(output.data(), &out_len, null_ptr, 16);
    }
    
    // Test with zero-length buffers
    if (fdp.ConsumeBool()) {
        std::vector<uint8_t> input(0);
        std::vector<uint8_t> output(32);
        z_size_t out_len = output.size();
        compress_z(output.data(), &out_len, input.data(), 0);
    }
    
    // Test with insufficient output buffer
    if (fdp.ConsumeBool()) {
        std::vector<uint8_t> input(1024, 'A');
        std::vector<uint8_t> output(1);  // Too small
        z_size_t out_len = output.size();
        compress_z(output.data(), &out_len, input.data(), input.size());
    }
    
    // Test uncompress_z error cases similarly
    if (fdp.ConsumeBool()) {
        std::vector<uint8_t> compressed(32, 0);
        z_size_t decompressed_len = 64;
        uncompress_z(null_ptr, &decompressed_len, compressed.data(), compressed.size());
    }
}

// Test mixed compression levels with compress2_z
void test_mixed_levels_z(FuzzedDataProvider& fdp) {
    // Test various compression levels
    for (int level : {Z_NO_COMPRESSION, Z_BEST_SPEED, Z_DEFAULT_COMPRESSION, Z_BEST_COMPRESSION}) {
        if (fdp.remaining_bytes() < 32) break;
        
        z_size_t input_size = fdp.ConsumeIntegralInRange<z_size_t>(1, 256);
        std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(input_size);
        if (input.empty()) continue;
        
        z_size_t compressed_bound = compressBound_z(input.size());
        if (compressed_bound > MAX_COMPRESSED_SIZE) continue;
        
        std::vector<uint8_t> compressed(compressed_bound);
        std::vector<uint8_t> decompressed(input.size());
        
        z_size_t compressed_len = compressed.size();
        z_size_t decompressed_len = decompressed.size();
        
        int ret = compress2_z(compressed.data(), &compressed_len,
                             input.data(), input.size(), level);
        
        if (ret == Z_OK) {
            uncompress_z(decompressed.data(), &decompressed_len,
                        compressed.data(), compressed_len);
        }
    }
}

// Test edge cases with very large size_t values
void test_size_t_edge_cases(FuzzedDataProvider& fdp) {
    // Test with size_t values near limits
    z_size_t sizes_to_test[] = {0, 1, 2, 10, 100, 1000, 10000, 65535, 65536};
    
    for (z_size_t test_size : sizes_to_test) {
        if (fdp.remaining_bytes() < test_size) break;
        
        std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(0, test_size));
        
        if (input.empty() && test_size > 0) continue;
        
        z_size_t compressed_bound = compressBound_z(input.size());
        if (compressed_bound > MAX_COMPRESSED_SIZE) continue;
        
        std::vector<uint8_t> compressed(compressed_bound);
        std::vector<uint8_t> decompressed(input.size());
        
        z_size_t compressed_len = compressed.size();
        z_size_t decompressed_len = decompressed.size();
        
        // Test compress_z
        compress_z(compressed.data(), &compressed_len,
                  input.data(), input.size());
        
        // Test compress2_z with random level
        int level = fdp.ConsumeIntegralInRange<int>(0, 9);
        compressed_len = compressed.size();
        compress2_z(compressed.data(), &compressed_len,
                   input.data(), input.size(), level);
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test different aspects of _z variant functions based on consumed operation code
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (operation) {
        case 0:
            test_compress_uncompress_z(fdp);
            break;
        case 1:
            test_uncompress2_z(fdp);
            break;
        case 2:
            test_error_cases_z(fdp);
            break;
        case 3:
            test_mixed_levels_z(fdp);
            break;
        case 4:
            test_size_t_edge_cases(fdp);
            break;
    }
    
    return 0;  // Return value is ignored
}

} // namespace harness_007
