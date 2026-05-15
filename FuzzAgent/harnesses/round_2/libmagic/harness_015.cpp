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
#include <algorithm>
#include <memory>

namespace harness_015 {
/*
 * libmagic fuzzing harness #15 - Final comprehensive API coverage test
 * Target: All remaining public APIs not thoroughly tested, with focus on magic_getpath
 * 
 * This harness serves as the final comprehensive API coverage test for libmagic,
 * specifically targeting APIs that may have been missed or under-tested in previous harnesses:
 * 
 * Primary focus: magic_getpath() - the only API not used in any previous harness
 * Secondary focus: Complex API sequences and edge cases for all public APIs
 * 
 * 1) Test magic_getpath() with various parameters:
 *    - NULL path, empty string, non-existent path
 *    - Valid magic database paths
 *    - Different action parameters
 * 
 * 2) Test all public APIs in complex sequences:
 *    - Multiple magic instances with different flags
 *    - Interleaved API calls with state changes
 *    - Error recovery sequences
 * 
 * 3) Test edge cases for all parameterized APIs:
 *    - Boundary values for magic_setparam/magic_getparam
 *    - Extreme flag combinations for magic_setflags
 *    - Invalid and NULL pointer inputs
 * 
 * 4) Test API interactions and state persistence:
 *    - Load/compile/check/list operations in sequence
 *    - Flag changes between detection calls
 *    - Parameter changes affecting subsequent operations
 * 
 * 5) Ensure 100% public API coverage:
 *    - magic_open(), magic_close(), magic_load(), magic_compile(), magic_check()
 *    - magic_buffer(), magic_file(), magic_descriptor(), magic_load_buffers()
 *    - magic_setflags(), magic_getflags(), magic_setparam(), magic_getparam()
 *    - magic_error(), magic_errno(), magic_list(), magic_version(), magic_getpath()
 */



// Constants for reasonable limits
const size_t MAX_BUFFER_SIZE = 65536;
const size_t MAX_MAGIC_DB_SIZE = 32768;
const int MAX_MAGIC_INSTANCES = 5;
const int MAX_API_SEQUENCE_LENGTH = 20;
class TemporaryFile {
public:
    TemporaryFile(const uint8_t* data, size_t size) {
        if (size > MAX_BUFFER_SIZE) size = MAX_BUFFER_SIZE;
        
        char filename[] = "/tmp/magic_final_XXXXXX";
        fd = mkstemp(filename);
        if (fd < 0) {
            return;
        }
        
        strncpy(path, filename, sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
        
        ssize_t written = write(fd, data, size);
        if (written < 0) {
            close(fd);
            fd = -1;
            unlink(path);
            path[0] = '\0';
            return;
        }
        
        close(fd);
        fd = -1;
    }
    
    ~TemporaryFile() {
        if (path[0] != '\0') {
            unlink(path);
        }
    }
    
    const char* getPath() const { return path[0] != '\0' ? path : NULL; }
    
private:
    char path[256] = {0};
    int fd = -1;
};

// Test magic_getpath with various parameters
void test_magic_getpath(FuzzedDataProvider& fdp) {
    // Test different path scenarios
    int action = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    const char* test_path = NULL;
    std::string path_str;
    
    int path_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (path_choice) {
        case 0:
            // NULL path
            test_path = NULL;
            break;
        case 1:
            // Empty string
            test_path = "";
            break;
        case 2:
            // Non-existent path
            path_str = fdp.ConsumeRandomLengthString(64);
            test_path = path_str.c_str();
            break;
        case 3:
            // Potential magic database path
            path_str = fdp.ConsumeRandomLengthString(128);
            // Add common magic database locations
            if (fdp.ConsumeBool()) {
                path_str = "/usr/share/misc/magic";
            } else if (fdp.ConsumeBool()) {
                path_str = "/etc/magic";
            }
            test_path = path_str.c_str();
            break;
    }
    
    // Call magic_getpath - result can be NULL or a valid path
    const char* result = magic_getpath(test_path, action);
    (void)result; // Use result to avoid unused variable warning
}

// Test complex API sequences
void test_complex_sequences(magic_t magic, FuzzedDataProvider& fdp, 
                           const std::vector<uint8_t>& buffer_data,
                           const char* temp_file_path) {
    
    // Determine sequence length
    int sequence_length = fdp.ConsumeIntegralInRange<int>(1, MAX_API_SEQUENCE_LENGTH);
    
    for (int i = 0; i < sequence_length && fdp.remaining_bytes() >= 4; i++) {
        int operation = fdp.ConsumeIntegralInRange<int>(0, 15);
        
        switch (operation) {
            case 0: // magic_buffer
                if (!buffer_data.empty() && buffer_data.data() != NULL) {
                    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(
                        0, buffer_data.size());
                    const char* result = magic_buffer(magic, 
                        buffer_data.data(), buffer_size);
                    (void)result;
                }
                break;
                
            case 1: // magic_file
                if (temp_file_path != NULL) {
                    const char* result = magic_file(magic, temp_file_path);
                    (void)result;
                }
                break;
                
            case 2: // magic_descriptor
                if (temp_file_path != NULL) {
                    int fd = open(temp_file_path, O_RDONLY);
                    if (fd >= 0) {
                        const char* result = magic_descriptor(magic, fd);
                        (void)result;
                        close(fd);
                    }
                }
                break;
                
            case 3: // magic_setflags / magic_getflags
                {
                    int current_flags = magic_getflags(magic);
                    int new_flags = fdp.ConsumeIntegral<int>();
                    // Mask to valid flags
                    new_flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | 
                                 MAGIC_DEVICES | MAGIC_MIME_TYPE | MAGIC_CONTINUE |
                                 MAGIC_CHECK | MAGIC_PRESERVE_ATIME | MAGIC_RAW |
                                 MAGIC_ERROR | MAGIC_MIME_ENCODING | MAGIC_APPLE |
                                 MAGIC_EXTENSION);
                    magic_setflags(magic, new_flags);
                    
                    // Do a detection with new flags
                    if (!buffer_data.empty()) {
                        const char* result = magic_buffer(magic, 
                            buffer_data.data(), 
                            fdp.ConsumeIntegralInRange<size_t>(1, buffer_data.size()));
                        (void)result;
                    }
                    
                    // Restore flags
                    magic_setflags(magic, current_flags);
                }
                break;
                
            case 4: // magic_setparam / magic_getparam
                {
                    int param_type = fdp.ConsumeIntegralInRange<int>(0, 9);
                    // Use reasonable bounds for size_t parameters to prevent excessive allocations
                    // Different parameters have different typical bounds:
                    // MAGIC_PARAM_BYTES_MAX, MAGIC_PARAM_INDIR_MAX, etc. should be bounded
                    size_t param_value;
                    if (param_type == MAGIC_PARAM_BYTES_MAX) {
                        // Bytes max should be reasonably bounded to avoid huge allocations
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 1024 * 1024); // 1MB max
                    } else if (param_type == MAGIC_PARAM_INDIR_MAX) {
                        // Indirect recursion limit
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                    } else if (param_type == MAGIC_PARAM_NAME_MAX) {
                        // Name length limit
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
                    } else if (param_type == MAGIC_PARAM_ELF_PHNUM_MAX || 
                               param_type == MAGIC_PARAM_ELF_SHNUM_MAX ||
                               param_type == MAGIC_PARAM_ELF_NOTES_MAX ||
                               param_type == MAGIC_PARAM_ELF_SHSIZE_MAX) {
                        // ELF-related limits
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 65536);
                    } else if (param_type == MAGIC_PARAM_REGEX_MAX) {
                        // Regex pattern limit
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 8192);
                    } else if (param_type == MAGIC_PARAM_ENCODING_MAX) {
                        // Encoding detection limit
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 16384);
                    } else if (param_type == MAGIC_PARAM_MAGWARN_MAX) {
                        // Magic warning limit
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 1000);
                    } else {
                        // Default reasonable bound for other parameters
                        param_value = fdp.ConsumeIntegralInRange<size_t>(0, 65536);
                    }
                    
                    // Test setting parameter - param_value must be size_t as parameters are size limits
                    magic_setparam(magic, param_type, &param_value);
                    
                    // Test getting parameter - retrieved_value must be size_t
                    size_t retrieved_value = 0;
                    magic_getparam(magic, param_type, &retrieved_value);
                    (void)retrieved_value;
                }
                break;
                
            case 5: // magic_load with different paths
                {
                    int load_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
                    const char* load_path = NULL;
                    
                    if (load_choice == 0) {
                        load_path = NULL; // Default database
                    } else if (load_choice == 1) {
                        load_path = "/usr/share/misc/magic.mgc";
                    } else {
                        load_path = "/etc/magic";
                    }
                    
                    magic_load(magic, load_path);
                }
                break;
                
            case 6: // magic_compile
                if (temp_file_path != NULL) {
                    magic_compile(magic, temp_file_path);
                }
                break;
                
            case 7: // magic_check
                if (temp_file_path != NULL) {
                    magic_check(magic, temp_file_path);
                }
                break;
                
            case 8: // magic_list
                if (temp_file_path != NULL) {
                    magic_list(magic, temp_file_path);
                }
                break;
                
            case 9: // magic_load_buffers
                {
                    size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                    if (num_buffers > 0 && !buffer_data.empty()) {
                        void* buffers[3];
                        size_t sizes[3];
                        
                        for (size_t j = 0; j < num_buffers; j++) {
                            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(
                                1, std::min(buffer_data.size(), (size_t)1024));
                            // Use buffer data for magic database content
                            buffers[j] = const_cast<void*>(static_cast<const void*>(
                                buffer_data.data()));
                            sizes[j] = buffer_size;
                        }
                        
                        magic_load_buffers(magic, buffers, sizes, num_buffers);
                    }
                }
                break;
                
            case 10: // magic_error / magic_errno
                {
                    const char* error_msg = magic_error(magic);
                    (void)error_msg;
                    
                    int err = magic_errno(magic);
                    (void)err;
                }
                break;
                
            case 11: // magic_version
                {
                    int version = magic_version();
                    (void)version;
                }
                break;
                
            case 12: // Test with MIME flags
                {
                    int current_flags = magic_getflags(magic);
                    int mime_flags = current_flags | MAGIC_MIME;
                    magic_setflags(magic, mime_flags);
                    
                    if (!buffer_data.empty()) {
                        const char* result = magic_buffer(magic, 
                            buffer_data.data(), 
                            fdp.ConsumeIntegralInRange<size_t>(1, buffer_data.size()));
                        (void)result;
                    }
                    
                    magic_setflags(magic, current_flags);
                }
                break;
                
            case 13: // Test with debugging flags
                {
                    int current_flags = magic_getflags(magic);
                    magic_setflags(magic, current_flags | MAGIC_DEBUG);
                    
                    if (!buffer_data.empty()) {
                        const char* result = magic_buffer(magic, 
                            buffer_data.data(), 
                            fdp.ConsumeIntegralInRange<size_t>(1, buffer_data.size()));
                        (void)result;
                    }
                    
                    magic_setflags(magic, current_flags);
                }
                break;
                
            case 14: // Test with symlink following
                {
                    int current_flags = magic_getflags(magic);
                    magic_setflags(magic, current_flags | MAGIC_SYMLINK);
                    
                    if (temp_file_path != NULL) {
                        const char* result = magic_file(magic, temp_file_path);
                        (void)result;
                    }
                    
                    magic_setflags(magic, current_flags);
                }
                break;
                
            case 15: // Test compression detection
                {
                    int current_flags = magic_getflags(magic);
                    magic_setflags(magic, current_flags | MAGIC_COMPRESS);
                    
                    if (!buffer_data.empty()) {
                        const char* result = magic_buffer(magic, 
                            buffer_data.data(), 
                            fdp.ConsumeIntegralInRange<size_t>(1, buffer_data.size()));
                        (void)result;
                    }
                    
                    magic_setflags(magic, current_flags);
                }
                break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Test magic_getpath() - the primary uncovered API
    test_magic_getpath(fdp);
    
    // Phase 2: Test with multiple magic instances
    int num_instances = fdp.ConsumeIntegralInRange<int>(1, MAX_MAGIC_INSTANCES);
    std::vector<magic_t> magic_instances;
    
    for (int i = 0; i < num_instances; i++) {
        // Consume flags for this instance
        int flags = fdp.ConsumeIntegral<int>();
        flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
                  MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
                  MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
                  MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
        
        magic_t magic = magic_open(flags);
        if (magic == NULL) {
            continue;
        }
        
        magic_instances.push_back(magic);
        
        // Load magic database (ignore failure)
        magic_load(magic, NULL);
    }
    
    // If no instances were created, return early
    if (magic_instances.empty()) {
        return 0;
    }
    
    // Create test buffer and temporary file for all instances
    // Use at most half of remaining bytes for buffer data to leave some for API sequences
    size_t max_buffer_size = std::min(MAX_BUFFER_SIZE, fdp.remaining_bytes() / 2);
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, max_buffer_size);
    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    TemporaryFile temp_file(buffer_data.data(), buffer_data.size());
    
    // Get remaining bytes for API sequence testing
    std::vector<uint8_t> sequence_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Phase 3: Test complex API sequences on each instance
    // Split sequence data among instances
    for (size_t i = 0; i < magic_instances.size(); i++) {
        // Calculate slice of sequence data for this instance
        size_t start = (i * sequence_data.size()) / magic_instances.size();
        size_t end = ((i + 1) * sequence_data.size()) / magic_instances.size();
        if (start >= end) {
            continue;
        }
        
        std::vector<uint8_t> instance_data(sequence_data.begin() + start, 
                                          sequence_data.begin() + end);
        FuzzedDataProvider instance_fdp(instance_data.data(), instance_data.size());
        
        test_complex_sequences(magic_instances[i], instance_fdp, buffer_data, temp_file.getPath());
    }
    
    // Phase 4: Test error conditions and edge cases
    // Use any leftover input or just test with default values
    for (magic_t magic : magic_instances) {
        // Test with NULL magic cookie (should handle gracefully) - but only if buffer is valid
        if (buffer_data.empty() || buffer_data.data() != NULL) {
            const char* result = magic_buffer(NULL, buffer_data.data(), 
                buffer_data.empty() ? 0 : buffer_data.size());
            (void)result;
        }
        
        // Test with invalid buffer combinations - only valid combinations
        // NULL buffer with size 0 is allowed
        const char* result = magic_buffer(magic, NULL, 0);
        (void)result;
        
        // Don't test NULL buffer with non-zero size (API contract violation)
        // Instead test valid buffer with size 0
        if (!buffer_data.empty() && buffer_data.data() != NULL) {
            result = magic_buffer(magic, buffer_data.data(), 0);
            (void)result;
        }
        
        // Test magic_file with invalid paths
        result = magic_file(magic, NULL);
        (void)result;
        
        result = magic_file(magic, "/nonexistent/path");
        (void)result;
        
        // Test magic_descriptor with invalid descriptors
        result = magic_descriptor(magic, -1);
        (void)result;
        
        // Test magic_setparam with invalid parameters (use bounded arbitrary values)
        int invalid_param = 999;
        size_t value = 123; // Small bounded value for invalid parameter test
        magic_setparam(magic, invalid_param, &value);
        
        // Test magic_getparam with invalid parameters
        size_t retrieved_value = 0;
        magic_getparam(magic, invalid_param, &retrieved_value);
    }
    
    // Phase 5: Clean up all instances
    for (magic_t magic : magic_instances) {
        magic_close(magic);
    }
    
    return 0;
}

} // namespace harness_015
