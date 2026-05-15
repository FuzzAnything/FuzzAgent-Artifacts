#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_008 {
/*
 * Fuzzing harness for zlib utility/version/error functions
 * Targets: zlibVersion, zlibCompileFlags, zError
 * 
 * This harness specifically targets the utility functions in zutil.c
 * that have not been covered by previous fuzzing sessions. Differentiates
 * from harness_000-harness_007 by focusing exclusively on version,
 * compilation flags, and error code translation utilities.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least 1 byte to determine test scenario
    if (size < 1) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario parameter
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Always call zlibVersion() - it's a simple version function
    const char* version_str = zlibVersion();
    if (version_str == NULL) {
        // This should never happen, but check anyway
        return 0;
    }
    
    // Always call zlibCompileFlags() - it returns compile-time configuration
    unsigned long compile_flags = zlibCompileFlags();
    
    // Test zError() with various error codes based on fuzzer input
    // Error codes to test (from zlib.h):
    // Z_OK = 0, Z_STREAM_END = 1, Z_NEED_DICT = 2
    // Z_ERRNO = -1, Z_STREAM_ERROR = -2, Z_DATA_ERROR = -3
    // Z_MEM_ERROR = -4, Z_BUF_ERROR = -5, Z_VERSION_ERROR = -6
    
    const char* error_str = NULL;
    
    switch (test_scenario) {
        case 0: {
            // Test with valid positive error codes
            uint8_t error_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            int error_code = 0;
            
            switch (error_choice) {
                case 0:
                    error_code = Z_OK;  // 0
                    break;
                case 1:
                    error_code = Z_STREAM_END;  // 1
                    break;
                case 2:
                    error_code = Z_NEED_DICT;  // 2
                    break;
            }
            
            error_str = zError(error_code);
            break;
        }
        
        case 1: {
            // Test with valid negative error codes
            uint8_t error_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
            int error_code = 0;
            
            switch (error_choice) {
                case 0:
                    error_code = Z_ERRNO;  // -1
                    break;
                case 1:
                    error_code = Z_STREAM_ERROR;  // -2
                    break;
                case 2:
                    error_code = Z_DATA_ERROR;  // -3
                    break;
                case 3:
                    error_code = Z_MEM_ERROR;  // -4
                    break;
                case 4:
                    error_code = Z_BUF_ERROR;  // -5
                    break;
            }
            
            error_str = zError(error_code);
            break;
        }
        
        case 2: {
            // Test with Z_VERSION_ERROR and possibly out-of-range codes
            uint8_t test_variant = fdp.ConsumeIntegral<uint8_t>() % 2;
            
            if (test_variant == 0) {
                // Test Z_VERSION_ERROR (-6)
                error_str = zError(Z_VERSION_ERROR);
            } else {
                // Test with fuzzer-provided error code (may be out of range)
                int custom_error = fdp.ConsumeIntegral<int>();
                error_str = zError(custom_error);
                
                // Note: zError should return a string even for invalid codes
                // According to zutil.c, ERR_MSG macro handles out-of-range codes
            }
            break;
        }
    }
    
    // The functions have been called, which is the primary goal
    // We could optionally verify the returned strings are not NULL
    // (though zError should always return a non-NULL string)
    if (error_str == NULL) {
        // This might indicate an issue with error code handling
        // but we don't fail the test because the primary goal is coverage
    }
    
    // Additional test: call zError with all valid codes in sequence
    // This ensures we hit all branches in the error message array
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        // Array of all valid zlib error codes
        const int valid_error_codes[] = {
            Z_OK,
            Z_STREAM_END,
            Z_NEED_DICT,
            Z_ERRNO,
            Z_STREAM_ERROR,
            Z_DATA_ERROR,
            Z_MEM_ERROR,
            Z_BUF_ERROR,
            Z_VERSION_ERROR
        };
        
        const size_t num_codes = sizeof(valid_error_codes) / sizeof(valid_error_codes[0]);
        
        // Test a random subset or all codes based on remaining input
        size_t codes_to_test = fdp.ConsumeIntegralInRange<size_t>(1, num_codes);
        if (codes_to_test > num_codes) {
            codes_to_test = num_codes;
        }
        
        for (size_t i = 0; i < codes_to_test; i++) {
            // Use sequential or random selection
            size_t index = (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) 
                          ? fdp.ConsumeIntegralInRange<size_t>(0, num_codes - 1)
                          : i % num_codes;
            
            const char* test_error_str = zError(valid_error_codes[index]);
            (void)test_error_str; // Prevent unused variable warning
        }
    }
    
    return 0;
}

} // namespace harness_008
