#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "magic.h"

namespace harness_003 {
/*
 * Fuzzing harness for libmagic library - Compression Detection and Processing
 * Targets: Compression handling functions via magic_buffer with MAGIC_COMPRESS flag
 * Specifically exercises: uncompressgzipped, uncompresszlib, uncompresszstd, uncompressxzlib
 * These functions have 0% coverage with 72 blocked branches (per coverage analysis)
 * Focuses on compressed data processing for gzip, zlib, zstd, and xz formats
 * Uses FuzzedDataProvider to create various compressed/malformed test cases
 */


// Include libmagic header - using the path from the existing fuzzer

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - compressed data needs some meaningful size
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - MUST include MAGIC_COMPRESS for compression detection
    int flags = MAGIC_COMPRESS; // Essential for compression handling
    
    // Consume additional flags from input
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Try to load magic database - use default path
    const char* magic_path = NULL; // NULL means use default
    int load_result = magic_load(magic, magic_path);
    
    // If loading fails, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Consume operation type to test different compression scenarios
    // 0 = raw compressed data, 1 = truncated data, 2 = nested compression simulation,
    // 3 = malformed headers, 4 = mixed compressed data
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Consume compression format selector to target specific decompressors
    // This helps guide the fuzzer to explore different compression code paths
    uint8_t format_hint = fdp.ConsumeIntegral<uint8_t>() % 4; // 0=gzip, 1=zlib, 2=zstd, 3=xz
    
    switch (operation) {
        case 0: {
            // Test 1: Raw compressed data
            // Use all remaining bytes as compressed buffer
            std::vector<uint8_t> compressed_data = fdp.ConsumeRemainingBytes<uint8_t>();
            
            if (!compressed_data.empty()) {
                const char* result = magic_buffer(magic, compressed_data.data(), compressed_data.size());
                // Result can be NULL for unknown/unrecognized types, that's OK
                // The goal is to exercise the compression detection and decompression paths
                (void)result; // Avoid unused variable warning
            }
            break;
        }
        
        case 1: {
            // Test 2: Truncated compressed data
            // Create intentionally truncated data to test error handling paths
            size_t truncate_point = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
            if (truncate_point > 0) {
                std::vector<uint8_t> truncated_data = fdp.ConsumeBytes<uint8_t>(truncate_point);
                
                // Further truncate the data (make it intentionally too short)
                if (truncated_data.size() > 8) {
                    size_t final_size = fdp.ConsumeIntegralInRange<size_t>(1, truncated_data.size() - 1);
                    truncated_data.resize(final_size);
                }
                
                if (!truncated_data.empty()) {
                    const char* result = magic_buffer(magic, truncated_data.data(), truncated_data.size());
                    (void)result;
                }
            }
            break;
        }
        
        case 2: {
            // Test 3: Simulated nested compression
            // Create data that looks like multiple compression layers
            // This exercises deeper code paths in compression detection
            
            // First layer: consume some bytes
            size_t layer1_size = fdp.ConsumeIntegralInRange<size_t>(10, fdp.remaining_bytes());
            std::vector<uint8_t> layer1_data = fdp.ConsumeBytes<uint8_t>(layer1_size);
            
            // Create a buffer that might resemble nested compression
            std::vector<uint8_t> nested_buffer;
            
            // Add format-specific headers based on format_hint
            switch (format_hint) {
                case 0: // gzip-like
                    nested_buffer.push_back(0x1f); // gzip magic bytes
                    nested_buffer.push_back(0x8b);
                    nested_buffer.push_back(0x08); // DEFLATE compression
                    nested_buffer.push_back(0x00); // flags
                    break;
                case 1: // zlib-like  
                    nested_buffer.push_back(0x78); // zlib header
                    nested_buffer.push_back(0x9c); // default compression
                    break;
                case 2: // zstd-like
                    // zstd frame header magic number
                    nested_buffer.push_back(0x28);
                    nested_buffer.push_back(0xb5);
                    nested_buffer.push_back(0x2f);
                    nested_buffer.push_back(0xfd);
                    break;
                case 3: // xz-like
                    // xz magic bytes
                    nested_buffer.push_back(0xfd);
                    nested_buffer.push_back(0x37);
                    nested_buffer.push_back(0x7a);
                    nested_buffer.push_back(0x58);
                    nested_buffer.push_back(0x5a);
                    nested_buffer.push_back(0x00);
                    break;
            }
            
            // Append the actual data
            nested_buffer.insert(nested_buffer.end(), layer1_data.begin(), layer1_data.end());
            
            // Add some trailing bytes that might look like checksums or footers
            for (int i = 0; i < 8 && fdp.remaining_bytes() > 0; i++) {
                nested_buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            if (!nested_buffer.empty()) {
                const char* result = magic_buffer(magic, nested_buffer.data(), nested_buffer.size());
                (void)result;
            }
            break;
        }
        
        case 3: {
            // Test 4: Malformed compression headers
            // Create data with intentionally corrupted/malformed headers
            
            std::vector<uint8_t> malformed_data;
            
            // Start with valid magic bytes then corrupt them
            switch (format_hint) {
                case 0: // gzip
                    malformed_data.push_back(0x1f);
                    malformed_data.push_back(0x8b);
                    // Corrupt the rest
                    malformed_data.push_back(fdp.ConsumeIntegral<uint8_t>()); // Random method
                    malformed_data.push_back(fdp.ConsumeIntegral<uint8_t>()); // Random flags
                    break;
                case 1: // zlib
                    malformed_data.push_back(0x78);
                    // Corrupt the second byte
                    malformed_data.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0x00, 0xff));
                    break;
                case 2: // zstd
                    // Partial or corrupted zstd header
                    malformed_data.push_back(0x28);
                    malformed_data.push_back(0xb5);
                    malformed_data.push_back(fdp.ConsumeIntegral<uint8_t>()); // Corrupt
                    break;
                case 3: // xz
                    malformed_data.push_back(0xfd);
                    malformed_data.push_back(0x37);
                    malformed_data.push_back(0x7a);
                    malformed_data.push_back(fdp.ConsumeIntegral<uint8_t>()); // Corrupt
                    break;
            }
            
            // Add some random payload
            size_t payload_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
            std::vector<uint8_t> payload = fdp.ConsumeBytes<uint8_t>(payload_size);
            malformed_data.insert(malformed_data.end(), payload.begin(), payload.end());
            
            if (!malformed_data.empty()) {
                const char* result = magic_buffer(magic, malformed_data.data(), malformed_data.size());
                (void)result;
            }
            break;
        }
        
        case 4: {
            // Test 5: Mixed format testing
            // Create multiple test buffers with different compression formats in one run
            
            // Determine how many test buffers to create (1-3)
            size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            
            for (size_t i = 0; i < num_buffers && fdp.remaining_bytes() > 10; i++) {
                // For each buffer, use different format hints
                uint8_t buffer_format = (format_hint + i) % 4;
                
                // Create a test buffer
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, fdp.remaining_bytes());
                std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                // Prepend format-specific hints
                std::vector<uint8_t> test_buffer;
                
                // Add minimal header hints
                switch (buffer_format) {
                    case 0: // gzip hint
                        test_buffer.push_back(0x1f);
                        test_buffer.push_back(0x8b);
                        break;
                    case 1: // zlib hint
                        test_buffer.push_back(0x78);
                        break;
                    case 2: // zstd hint
                        test_buffer.push_back(0x28);
                        test_buffer.push_back(0xb5);
                        break;
                    case 3: // xz hint
                        test_buffer.push_back(0xfd);
                        test_buffer.push_back(0x37);
                        break;
                }
                
                test_buffer.insert(test_buffer.end(), buffer_data.begin(), buffer_data.end());
                
                if (!test_buffer.empty()) {
                    const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                    (void)result;
                }
            }
            break;
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_003
