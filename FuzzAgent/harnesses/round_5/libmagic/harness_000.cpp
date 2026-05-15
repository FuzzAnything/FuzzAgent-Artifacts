#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "magic.h"

namespace harness_000 {
/*
 * Fuzzing harness for libmagic file type detection library
 * Targets core API functions: magic_open, magic_load, magic_file, magic_buffer, magic_descriptor
 * Uses FuzzedDataProvider for structured input consumption
 */



// Maximum size for temporary file operations
#define MAX_TEMP_FILE_SIZE 1024 * 1024  // 1MB

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume flags from input
    int flags = fdp.ConsumeIntegral<int>();
    
    // Limit flags to valid combinations
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | MAGIC_PRESERVE_ATIME |
              MAGIC_RAW | MAGIC_ERROR | MAGIC_MIME_ENCODING | MAGIC_APPLE |
              MAGIC_EXTENSION | MAGIC_COMPRESS_TRANSP | MAGIC_NO_COMPRESS_FORK);

    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        return 0;  // Failed to open magic handle
    }

    // Try to load default magic database
    int load_result = magic_load(magic, nullptr);
    if (load_result == -1) {
        // If loading default fails, try to load from common paths
        const char* magic_paths[] = {"/etc/magic", "/usr/share/misc/magic"};
        for (const char* path : magic_paths) {
            load_result = magic_load(magic, path);
            if (load_result != -1) {
                break;
            }
        }
    }

    // If we still couldn't load magic database, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }

    // Consume operation type from input (0: buffer, 1: file, 2: descriptor)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;

    // Get remaining bytes for testing
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes == 0) {
        magic_close(magic);
        return 0;
    }

    // Consume test data
    std::vector<uint8_t> test_data = fdp.ConsumeRemainingBytes<uint8_t>();

    switch (operation) {
        case 0: {
            // Test magic_buffer
            const char* result = magic_buffer(magic, test_data.data(), test_data.size());
            // Result can be nullptr on error, which is fine for fuzzing
            (void)result;
            break;
        }
        
        case 1: {
            // Test magic_file - create temporary file with test data
            char temp_filename[] = "/tmp/libmagic_fuzz_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write test data to temp file
                if (write(fd, test_data.data(), test_data.size()) == (ssize_t)test_data.size()) {
                    fsync(fd);
                    close(fd);
                    
                    // Use magic_file to identify the file
                    const char* result = magic_file(magic, temp_filename);
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
            // Test magic_descriptor - create temporary file and get descriptor
            char temp_filename[] = "/tmp/libmagic_fuzz_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write test data to temp file
                if (write(fd, test_data.data(), test_data.size()) == (ssize_t)test_data.size()) {
                    fsync(fd);
                    
                    // Use magic_descriptor to identify the file
                    const char* result = magic_descriptor(magic, fd);
                    (void)result;
                    
                    // Clean up
                    close(fd);
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            }
            break;
        }
    }

    // Test additional API functions with small probability
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        // Test magic_getflags and magic_setflags
        int current_flags = magic_getflags(magic);
        (void)current_flags;
        
        // Try to set some random flags
        int new_flags = fdp.ConsumeIntegral<int>();
        new_flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
                     MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK);
        magic_setflags(magic, new_flags);
        
        // Test magic_error to check if there were any errors
        const char* error_msg = magic_error(magic);
        (void)error_msg;
    }

    // Clean up
    magic_close(magic);
    
    return 0;
}

// Optional: Initialize function for shared state
extern "C" int LLVMFuzzerInitialize(int *argc, char ***argv) {
    // Nothing to initialize for this harness
    (void)argc;
    (void)argv;
    return 0;
}

} // namespace harness_000
