#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_001 {
/*
 * Fuzzing harness for libmagic parameter configuration functions
 * Targets: magic_setparam, magic_getparam, magic_errno, magic_list, magic_load_buffers
 * Focus: Parameter manipulation, configuration lifecycle, error handling
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need enough for: operation selection + flags + param values
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 7); // 0-7 different operation sequences
    bool test_list = fdp.ConsumeBool();
    bool test_buffers = fdp.ConsumeBool();
    bool test_multi_params = fdp.ConsumeBool();
    
    // Build flags (limit to reasonable range)
    int actual_flags = flags & 0xFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Operation sequences
    switch (operation) {
        case 0: {
            // Basic parameter setting and getting
            for (int param = 0; param < 10; param++) { // All 10 MAGIC_PARAM values
                if (fdp.remaining_bytes() < sizeof(size_t)) break;
                
                size_t param_value = fdp.ConsumeIntegral<size_t>();
                
                // Set parameter
                int set_result = magic_setparam(magic, param, &param_value);
                
                // Get parameter back
                size_t get_value = 0;
                int get_result = magic_getparam(magic, param, &get_value);
                
                // Check error status
                int err = magic_errno(magic);
                (void)err; // Use err to avoid unused variable warning
                
                // Only compare if both operations succeeded
                if (set_result == 0 && get_result == 0) {
                    // For uint16_t parameters, we need to mask
                    if (param == MAGIC_PARAM_INDIR_MAX || 
                        param == MAGIC_PARAM_NAME_MAX || 
                        param == MAGIC_PARAM_ELF_PHNUM_MAX || 
                        param == MAGIC_PARAM_ELF_SHNUM_MAX || 
                        param == MAGIC_PARAM_ELF_NOTES_MAX || 
                        param == MAGIC_PARAM_REGEX_MAX) {
                        // These are stored as uint16_t
                        get_value &= 0xFFFF;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test magic_setflags before parameters
            int new_flags = fdp.ConsumeIntegral<int>();
            magic_setflags(magic, new_flags & 0xFFFFF);
            
            // Set a random parameter
            if (fdp.remaining_bytes() >= sizeof(size_t)) {
                int param = fdp.ConsumeIntegralInRange<int>(0, 9);
                size_t value = fdp.ConsumeIntegral<size_t>();
                magic_setparam(magic, param, &value);
            }
            break;
        }
        
        case 2: {
            // Test error handling with invalid parameters
            // Try setting with invalid param values
            for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(size_t); i++) {
                int invalid_param = fdp.ConsumeIntegralInRange<int>(10, 100); // Invalid param range
                size_t value = fdp.ConsumeIntegral<size_t>();
                int result = magic_setparam(magic, invalid_param, &value);
                (void)result; // Result expected to be -1 for invalid param
                
                // Check error
                int err = magic_errno(magic);
                (void)err;
            }
            break;
        }
        
        case 3: {
            // Test magic_list if requested
            if (test_list && fdp.remaining_bytes() > 0) {
                std::string magicfile = fdp.ConsumeRandomLengthString(256);
                // Try with various magic file paths
                magic_list(magic, magicfile.c_str());
                magic_list(magic, "/etc/magic");
                magic_list(magic, "magic");
                magic_list(magic, NULL);
            }
            break;
        }
        
        case 4: {
            // Test magic_load_buffers if requested and we have compiled magic files
            if (test_buffers) {
                // Create dummy buffers for testing
                const size_t num_buffers = 3;
                void* buffers[num_buffers] = {nullptr};
                size_t sizes[num_buffers] = {0};
                
                // Create some dummy buffer data from fuzzer input
                for (size_t i = 0; i < num_buffers && fdp.remaining_bytes() > 0; i++) {
                    size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (fdp.remaining_bytes() >= buf_size) {
                        std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buf_size);
                        // In real scenario, these would be compiled .mgc files
                        // For fuzzing, we just pass the raw data
                        buffers[i] = malloc(buf_size);
                        if (buffers[i]) {
                            memcpy(buffers[i], buffer.data(), buf_size);
                            sizes[i] = buf_size;
                        }
                    }
                }
                
                // Try to load buffers
                magic_load_buffers(magic, buffers, sizes, num_buffers);
                
                // Clean up
                for (size_t i = 0; i < num_buffers; i++) {
                    if (buffers[i]) free(buffers[i]);
                }
            }
            break;
        }
        
        case 5: {
            // Test multiple parameters in sequence
            if (test_multi_params) {
                std::vector<std::pair<int, size_t>> params_to_test;
                
                // Consume up to 5 parameter-value pairs
                for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(size_t) + 1; i++) {
                    int param = fdp.ConsumeIntegralInRange<int>(0, 9);
                    size_t value = fdp.ConsumeIntegral<size_t>();
                    params_to_test.emplace_back(param, value);
                }
                
                // Set all parameters
                for (const auto& p : params_to_test) {
                    magic_setparam(magic, p.first, &p.second);
                }
                
                // Get all parameters back
                for (const auto& p : params_to_test) {
                    size_t get_value = 0;
                    magic_getparam(magic, p.first, &get_value);
                }
            }
            break;
        }
        
        case 6: {
            // Test parameter boundary values
            // Test with minimum and maximum size_t values
            size_t min_val = 0;
            size_t max_val = SIZE_MAX;
            
            // Test a few parameters with boundary values
            int test_params[] = {MAGIC_PARAM_BYTES_MAX, MAGIC_PARAM_ENCODING_MAX, 
                                 MAGIC_PARAM_ELF_SHSIZE_MAX, MAGIC_PARAM_MAGWARN_MAX};
            
            for (int param : test_params) {
                magic_setparam(magic, param, &min_val);
                magic_setparam(magic, param, &max_val);
                
                size_t get_val = 0;
                magic_getparam(magic, param, &get_val);
            }
            break;
        }
        
        case 7: {
            // Comprehensive test: flags, multiple params, error checking
            // Set random flags
            int random_flags = fdp.ConsumeIntegral<int>();
            magic_setflags(magic, random_flags & 0xFFFFF);
            
            // Test a few random parameters
            for (int i = 0; i < 3 && fdp.remaining_bytes() >= sizeof(size_t); i++) {
                int param = fdp.ConsumeIntegralInRange<int>(0, 9);
                size_t value = fdp.ConsumeIntegral<size_t>();
                magic_setparam(magic, param, &value);
                
                size_t get_value = 0;
                magic_getparam(magic, param, &get_value);
            }
            
            // Check error status
            int err = magic_errno(magic);
            (void)err;
            
            // Try magic_list with random path
            if (fdp.remaining_bytes() > 0) {
                std::string random_path = fdp.ConsumeRandomLengthString(128);
                magic_list(magic, random_path.c_str());
            }
            break;
        }
    }

    // Always check error status before closing
    int final_err = magic_errno(magic);
    (void)final_err;
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_001
