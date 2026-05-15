#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "magic.h"

namespace harness_002 {
/*
 * Fuzzing harness for libmagic database operations
 * Targets: magic_compile, magic_check, magic_list, magic_load_buffers
 * Uses FuzzedDataProvider for structured input consumption
 * Differentiates from previous harnesses by focusing on database compilation,
 * validation, and listing operations
 */



// Maximum size for magic database buffers
#define MAX_MAGIC_BUFFER_SIZE 1024 * 1024  // 1MB
#define MAX_BUFFER_COUNT 10

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < sizeof(int) + 1) {
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

    // Consume operation type from input (0: compile/check/list, 1: load_buffers)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 2;

    // Test database operations
    if (operation == 0) {
        // Path 1: Traditional database operations (compile, check, list)
        
        // First try to load default magic database
        int load_result = magic_load(magic, nullptr);
        
        // If loading default fails, try to load from common paths
        if (load_result == -1) {
            const char* magic_paths[] = {"/etc/magic", "/usr/share/misc/magic"};
            for (const char* path : magic_paths) {
                load_result = magic_load(magic, path);
                if (load_result != -1) {
                    break;
                }
            }
        }

        // If we still couldn't load magic database, try alternative path
        if (load_result == -1) {
            // Create a minimal magic database from fuzzer input for testing
            if (fdp.remaining_bytes() > 0) {
                std::string magic_data = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
                
                // Create temporary file with magic data
                char temp_filename[] = "/tmp/libmagic_db_fuzz_XXXXXX";
                int fd = mkstemp(temp_filename);
                if (fd >= 0) {
                    // Write magic data to temp file
                    if (write(fd, magic_data.data(), magic_data.size()) == (ssize_t)magic_data.size()) {
                        fsync(fd);
                        close(fd);
                        
                        // Try to load from temp file
                        load_result = magic_load(magic, temp_filename);
                        
                        // Clean up temp file
                        unlink(temp_filename);
                    } else {
                        close(fd);
                        unlink(temp_filename);
                    }
                }
            }
        }

        // If database is loaded successfully, test compilation, checking, and listing
        if (load_result != -1) {
            // Test magic_compile
            int compile_result = magic_compile(magic, nullptr);
            (void)compile_result;  // Use result to avoid unused variable warning
            
            // Test magic_check
            int check_result = magic_check(magic, nullptr);
            (void)check_result;
            
            // Test magic_list
            int list_result = magic_list(magic, nullptr);
            (void)list_result;
            
            // Also test with specific magic file paths
            if (fdp.remaining_bytes() > 10) {
                std::string custom_path = fdp.ConsumeRandomLengthString(256);
                magic_compile(magic, custom_path.c_str());
                magic_check(magic, custom_path.c_str());
                magic_list(magic, custom_path.c_str());
            }
        }
    } else {
        // Path 2: Test magic_load_buffers with compiled buffers
        
        // Determine number of buffers to create
        size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(1, MAX_BUFFER_COUNT);
        
        // Create arrays for buffers and sizes
        void** buffers = new void*[num_buffers]();  // Value-initialized to nullptr
        size_t* sizes = new size_t[num_buffers];
        // Fill buffers with fuzzer data
        bool buffers_created = true;
        for (size_t i = 0; i < num_buffers && fdp.remaining_bytes() > 0; i++) {
            // Consume buffer size
            sizes[i] = fdp.ConsumeIntegralInRange<size_t>(1, MAX_MAGIC_BUFFER_SIZE);
            
            // Limit size to remaining bytes
            if (sizes[i] > fdp.remaining_bytes()) {
                sizes[i] = fdp.remaining_bytes();
            }
            
            if (sizes[i] > 0) {
                // Allocate and fill buffer
                buffers[i] = new uint8_t[sizes[i]];
                std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(sizes[i]);
                memcpy(buffers[i], buffer_data.data(), sizes[i]);
            } else {
                buffers[i] = nullptr;
                buffers_created = false;
                break;
            }
        }
        
        // Test magic_load_buffers if we created valid buffers
        if (buffers_created) {
            int load_buffers_result = magic_load_buffers(magic, buffers, sizes, num_buffers);
            (void)load_buffers_result;
            
            // If buffers loaded successfully, test other database operations
            if (load_buffers_result != -1 && fdp.remaining_bytes() > 0) {
                // Test compilation with loaded buffers
                magic_compile(magic, nullptr);
                
                // Test checking with loaded buffers
                magic_check(magic, nullptr);
                
                // Test listing with loaded buffers
                magic_list(magic, nullptr);
            }
        }
        
        // Clean up buffers
        for (size_t i = 0; i < num_buffers; i++) {
            if (buffers[i] != nullptr) {
                delete[] static_cast<uint8_t*>(buffers[i]);
            }
        }
        delete[] buffers;
        delete[] sizes;
    }

    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_002
