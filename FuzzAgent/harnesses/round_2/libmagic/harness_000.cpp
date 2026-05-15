#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_000 {
/*
 * libmagic fuzzing harness - Comprehensive API testing
 * Target: Core libmagic functions for file type detection
 * 
 * This harness tests multiple libmagic APIs to maximize coverage:
 * - magic_open(), magic_close() - Magic cookie management
 * - magic_buffer() - Content detection
 * - magic_setflags(), magic_getflags() - Flag manipulation
 * - magic_error(), magic_errno() - Error handling
 * 
 * Based on existing libmagic fuzzer pattern from magic_fuzzer.c
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        // Too small for meaningful testing
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Initialize magic cookie with flags derived from fuzzer input
    // Mask flags to valid MAGIC_* values
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Try to load magic database from standard location
    // If it fails, we can still test other APIs
    const char* magic_path = "/usr/share/misc/magic.mgc";
    magic_load(magic, magic_path); // Ignore failure
    
    // Get remaining data for buffer testing
    std::vector<uint8_t> buffer_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test magic_buffer() - buffer-based detection
            if (!buffer_data.empty()) {
                const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                // result can be NULL or a string, both are valid
                (void)result; // Use result to avoid unused variable warning
            }
            break;
            
        case 1:
            // Test flag manipulation
            {
                int current_flags = magic_getflags(magic);
                // Set new flags based on fuzzer input (masked)
                int new_flags = flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | 
                                        MAGIC_DEVICES | MAGIC_MIME_TYPE | MAGIC_CONTINUE);
                magic_setflags(magic, new_flags);
                
                // Test buffer detection with new flags
                if (!buffer_data.empty()) {
                    const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                    (void)result;
                }
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 2:
            // Test error handling APIs
            {
                // Test with invalid buffer (NULL pointer, zero size)
                const char* result = magic_buffer(magic, NULL, 0);
                (void)result;
                
                // Get error information
                const char* error_msg = magic_error(magic);
                (void)error_msg;
                
                int err = magic_errno(magic);
                (void)err;
            }
            break;
            
        case 3:
            // Test with various buffer sizes
            {
                if (!buffer_data.empty()) {
                    // Test with partial buffer
                    size_t partial_size = buffer_data.size() / 2;
                    if (partial_size > 0) {
                        const char* result = magic_buffer(magic, buffer_data.data(), partial_size);
                        (void)result;
                    }
                    
                    // Test with full buffer
                    const char* result2 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                    (void)result2;
                }
            }
            break;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_000
