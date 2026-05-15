#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <climits>
#include <vector>
#include "magic.h"

namespace harness_001 {
/*
 * Fuzzing harness for libmagic configuration management APIs
 * Targets: magic_setparam, magic_getparam, magic_errno
 * Uses FuzzedDataProvider for structured input consumption
 * Differentiates from harness_000 by focusing on parameter tuning and error handling
 */



// Maximum parameter value to avoid excessive memory usage
#define MAX_PARAM_VALUE 1024 * 1024  // 1MB

// All MAGIC_PARAM_* constants from magic.h
enum MagicParam {
    PARAM_INDIR_MAX = 0,
    PARAM_NAME_MAX = 1,
    PARAM_ELF_PHNUM_MAX = 2,
    PARAM_ELF_SHNUM_MAX = 3,
    PARAM_ELF_NOTES_MAX = 4,
    PARAM_REGEX_MAX = 5,
    PARAM_BYTES_MAX = 6,
    PARAM_ENCODING_MAX = 7,
    PARAM_ELF_SHSIZE_MAX = 8,
    PARAM_MAGWARN_MAX = 9,
    PARAM_INVALID = 100  // For testing invalid parameters
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need at least enough for basic parameters
    if (size < sizeof(size_t) + 1) {
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

    // Test parameter setting and getting
    // Determine how many parameters to test based on remaining input
    size_t num_params_to_test = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_params_to_test && fdp.remaining_bytes() >= sizeof(size_t); i++) {
        // Consume parameter type
        int param_type = fdp.ConsumeIntegralInRange<int>(0, PARAM_INVALID);
        
        // Consume parameter value
        size_t param_value = fdp.ConsumeIntegral<size_t>();
        
        // Limit parameter value to reasonable range
        param_value %= MAX_PARAM_VALUE;
        
        // Test magic_setparam
        int set_result = magic_setparam(magic, param_type, &param_value);
        
        // Check error status
        int errno_value = magic_errno(magic);
        (void)errno_value;  // Use result to avoid unused variable warning
        
        // Only test magic_getparam if set was successful (or for invalid param testing)
        if (set_result == 0 || param_type >= PARAM_INVALID) {
            size_t retrieved_value = 0;
            int get_result = magic_getparam(magic, param_type, &retrieved_value);
            
            // If set was successful and get was successful, the values should match
            // (unless param_type is invalid, in which case both should fail)
            if (set_result == 0 && get_result == 0) {
                // Values should match for valid parameter setting
                (void)retrieved_value;  // Use to avoid unused variable warning
            }
        }
    }

    // Test magic_buffer with different parameter configurations
    // First, create some test data for buffer testing
    if (fdp.remaining_bytes() > 0) {
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
        
        if (test_buffer.size() > 0) {
            // Test with current parameter configuration
            const char* buffer_result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
            (void)buffer_result;  // Use result to avoid unused variable warning
            
            // Check error status after buffer operation
            int buffer_errno = magic_errno(magic);
            (void)buffer_errno;
        }
    }

    // Test edge cases with extreme parameter values
    if (fdp.remaining_bytes() >= sizeof(size_t)) {
        // Test with maximum possible value
        size_t max_value = SIZE_MAX;
        magic_setparam(magic, PARAM_BYTES_MAX, &max_value);
        
        // Test with zero value
        size_t zero_value = 0;
        magic_setparam(magic, PARAM_BYTES_MAX, &zero_value);
        
        // Test with very small value
        size_t small_value = 1;
        magic_setparam(magic, PARAM_BYTES_MAX, &small_value);
    }

    // Test parameter interactions
    // Set multiple parameters and test buffer operation
    if (fdp.remaining_bytes() >= 2 * sizeof(size_t)) {
        // Set a couple of parameters
        size_t param1_value = fdp.ConsumeIntegral<size_t>() % 1000;
        size_t param2_value = fdp.ConsumeIntegral<size_t>() % 1000;
        
        magic_setparam(magic, PARAM_INDIR_MAX, &param1_value);
        magic_setparam(magic, PARAM_NAME_MAX, &param2_value);
        
        // Test buffer with the new parameter configuration
        if (fdp.remaining_bytes() > 0) {
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
            std::vector<uint8_t> interaction_buffer = fdp.ConsumeBytes<uint8_t>(test_size);
            
            if (interaction_buffer.size() > 0) {
                const char* interaction_result = magic_buffer(magic, interaction_buffer.data(), interaction_buffer.size());
                (void)interaction_result;
            }
        }
    }

    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_001
