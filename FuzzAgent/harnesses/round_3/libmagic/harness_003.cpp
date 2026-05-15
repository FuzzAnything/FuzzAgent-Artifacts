#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_003 {
/*
 * Fuzzing harness for libmagic error handling and additional functionality
 * This harness targets error handling paths and APIs not covered in existing harnesses:
 * - magic_getpath (completely uncovered)
 * - Comprehensive magic_error/magic_errno testing
 * - Additional flags: MAGIC_APPLE, MAGIC_EXTENSION, MAGIC_COMPRESS_TRANSP, MAGIC_NO_COMPRESS_FORK
 * - MAGIC_NO_CHECK_* flags for disabling specific file type checks
 * - Edge cases: invalid file descriptors, null pointers, malformed inputs
 * 
 * Based on analysis of existing harnesses:
 * - harness_000: Basic file detection
 * - harness_001: Parameter configuration  
 * - harness_002: Magic file compilation lifecycle
 * 
 * This harness complements by focusing on error recovery and advanced flags
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful error handling testing
    // We need: flags (4 bytes) + operation selection (1 byte) + some test data
    if (size < 10) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PHASE 1: Test magic_getpath with fuzzed parameters
    // ============================================================
    
    // Consume search path from fuzzed input
    size_t path_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string search_path = fdp.ConsumeBytesAsString(path_len);
    
    // Consume action parameter (0 or 1 as per magic.h documentation)
    int action = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Test magic_getpath - this function should always return something
    // (either default magic file path or custom path)
    const char* magic_path = magic_getpath(search_path.empty() ? nullptr : search_path.c_str(), action);
    if (magic_path != nullptr) {
        // Path returned, can be used for reference
        (void)magic_path; // Use to avoid unused variable warning
    }
    
    // ============================================================
    // PHASE 2: Create magic handle with comprehensive flags
    // ============================================================
    
    int flags = 0;
    
    // Consume flags from fuzzed input - test ALL available flags
    // Basic flags (some tested in harness_000)
    if (fdp.ConsumeBool()) flags |= MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEBUG;
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CHECK;
    if (fdp.ConsumeBool()) flags |= MAGIC_PRESERVE_ATIME;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_ENCODING;
    
    // Additional flags not extensively tested in existing harnesses
    if (fdp.ConsumeBool()) flags |= MAGIC_APPLE;
    if (fdp.ConsumeBool()) flags |= MAGIC_EXTENSION;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS_TRANSP;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_COMPRESS_FORK;
    
    // MAGIC_NO_CHECK_* flags for disabling specific checks
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_TAR;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_SOFT;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_APPTYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_ELF;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_TEXT;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_CDF;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_CSV;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_TOKENS;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_ENCODING;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_JSON;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_SIMH;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // If magic_open fails with complex flags, test error handling without handle
        // Try to get error information (though magic_error needs a handle)
        // We'll create a simple handle for remaining tests
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            // Can't proceed without magic handle
            return 0;
        }
    }
    
    // ============================================================
    // PHASE 3: Test magic_version and basic flag operations
    // ============================================================
    
    int version = magic_version();
    (void)version; // Use to avoid unused variable warning
    
    // Test getting current flags
    int current_flags = magic_getflags(magic);
    (void)current_flags;
    
    // Test setting new flags from fuzzed input
    int new_flags = fdp.ConsumeIntegral<int>();
    magic_setflags(magic, new_flags);
    
    // ============================================================
    // PHASE 4: Error handling testing - magic_error and magic_errno
    // ============================================================
    
    // First, get any existing error (should be NULL if no error)
    const char* existing_error = magic_error(magic);
    if (existing_error != nullptr) {
        // Error exists, we can examine it
        (void)existing_error;
    }
    
    // Get current errno value
    int current_errno = magic_errno(magic);
    (void)current_errno;
    
    // ============================================================
    // PHASE 5: Test with invalid operations to trigger errors
    // ============================================================
    
    // Test 5.1: magic_buffer with NULL pointer or zero size
    if (fdp.ConsumeBool()) {
        // Test with NULL data pointer
        const char* null_result = magic_buffer(magic, nullptr, 0);
        (void)null_result;
        
        // Check error after NULL operation
        const char* null_error = magic_error(magic);
        int null_errno = magic_errno(magic);
        (void)null_error;
        (void)null_errno;
    }
    
    // Test 5.2: magic_file with invalid/non-existent file
    if (fdp.ConsumeBool()) {
        // Create a non-existent file path
        size_t invalid_path_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string invalid_path = fdp.ConsumeBytesAsString(invalid_path_len);
        
        const char* file_result = magic_file(magic, invalid_path.c_str());
        (void)file_result;
        
        // Check error after invalid file operation
        const char* file_error = magic_error(magic);
        int file_errno = magic_errno(magic);
        (void)file_error;
        (void)file_errno;
    }
    
    // Test 5.3: magic_descriptor with invalid file descriptor
    if (fdp.ConsumeBool()) {
        // Use invalid file descriptor (-1 or other negative value)
        int invalid_fd = fdp.ConsumeIntegralInRange<int>(-100, -1);
        const char* fd_result = magic_descriptor(magic, invalid_fd);
        (void)fd_result;
        
        // Check error after invalid descriptor operation
        const char* fd_error = magic_error(magic);
        int fd_errno = magic_errno(magic);
        (void)fd_error;
        (void)fd_errno;
    }
    
    // ============================================================
    // PHASE 6: Test with valid data to ensure normal operation
    // ============================================================
    
    if (fdp.remaining_bytes() > 0) {
        // Consume test data for valid operations
        size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
        
        // Test magic_buffer with valid data
        const char* buffer_result = magic_buffer(magic, test_data.data(), test_data.size());
        (void)buffer_result;
        
        // Test error state after successful operation
        const char* post_buffer_error = magic_error(magic);
        int post_buffer_errno = magic_errno(magic);
        (void)post_buffer_error;
        (void)post_buffer_errno;
        
        // If we have enough data left, test magic_file with temporary file
        if (fdp.remaining_bytes() > 100) {
            // Create temporary file
            char file_template[] = "/tmp/magic_error_fuzz_XXXXXX";
            int file_fd = mkstemp(file_template);
            if (file_fd != -1) {
                // Write some data to the file
                size_t file_data_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
                std::string file_data = fdp.ConsumeBytesAsString(file_data_size);
                write(file_fd, file_data.c_str(), file_data.size());
                fsync(file_fd);
                close(file_fd);
                
                // Test magic_file with the temporary file
                const char* temp_file_result = magic_file(magic, file_template);
                (void)temp_file_result;
                
                // Test magic_descriptor with the file descriptor
                int read_fd = open(file_template, O_RDONLY);
                if (read_fd != -1) {
                    const char* descriptor_result = magic_descriptor(magic, read_fd);
                    (void)descriptor_result;
                    close(read_fd);
                }
                
                // Clean up
                unlink(file_template);
            }
        }
    }
    
    // ============================================================
    // PHASE 7: Test flag toggling and error state persistence
    // ============================================================
    
    // Toggle some flags to see if error state persists
    if (fdp.ConsumeBool()) {
        int toggle_flags = fdp.ConsumeIntegral<int>();
        magic_setflags(magic, toggle_flags);
        
        // Check error after flag change
        const char* toggle_error = magic_error(magic);
        int toggle_errno = magic_errno(magic);
        (void)toggle_error;
        (void)toggle_errno;
    }
    
    // ============================================================
    // PHASE 8: Final error state check before cleanup
    // ============================================================
    
    // Get final error state
    const char* final_error = magic_error(magic);
    int final_errno = magic_errno(magic);
    (void)final_error;
    (void)final_errno;
    
    // ============================================================
    // PHASE 9: Cleanup
    // ============================================================
    
    magic_close(magic);
    
    return 0;
}

} // namespace harness_003
