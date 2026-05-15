#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "magic.h"

namespace harness_006 {
/*
 * Fuzzing harness for libmagic compression module functions
 * This harness specifically targets compression detection and decompression functions
 * that have 0% coverage according to previous analysis.
 * 
 * Target functions: uncompresszlib, uncompressbzlib, uncompressxzlib, uncompresszstd
 * 
 * Testing strategy:
 * 1. Generate test buffers with various compression formats (zlib, bz2, xz, zstd)
 * 2. Include edge cases: corrupted compressed data, partial compression headers
 * 3. Test mixed compression formats to exercise detection paths
 * 4. Use MAGIC_COMPRESS flag to enable compression detection
 * 
 * Follow sequence: magic_open -> load magic database -> magic_buffer with test data -> magic_close
 * 
 * Based on analysis of compress.c which shows internal uncompress* functions for
 * different compression formats.
 */


// Include libmagic headers

// Helper function to create test buffers with compression-like patterns
std::vector<uint8_t> create_compression_test_buffer(FuzzedDataProvider& fdp, size_t max_size) {
    std::vector<uint8_t> buffer;
    
    // Determine which compression format to simulate
    uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Create buffer with compression header-like patterns
    switch (format_type) {
        case 0: // Simulate zlib/gzip header
            if (max_size >= 10) {
                // zlib header: 0x78 0x9C (default compression)
                buffer.push_back(0x78);
                buffer.push_back(0x9C);
                // Add some compressed-like data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size - 2);
                auto data = fdp.ConsumeBytes<uint8_t>(data_size);
                buffer.insert(buffer.end(), data.begin(), data.end());
            }
            break;
            
        case 1: // Simulate bzip2 header
            if (max_size >= 10) {
                // bzip2 header: 'B' 'Z' 'h' (for huffman coding)
                buffer.push_back('B');
                buffer.push_back('Z');
                buffer.push_back('h');
                // Version number and block size
                buffer.push_back('1'); // Version 1
                uint8_t block_size = fdp.ConsumeIntegralInRange<uint8_t>('1', '9');
                buffer.push_back(block_size);
                // Add some compressed-like data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size - 5);
                auto data = fdp.ConsumeBytes<uint8_t>(data_size);
                buffer.insert(buffer.end(), data.begin(), data.end());
            }
            break;
            
        case 2: // Simulate xz/lzma header
            if (max_size >= 12) {
                // xz header: 0xFD '7' 'z' 'X' 'Z' 0x00
                buffer.push_back(0xFD);
                buffer.push_back('7');
                buffer.push_back('z');
                buffer.push_back('X');
                buffer.push_back('Z');
                buffer.push_back(0x00);
                // Stream flags and CRC
                buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                // Add some compressed-like data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size - 8);
                auto data = fdp.ConsumeBytes<uint8_t>(data_size);
                buffer.insert(buffer.end(), data.begin(), data.end());
            }
            break;
            
        case 3: // Simulate zstd header
            if (max_size >= 7) {
                // zstd magic number: 0x28 0xB5 0x2F 0xFD
                buffer.push_back(0x28);
                buffer.push_back(0xB5);
                buffer.push_back(0x2F);
                buffer.push_back(0xFD);
                // Frame header descriptor
                buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                // Add some compressed-like data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size - 5);
                auto data = fdp.ConsumeBytes<uint8_t>(data_size);
                buffer.insert(buffer.end(), data.begin(), data.end());
            }
            break;
            
        case 4: // Corrupted compression header (partial)
            if (max_size >= 3) {
                // Start with partial header
                buffer.push_back(0x78); // zlib first byte
                // Maybe add second byte, maybe not
                if (fdp.ConsumeBool() && max_size >= 4) {
                    buffer.push_back(0x9C);
                }
                // Add random data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size - buffer.size());
                auto data = fdp.ConsumeBytes<uint8_t>(data_size);
                buffer.insert(buffer.end(), data.begin(), data.end());
            }
            break;
            
        case 5: // Mixed/confused headers
            if (max_size >= 10) {
                // Mix headers from different formats
                buffer.push_back(0x78); // zlib
                buffer.push_back('B');  // bzip2
                buffer.push_back('Z');
                buffer.push_back(0xFD); // xz
                // Add random data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size - 4);
                auto data = fdp.ConsumeBytes<uint8_t>(data_size);
                buffer.insert(buffer.end(), data.begin(), data.end());
            }
            break;
            
        case 6: // Valid header but corrupted data
            if (max_size >= 10) {
                // Valid zlib header
                buffer.push_back(0x78);
                buffer.push_back(0x9C);
                // Add corrupted/invalid compressed data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(10, max_size - 2);
                auto data = fdp.ConsumeBytes<uint8_t>(data_size);
                buffer.insert(buffer.end(), data.begin(), data.end());
                // Corrupt some bytes in the middle
                if (buffer.size() > 20) {
                    size_t corrupt_pos = fdp.ConsumeIntegralInRange<size_t>(10, buffer.size() - 10);
                    buffer[corrupt_pos] = fdp.ConsumeIntegral<uint8_t>();
                }
            }
            break;
            
        case 7: // No compression header, just random data
        default:
            // Just use random bytes to test detection of non-compressed data
            size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size);
            auto data = fdp.ConsumeBytes<uint8_t>(data_size);
            buffer.insert(buffer.end(), data.begin(), data.end());
            break;
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: magic_open with compression flags enabled
    int flags = MAGIC_NONE;
    
    // Always enable compression detection
    flags |= MAGIC_COMPRESS;
    
    // Randomly enable other flags that might affect compression handling
    if (fdp.ConsumeBool()) flags |= MAGIC_DEBUG;
    if (fdp.ConsumeBool()) flags |= MAGIC_CHECK;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    
    // Try to disable compression checking sometimes to test both paths
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_COMPRESS;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // If magic_open fails with complex flags, try with basic flags
        magic = magic_open(MAGIC_NONE | MAGIC_COMPRESS);
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // Test 2: Try to load magic database
    // Use NULL to load default database, which should include compression detection
    if (magic_load(magic, NULL) == -1) {
        // If default load fails, continue anyway - some APIs may still work
        // Could also be due to missing default magic file
    }
    
    // Test 3: Create multiple test buffers with compression patterns
    // and test magic_buffer on each
    
    // Determine how many test cases to run
    size_t num_tests = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_tests && fdp.remaining_bytes() > 16; i++) {
        // Determine buffer size for this test
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        if (buffer_size > fdp.remaining_bytes()) {
            buffer_size = fdp.remaining_bytes();
        }
        
        if (buffer_size == 0) {
            break;
        }
        
        // Create test buffer with compression patterns
        auto test_buffer = create_compression_test_buffer(fdp, buffer_size);
        
        if (test_buffer.empty()) {
            // Skip empty buffers
            continue;
        }
        
        // Test magic_buffer with the compression test buffer
        const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        
        // Result might be NULL for unrecognized formats or errors
        // That's fine - we're testing error paths too
        (void)result;
        
        // Also test with a slice of the buffer to exercise partial data handling
        if (test_buffer.size() > 10) {
            size_t slice_size = fdp.ConsumeIntegralInRange<size_t>(1, test_buffer.size());
            const char* slice_result = magic_buffer(magic, test_buffer.data(), slice_size);
            (void)slice_result;
        }
        
        // Test with different flags on the same buffer
        // Try changing flags dynamically
        int current_flags = magic_getflags(magic);
        (void)current_flags;
        
        // Try toggling compression flag
        if (fdp.ConsumeBool()) {
            int new_flags = current_flags;
            if (fdp.ConsumeBool()) {
                new_flags |= MAGIC_COMPRESS;
            } else {
                new_flags &= ~MAGIC_COMPRESS;
            }
            magic_setflags(magic, new_flags);
            
            // Test again with new flags
            const char* result2 = magic_buffer(magic, test_buffer.data(), test_buffer.size());
            (void)result2;
            
            // Restore original flags
            magic_setflags(magic, current_flags);
        }
    }
    
    // Test 4: Additional edge cases
    
    // Test with very small buffers (partial headers)
    if (fdp.remaining_bytes() > 0) {
        size_t small_size = fdp.ConsumeIntegralInRange<size_t>(1, 10);
        if (small_size <= fdp.remaining_bytes()) {
            auto small_buffer = fdp.ConsumeBytes<uint8_t>(small_size);
            const char* small_result = magic_buffer(magic, small_buffer.data(), small_buffer.size());
            (void)small_result;
        }
    }
    
    // Test with empty buffer
    if (fdp.ConsumeBool()) {
        const char* empty_result = magic_buffer(magic, nullptr, 0);
        (void)empty_result;
    }
    
    // Test 5: Test magic_file with compression if we can create a temp file
    // (Skipping file I/O for simplicity in fuzzing harness)
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_006
