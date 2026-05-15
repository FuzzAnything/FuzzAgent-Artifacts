#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include "cjson/cJSON.h"

namespace harness_009 {
/*
 * Harness for cJSON_PrintPreallocated API - targets JSON printing with preallocated buffers
 * 
 * This harness specifically targets cJSON_PrintPreallocated which has 4 undiscovered branches
 * and 0% function lines hit according to coverage guidance. It focuses on testing the
 * preallocated buffer printing functionality with various edge cases.
 * 
 * Implementation follows the coverage guidance invocation sequence:
 * 1. Create root JSON object/array
 * 2. Build complex nested structure with various value types
 * 3. Preallocate buffer of sufficient size
 * 4. Call cJSON_PrintPreallocated
 * 5. Verify buffer contains valid JSON
 * 6. Clean up with cJSON_Delete
 * 7. Test edge cases (insufficient buffer size, null buffer, zero length)
 * 
 * This harness is semantically distinct from existing harnesses:
 * - harness_000 tests basic Print/PrintUnformatted/PrintBuffered
 * - This harness specifically targets PrintPreallocated with buffer management
 * - Focuses on buffer size edge cases and preallocated memory model
 */



// Helper function to create a complex nested JSON structure
static cJSON* create_complex_structure(FuzzedDataProvider& fdp, int depth) {
    if (depth <= 0) {
        // Leaf nodes - create primitive values
        uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 6;
        switch (type) {
            case 0:
                return cJSON_CreateNull();
            case 1:
                return cJSON_CreateBool(fdp.ConsumeBool());
            case 2:
                return cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
            case 3:
                return cJSON_CreateString(fdp.ConsumeRandomLengthString(128).c_str());
            case 4:
                return cJSON_CreateRaw(fdp.ConsumeRandomLengthString(64).c_str());
            case 5:
                return cJSON_CreateArray(); // Empty array as leaf
            default:
                return cJSON_CreateNull();
        }
    }
    
    // Decide whether to create object or array
    bool create_object = fdp.ConsumeBool();
    
    if (create_object) {
        cJSON* obj = cJSON_CreateObject();
        if (obj == NULL) return NULL;
        
        // Add 2-5 properties to the object
        int num_props = fdp.ConsumeIntegralInRange<int>(2, 5);
        for (int i = 0; i < num_props; i++) {
            std::string key = fdp.ConsumeRandomLengthString(32);
            cJSON* value = create_complex_structure(fdp, depth - 1);
            if (value != NULL) {
                cJSON_AddItemToObject(obj, key.c_str(), value);
            }
        }
        return obj;
    } else {
        cJSON* arr = cJSON_CreateArray();
        if (arr == NULL) return NULL;
        
        // Add 2-5 elements to the array
        int num_elements = fdp.ConsumeIntegralInRange<int>(2, 5);
        for (int i = 0; i < num_elements; i++) {
            cJSON* element = create_complex_structure(fdp, depth - 1);
            if (element != NULL) {
                cJSON_AddItemToArray(arr, element);
            }
        }
        return arr;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // Need enough data for structure creation and buffer size selection
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Part 1: Create complex nested JSON structure
    // --------------------------------------------
    
    // Determine nesting depth (1-3 levels for reasonable complexity)
    int depth = fdp.ConsumeIntegralInRange<int>(1, 3);
    
    cJSON* root = create_complex_structure(fdp, depth);
    if (root == NULL) {
        return 0;
    }
    
    // Part 2: Test cJSON_PrintPreallocated with various buffer sizes
    // --------------------------------------------------------------
    
    // First, get an estimate of how much buffer space we need
    // Use regular print to determine required size (for sufficient buffer test)
    char* regular_print = cJSON_PrintUnformatted(root);
    size_t needed_size = 0;
    if (regular_print != NULL) {
        needed_size = strlen(regular_print) + 1; // +1 for null terminator
        free(regular_print);
    }
    
    // Consume buffer size from fuzzer input (0 to 2*needed_size)
    // This allows testing both sufficient and insufficient buffers
    size_t buffer_size = 0;
    if (needed_size > 0) {
        buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, needed_size * 2);
    } else {
        buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    }
    
    // Consume format flag (0 = unformatted, 1 = formatted)
    cJSON_bool format = fdp.ConsumeBool();
    
    // Test case 1: Normal operation with allocated buffer
    if (buffer_size > 0) {
        char* buffer = (char*)malloc(buffer_size);
        if (buffer != NULL) {
            // Initialize buffer with pattern to detect overwrites
            memset(buffer, 0xAA, buffer_size);
            
            cJSON_bool result = cJSON_PrintPreallocated(root, buffer, buffer_size, format);
            
            // If printing succeeded, verify the buffer contains valid JSON
            if (result && buffer_size > 0) {
                // Check that buffer is null-terminated (within bounds)
                size_t actual_len = strnlen(buffer, buffer_size);
                if (actual_len < buffer_size) {
                    // Try to parse the printed JSON back
                    cJSON* parsed = cJSON_Parse(buffer);
                    if (parsed != NULL) {
                        cJSON_Delete(parsed);
                    }
                }
            }
            
            free(buffer);
        }
    }
    
    // Test case 2: Edge case - NULL buffer (should return false)
    if (fdp.ConsumeBool()) {
        cJSON_bool result = cJSON_PrintPreallocated(root, NULL, 100, format);
        // Should return false for NULL buffer
    }
    
    // Test case 3: Edge case - zero or negative length
    if (fdp.ConsumeBool()) {
        int invalid_length = fdp.ConsumeIntegralInRange<int>(-100, 0);
        char small_buffer[1];
        cJSON_bool result = cJSON_PrintPreallocated(root, small_buffer, invalid_length, format);
        // Should return false for invalid length
    }
    
    // Test case 4: Very small buffer (insufficient size)
    if (fdp.ConsumeBool() && buffer_size > 1) {
        size_t small_size = fdp.ConsumeIntegralInRange<size_t>(1, buffer_size - 1);
        if (small_size > 0) {
            char* small_buffer = (char*)malloc(small_size);
            if (small_buffer != NULL) {
                memset(small_buffer, 0xBB, small_size);
                cJSON_bool result = cJSON_PrintPreallocated(root, small_buffer, small_size, format);
                // May return false if buffer too small
                free(small_buffer);
            }
        }
    }
    
    // Test case 5: Test with both formatted and unformatted output
    // (Already done via format flag variation, but test explicitly)
    if (fdp.ConsumeBool() && buffer_size > 0) {
        char* buffer = (char*)malloc(buffer_size);
        if (buffer != NULL) {
            cJSON_bool format2 = !format; // Test opposite format
            cJSON_bool result = cJSON_PrintPreallocated(root, buffer, buffer_size, format2);
            free(buffer);
        }
    }
    
    // Part 3: Cleanup
    // --------------
    cJSON_Delete(root);
    
    return 0;
}

} // namespace harness_009
