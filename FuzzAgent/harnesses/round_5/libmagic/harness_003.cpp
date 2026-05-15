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
#include <dirent.h>
#include "magic.h"

namespace harness_003 {
/*
 * Fuzzing harness for libmagic file-based operations
 * Targets APIs with 0% coverage: magic_file, magic_descriptor, magic_error, magic_getflags
 * Uses FuzzedDataProvider for structured input consumption
 * Focuses on comprehensive file detection including filesystem objects and special files
 * Semantic differentiation from previous harnesses:
 * - harness_000: Basic buffer detection
 * - harness_001: Configuration management  
 * - harness_002: Database operations
 * - harness_003: File-based operations and filesystem detection
 */



// Maximum size for temporary file operations
#define MAX_TEMP_FILE_SIZE 1024 * 1024  // 1MB

// Helper for creating temporary files from fuzzer input
class FuzzerTemporaryFile {
public:
    FuzzerTemporaryFile(const uint8_t* data, size_t size) {
        char filename_buffer[] = "/tmp/libmagic_fuzz_XXXXXX";
        fd = mkstemp(filename_buffer);
        if (fd >= 0) {
            strncpy(filename, filename_buffer, sizeof(filename) - 1);
            filename[sizeof(filename) - 1] = '\0';
            
            if (write(fd, data, size) == (ssize_t)size) {
                fsync(fd);
                lseek(fd, 0, SEEK_SET); // Rewind for reading
            } else {
                close(fd);
                unlink(filename);
                fd = -1;
            }
        }
    }
    
    ~FuzzerTemporaryFile() {
        if (fd >= 0) {
            close(fd);
            unlink(filename);
        }
    }
    
    const char* get_filename() const { return fd >= 0 ? filename : nullptr; }
    int get_fd() const { return fd; }
    
private:
    char filename[64];
    int fd;
};

// Create a directory for testing directory detection
static bool create_test_directory(const char* dirname) {
    if (mkdir(dirname, 0755) == 0) {
        return true;
    }
    return false;
}

// Create a symlink for testing symlink detection  
static bool create_test_symlink(const char* target, const char* linkname) {
    if (symlink(target, linkname) == 0) {
        return true;
    }
    return false;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for basic operations
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume flags from input
    int flags = fdp.ConsumeIntegral<int>();
    
    // Limit flags to valid combinations for file detection
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | MAGIC_PRESERVE_ATIME |
              MAGIC_RAW | MAGIC_ERROR | MAGIC_MIME_ENCODING | MAGIC_APPLE |
              MAGIC_EXTENSION | MAGIC_COMPRESS_TRANSP | MAGIC_NO_COMPRESS_FORK);
    
    // Add symlink flag to test symlink following (important for filesystem detection)
    if (fdp.ConsumeBool()) {
        flags |= MAGIC_SYMLINK;
    }
    
    // Add devices flag to test device file detection
    if (fdp.ConsumeBool()) {
        flags |= MAGIC_DEVICES;
    }

    // Step 1: magic_open() - Initialize magic context
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        return 0;  // Failed to open magic handle
    }

    // Step 2: magic_setflags() - Configure appropriate flags for file detection
    // Use a subset of flags for setflags to test flag management
    int setflags = fdp.ConsumeIntegral<int>();
    setflags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
                MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK);
    magic_setflags(magic, setflags);

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

    // Consume operation type from input to test different file scenarios
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Get remaining bytes for test data
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes == 0) {
        magic_close(magic);
        return 0;
    }
    
    // Consume test data for file content
    std::vector<uint8_t> test_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (test_data.size() > MAX_TEMP_FILE_SIZE) {
        test_data.resize(MAX_TEMP_FILE_SIZE);
    }

    // Step 3-4: Test magic_file() and magic_descriptor() based on operation type
    switch (operation) {
        case 0: {
            // Test regular file with magic_file()
            FuzzerTemporaryFile temp_file(test_data.data(), test_data.size());
            if (temp_file.get_filename() != nullptr) {
                const char* result = magic_file(magic, temp_file.get_filename());
                (void)result;
                
                // Step 5: magic_error() - Check for errors after file operation
                const char* error_msg = magic_error(magic);
                (void)error_msg;
                
                // Step 6: magic_getflags() - Verify current flag state
                int current_flags = magic_getflags(magic);
                (void)current_flags;
            }
            break;
        }
        
        case 1: {
            // Test file descriptor with magic_descriptor()
            FuzzerTemporaryFile temp_file(test_data.data(), test_data.size());
            if (temp_file.get_fd() >= 0) {
                const char* result = magic_descriptor(magic, temp_file.get_fd());
                (void)result;
                
                const char* error_msg = magic_error(magic);
                (void)error_msg;
                
                int current_flags = magic_getflags(magic);
                (void)current_flags;
            }
            break;
        }
        
        case 2: {
            // Test both magic_file() and magic_descriptor() on same file
            FuzzerTemporaryFile temp_file(test_data.data(), test_data.size());
            if (temp_file.get_filename() != nullptr && temp_file.get_fd() >= 0) {
                // First test magic_file()
                const char* file_result = magic_file(magic, temp_file.get_filename());
                (void)file_result;
                
                const char* file_error = magic_error(magic);
                (void)file_error;
                
                // Then test magic_descriptor() on the same file
                const char* desc_result = magic_descriptor(magic, temp_file.get_fd());
                (void)desc_result;
                
                const char* desc_error = magic_error(magic);
                (void)desc_error;
                
                int current_flags = magic_getflags(magic);
                (void)current_flags;
            }
            break;
        }
        
        case 3: {
            // Test special filesystem objects (if we have enough input)
            if (test_data.size() > 100) {
                // Create a directory for testing
                char dirname[] = "/tmp/libmagic_dir_XXXXXX";
                if (mkdtemp(dirname) != nullptr) {
                    // Test directory detection with magic_file()
                    const char* dir_result = magic_file(magic, dirname);
                    (void)dir_result;
                    
                    const char* dir_error = magic_error(magic);
                    (void)dir_error;
                    
                    // Create a symlink inside the directory
                    char symlink_path[128];
                    snprintf(symlink_path, sizeof(symlink_path), "%s/symlink", dirname);
                    
                    char target_path[] = "/dev/null";
                    if (symlink(target_path, symlink_path) == 0) {
                        // Test symlink detection with magic_file()
                        const char* symlink_result = magic_file(magic, symlink_path);
                        (void)symlink_result;
                        
                        const char* symlink_error = magic_error(magic);
                        (void)symlink_error;
                        
                        unlink(symlink_path);
                    }
                    
                    // Clean up directory
                    rmdir(dirname);
                }
                
                // Also test with a regular temp file
                FuzzerTemporaryFile temp_file(test_data.data(), test_data.size());
                if (temp_file.get_filename() != nullptr) {
                    const char* result = magic_file(magic, temp_file.get_filename());
                    (void)result;
                    
                    const char* error_msg = magic_error(magic);
                    (void)error_msg;
                    
                    int current_flags = magic_getflags(magic);
                    (void)current_flags;
                }
            }
            break;
        }
    }
    
    // Additional error checking and flag verification
    const char* final_error = magic_error(magic);
    (void)final_error;
    
    int final_flags = magic_getflags(magic);
    (void)final_flags;
    
    // Optional: Test magic_version()
    if (fdp.ConsumeBool()) {
        int version = magic_version();
        (void)version;
    }

    // Step 7: magic_close() - Clean up resources
    magic_close(magic);
    
    return 0;
}


} // namespace harness_003
