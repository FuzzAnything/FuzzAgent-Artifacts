#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace harness_001 {
/*
 * libmagic fuzzing harness - File-based detection and advanced API testing
 * Target: File-based detection APIs, custom magic databases, parameter configuration
 * 
 * This harness targets uncovered modules and APIs in libmagic:
 * - magic_file(), magic_descriptor() - File-based detection APIs
 * - magic_load_buffers() - Custom magic database loading
 * - magic_setparam() - Parameter configuration
 * - Targeting CDF, compression, encoding, DER, and softmagic modules
 * 
 * Semantically distinct from harness_000 (which focused on magic_buffer and flags)
 */



// Simple temporary file creation utility similar to fuzzer_temp_file.h
class TemporaryFile {
public:
    TemporaryFile(const uint8_t* data, size_t size) {
        char filename[] = "/tmp/libmagic_fuzz_XXXXXX";
        fd = mkstemp(filename);
        if (fd < 0) {
            perror("Failed to create temporary file");
            return;
        }
        
        // Store filename for cleanup
        strncpy(path, filename, sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
        
        // Write data to file
        ssize_t written = write(fd, data, size);
        if (written < 0) {
            perror("Failed to write to temporary file");
            close(fd);
            fd = -1;
            return;
        }
        
        // Close file for reading later
        close(fd);
        fd = -1;
    }
    
    ~TemporaryFile() {
        if (path[0] != '\0') {
            unlink(path);
        }
    }
    
    const char* getPath() const { return path; }
    
private:
    char path[256] = {0};
    int fd = -1;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 7);
    int param_type = fdp.ConsumeIntegralInRange<int>(0, 9); // MAGIC_PARAM_* values
    
    // Mask flags to valid MAGIC_* values
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Create a temporary file from remaining input for file-based operations
    std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
    TemporaryFile temp_file(file_data.data(), file_data.size());
    
    // Try to load standard magic database (ignore failure)
    magic_load(magic, NULL);
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test magic_file() - file-based detection
            if (temp_file.getPath()[0] != '\0') {
                const char* result = magic_file(magic, temp_file.getPath());
                (void)result;
            }
            break;
            
        case 1:
            // Test magic_descriptor() - descriptor-based detection
            if (temp_file.getPath()[0] != '\0') {
                int fd = open(temp_file.getPath(), O_RDONLY);
                if (fd >= 0) {
                    const char* result = magic_descriptor(magic, fd);
                    (void)result;
                    close(fd);
                }
            }
            break;
            
        case 2:
            // Test magic_load_buffers() - custom magic database loading
            {
                // Create a simple magic database buffer
                std::vector<uint8_t> magic_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
                if (!magic_buffer.empty()) {
                    void* buffers[1] = {magic_buffer.data()};
                    size_t sizes[1] = {magic_buffer.size()};
                    
                    // Create new magic instance for loading custom database
                    magic_t custom_magic = magic_open(MAGIC_NONE);
                    if (custom_magic) {
                        magic_load_buffers(custom_magic, buffers, sizes, 1);
                        
                        // Test detection with custom database
                        if (temp_file.getPath()[0] != '\0') {
                            const char* result = magic_file(custom_magic, temp_file.getPath());
                            (void)result;
                        }
                        
                        magic_close(custom_magic);
                    }
                }
            }
            break;
            
        case 3:
            // Test magic_setparam() - parameter configuration
            {
                size_t param_value = fdp.ConsumeIntegral<size_t>();
                magic_setparam(magic, param_type, &param_value);
                
                // Also test magic_getparam()
                size_t retrieved_value = 0;
                magic_getparam(magic, param_type, &retrieved_value);
                (void)retrieved_value;
                
                // Test file detection with modified parameters
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
            }
            break;
            
        case 4:
            // Target CDF module - clear MAGIC_NO_CHECK_CDF flag
            {
                int current_flags = magic_getflags(magic);
                // Clear the NO_CHECK_CDF flag to enable CDF checking
                int new_flags = current_flags & ~MAGIC_NO_CHECK_CDF;
                magic_setflags(magic, new_flags);
                
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 5:
            // Target compression module - enable MAGIC_COMPRESS flag
            {
                int current_flags = magic_getflags(magic);
                // Enable compression checking
                int new_flags = current_flags | MAGIC_COMPRESS;
                magic_setflags(magic, new_flags);
                
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 6:
            // Target encoding module - clear MAGIC_NO_CHECK_ENCODING flag
            {
                int current_flags = magic_getflags(magic);
                // Clear the NO_CHECK_ENCODING flag to enable encoding checking
                int new_flags = current_flags & ~MAGIC_NO_CHECK_ENCODING;
                magic_setflags(magic, new_flags);
                
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 7:
            // Target softmagic module - clear MAGIC_NO_CHECK_SOFT flag
            {
                int current_flags = magic_getflags(magic);
                // Clear the NO_CHECK_SOFT flag to enable softmagic checking
                int new_flags = current_flags & ~MAGIC_NO_CHECK_SOFT;
                magic_setflags(magic, new_flags);
                
                if (temp_file.getPath()[0] != '\0') {
                    const char* result = magic_file(magic, temp_file.getPath());
                    (void)result;
                }
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_001
