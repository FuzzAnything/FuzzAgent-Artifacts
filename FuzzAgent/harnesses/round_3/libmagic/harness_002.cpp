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

namespace harness_002 {
/*
 * Fuzzing harness for libmagic magic file compilation lifecycle operations
 * This harness targets completely uncovered APIs for magic file compilation:
 * - magic_compile (7 undiscovered branches)
 * - magic_check (7 undiscovered branches)
 * - magic_list (7 undiscovered branches)
 * - magic_descriptor (3 undiscovered branches)
 * - magic_load_buffers (2 undiscovered branches)
 * Total: 26+ undiscovered branches, 0% coverage
 * 
 * Implementation follows the magic file compilation lifecycle:
 * 1. magic_open -> 2. magic_compile -> 3. magic_check -> 4. magic_list ->
 * 5. magic_descriptor -> 6. magic_load_buffers -> 7. magic_buffer/file -> 8. magic_close
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful compilation lifecycle testing
    // We need: flags (4 bytes) + magic file content (min 100 bytes) + test data
    if (size < 104) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PHASE 1: Setup - Create magic handle with fuzzed flags
    // ============================================================
    
    int flags = 0;
    
    // Consume flags from fuzzed input - focus on compilation-relevant flags
    if (fdp.ConsumeBool()) flags |= MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
    // Add compilation-specific flags
    if (fdp.ConsumeBool()) flags |= MAGIC_CHECK;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // If magic_open fails with complex flags, try with simple flags
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // ============================================================
    // PHASE 2: Create temporary magic file for compilation
    // ============================================================
    
    // Consume magic file content from fuzzed input
    // Use a reasonable maximum size to avoid excessive memory usage
    size_t magic_content_size = fdp.ConsumeIntegralInRange<size_t>(100, 4096);
    if (fdp.remaining_bytes() < magic_content_size) {
        magic_content_size = fdp.remaining_bytes();
    }
    
    std::string magic_content = fdp.ConsumeBytesAsString(magic_content_size);
    
    // Write magic database to a temporary file
    char magic_template[] = "/tmp/magic_compile_fuzz_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Write the magic content
    ssize_t written = write(magic_fd, magic_content.c_str(), magic_content.size());
    if (written < 0) {
        close(magic_fd);
        unlink(magic_template);
        magic_close(magic);
        return 0;
    }
    fsync(magic_fd);
    close(magic_fd);
    
    // ============================================================
    // PHASE 3: Magic Compilation Lifecycle Operations
    // ============================================================
    
    // 1. magic_compile - Compile the magic file
    int compile_result = magic_compile(magic, magic_template);
    // Result can be -1 (error) or 0 (success) - both are valid test outcomes
    
    // 2. magic_check - Verify the compiled magic file
    int check_result = magic_check(magic, magic_template);
    // Result can be -1 (error) or 0 (success) - both are valid test outcomes
    
    // 3. magic_list - List magic entries from the file
    int list_result = magic_list(magic, magic_template);
    // Result can be -1 (error) or 0 (success) - both are valid test outcomes
    
    // Clean up temporary magic file before proceeding
    unlink(magic_template);
    
    // ============================================================
    // PHASE 4: Test magic_descriptor with a file descriptor
    // ============================================================
    
    // Create a test file with fuzzed content for descriptor testing
    char test_template[] = "/tmp/magic_desc_fuzz_XXXXXX";
    int test_fd = mkstemp(test_template);
    if (test_fd != -1) {
        // Consume test data from fuzzed input
        size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (fdp.remaining_bytes() < test_data_size) {
            test_data_size = fdp.remaining_bytes();
        }
        
        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
        if (test_data.size() > 0) {
            write(test_fd, test_data.data(), test_data.size());
            fsync(test_fd);
            
            // Test magic_descriptor with the file descriptor
            const char* descriptor_result = magic_descriptor(magic, test_fd);
            // Result can be NULL or a string - both are valid
            
            // Reset file pointer to beginning for possible reuse
            lseek(test_fd, 0, SEEK_SET);
        }
        
        // Keep file descriptor open for potential reuse
        // We'll close it after magic_load_buffers test
    }
    
    // ============================================================
    // PHASE 5: Test magic_load_buffers with compiled buffers
    // ============================================================
    
    // Prepare magic buffers for loading
    // Consume buffer configuration from fuzzed input
    size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(0, 4);
    if (num_buffers > 0 && fdp.remaining_bytes() >= num_buffers * sizeof(size_t)) {
        std::vector<void*> buffers;
        std::vector<size_t> buffer_sizes;
        
        // Allocate and fill buffers with fuzzed data
        for (size_t i = 0; i < num_buffers; i++) {
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() < buffer_size) {
                buffer_size = fdp.remaining_bytes();
            }
            
            if (buffer_size > 0) {
                void* buffer = malloc(buffer_size);
                if (buffer != nullptr) {
                    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
                    memcpy(buffer, buffer_data.data(), buffer_size);
                    buffers.push_back(buffer);
                    buffer_sizes.push_back(buffer_size);
                }
            }
        }
        
        if (!buffers.empty()) {
            // Test magic_load_buffers with the prepared buffers
            int load_buffers_result = magic_load_buffers(magic, buffers.data(), buffer_sizes.data(), buffers.size());
            // Result can be -1 (error) or 0 (success) - both are valid
            
            // Clean up allocated buffers
            for (void* buffer : buffers) {
                free(buffer);
            }
        }
    }
    
    // ============================================================
    // PHASE 6: Test the compiled magic database
    // ============================================================
    
    // Test with remaining fuzzed data using magic_buffer
    if (fdp.remaining_bytes() > 0) {
        size_t test_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(test_buffer_size);
        
        const char* buffer_result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        // Result can be NULL or a string - both are valid
    }
    
    // Also test with the test file if we created one
    if (test_fd != -1) {
        // Test magic_file if we have a valid file path
        const char* file_result = magic_file(magic, test_template);
        // Result can be NULL or a string - both are valid
        
        close(test_fd);
        unlink(test_template);
    }
    
    // ============================================================
    // PHASE 7: Cleanup
    // ============================================================
    
    magic_close(magic);
    
    // Use the results to avoid unused variable warnings
    (void)compile_result;
    (void)check_result;
    (void)list_result;
    
    return 0;
}

} // namespace harness_002
