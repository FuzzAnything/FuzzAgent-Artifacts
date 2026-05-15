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

namespace harness_000 {
/*
 * Fuzzing harness for libmagic library
 * Targets: magic_open, magic_load, magic_buffer, magic_file, magic_getflags, magic_setflags
 * Uses FuzzedDataProvider to split input for different operations and parameters
 */

// Include libmagic header - using the path from the existing fuzzer

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
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
    
    // Consume operation type: 0 = magic_buffer, 1 = magic_file (with temp file)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (operation) {
        case 0: {
            // magic_buffer operation
            // Consume remaining bytes as buffer
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
            std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            if (!buffer.empty()) {
                const char* result = magic_buffer(magic, buffer.data(), buffer.size());
                // Result can be NULL for unknown types, that's OK
                (void)result; // Avoid unused variable warning
            }
            break;
        }
        
        case 1: {
            // magic_file operation with temporary file
            // Create a temporary file
            char temp_filename[] = "/tmp/libmagic_fuzz_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write some data to the temp file
                size_t file_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
                std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
                
                if (write(fd, file_data.data(), file_data.size()) == (ssize_t)file_data.size()) {
                    close(fd);
                    
                    const char* result = magic_file(magic, temp_filename);
                    // Result can be NULL for unknown types, that's OK
                    (void)result;
                    
                    // Clean up temp file
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            }
            break;
        }
        
        case 2: {
            // Test flag manipulation
            int current_flags = magic_getflags(magic);
            
            // Toggle some flags based on consumed data
            uint8_t toggle_flags = fdp.ConsumeIntegral<uint8_t>();
            int new_flags = current_flags;
            
            if (toggle_flags & 0x01) new_flags ^= MAGIC_SYMLINK;
            if (toggle_flags & 0x02) new_flags ^= MAGIC_COMPRESS;
            if (toggle_flags & 0x04) new_flags ^= MAGIC_MIME_TYPE;
            if (toggle_flags & 0x08) new_flags ^= MAGIC_CONTINUE;
            
            magic_setflags(magic, new_flags);
            
            // Test buffer with new flags
            if (fdp.remaining_bytes() > 0) {
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                if (!buffer.empty()) {
                    const char* result = magic_buffer(magic, buffer.data(), buffer.size());
                    (void)result;
                }
            }
            break;
        }
    }
    
    // Get error message (if any) - exercise magic_error API
    const char* error_msg = magic_error(magic);
    (void)error_msg;
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_000
