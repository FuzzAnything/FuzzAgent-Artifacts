#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing of array operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // First, decide what type of array to create and test
    uint8_t array_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t test_operation = fdp.ConsumeIntegral<uint8_t>() % 4;

    // Determine array size (limited to prevent excessive memory usage)
    int array_size = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    cJSON* array = NULL;
    bool success = false;

    switch (array_type) {
        case 0: {
            // Test cJSON_CreateIntArray
            if (fdp.remaining_bytes() < static_cast<size_t>(array_size * sizeof(int))) {
                return 0;  // Not enough input for int array
            }
            std::vector<int> int_numbers;
            int_numbers.reserve(array_size);
            for (int i = 0; i < array_size; i++) {
                int_numbers.push_back(fdp.ConsumeIntegral<int>());
            }
            array = cJSON_CreateIntArray(int_numbers.data(), array_size);
            break;
        }
        case 1: {
            // Test cJSON_CreateFloatArray
            if (fdp.remaining_bytes() < static_cast<size_t>(array_size * sizeof(float))) {
                return 0;  // Not enough input for float array
            }
            std::vector<float> float_numbers;
            float_numbers.reserve(array_size);
            for (int i = 0; i < array_size; i++) {
                float_numbers.push_back(fdp.ConsumeFloatingPoint<float>());
            }
            array = cJSON_CreateFloatArray(float_numbers.data(), array_size);
            break;
        }
        case 2: {
            // Test cJSON_CreateDoubleArray
            if (fdp.remaining_bytes() < static_cast<size_t>(array_size * sizeof(double))) {
                return 0;  // Not enough input for double array
            }
            std::vector<double> double_numbers;
            double_numbers.reserve(array_size);
            for (int i = 0; i < array_size; i++) {
                double_numbers.push_back(fdp.ConsumeFloatingPoint<double>());
            }
            array = cJSON_CreateDoubleArray(double_numbers.data(), array_size);
            break;
        }
        case 3: {
            // Test cJSON_CreateStringArray
            // Need at least array_size bytes for string array (1 byte per string minimum)
            if (fdp.remaining_bytes() < static_cast<size_t>(array_size)) {
                return 0;  // Not enough input for string array
            }
            std::vector<const char*> string_ptrs;
            std::vector<std::string> strings;
            strings.reserve(array_size);
            string_ptrs.reserve(array_size);
            
            for (int i = 0; i < array_size; i++) {
                // Generate random string of reasonable length
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() < str_len) {
                    return 0;  // Not enough input for this string
                }
                std::string str = fdp.ConsumeBytesAsString(str_len);
                strings.push_back(str);
                string_ptrs.push_back(strings.back().c_str());
            }
            array = cJSON_CreateStringArray(string_ptrs.data(), array_size);
            break;
        }
    }
    if (array == NULL) {
        // Array creation failed
        return 0;
    }

    // Verify array was created successfully
    if (!cJSON_IsArray(array)) {
        cJSON_Delete(array);
        return 0;
    }

    // Test cJSON_GetArraySize
    int actual_size = cJSON_GetArraySize(array);
    
    // Perform array manipulation operations based on test_operation
    switch (test_operation) {
        case 0: {
            // Test cJSON_InsertItemInArray
            if (actual_size > 0 && fdp.remaining_bytes() > 0) {
                // Choose a position to insert (0 = beginning, actual_size = append)
                int insert_position = fdp.ConsumeIntegralInRange<int>(0, actual_size);
                
                // Create new item to insert
                cJSON* new_item = NULL;
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                
                switch (item_type) {
                    case 0:
                        if (fdp.remaining_bytes() >= sizeof(double)) {
                            new_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        }
                        break;
                    case 1:
                        if (fdp.remaining_bytes() > 0) {
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str = fdp.ConsumeBytesAsString(str_len);
                                new_item = cJSON_CreateString(str.c_str());
                            }
                        }
                        break;
                    case 2:
                        if (fdp.remaining_bytes() > 0) {
                            new_item = cJSON_CreateBool(fdp.ConsumeBool());
                        }
                        break;
                }
                
                if (new_item != NULL) {
                    success = cJSON_InsertItemInArray(array, insert_position, new_item);
                    // Note: cJSON_InsertItemInArray takes ownership of new_item
                }
            }
            break;
        }
        case 1: {
            // Test cJSON_ReplaceItemInArray
            if (actual_size > 0 && fdp.remaining_bytes() > 0) {
                // Choose an item to replace
                int replace_position = fdp.ConsumeIntegralInRange<int>(0, actual_size - 1);
                
                // Create replacement item
                cJSON* replacement = NULL;
                uint8_t replacement_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                
                switch (replacement_type) {
                    case 0:
                        if (fdp.remaining_bytes() >= sizeof(double)) {
                            replacement = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        }
                        break;
                    case 1:
                        if (fdp.remaining_bytes() > 0) {
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str = fdp.ConsumeBytesAsString(str_len);
                                replacement = cJSON_CreateString(str.c_str());
                            }
                        }
                        break;
                    case 2:
                        if (fdp.remaining_bytes() > 0) {
                            replacement = cJSON_CreateBool(fdp.ConsumeBool());
                        }
                        break;
                }
                if (replacement != NULL) {
                    success = cJSON_ReplaceItemInArray(array, replace_position, replacement);
                    // Note: cJSON_ReplaceItemInArray takes ownership of replacement
                }
            }
            break;
        }
        case 2: {
            // Test cJSON_DeleteItemFromArray
            if (actual_size > 0 && fdp.remaining_bytes() > 0) {
                // Choose an item to delete
                int delete_position = fdp.ConsumeIntegralInRange<int>(0, actual_size - 1);
                cJSON_DeleteItemFromArray(array, delete_position);
                success = true;
            }
            break;
        }
        case 3: {
            // Test cJSON_GetArrayItem - access all items
            if (actual_size > 0) {
                // Access a random item
                if (actual_size > 0 && fdp.remaining_bytes() > 0) {
                    int access_index = fdp.ConsumeIntegralInRange<int>(0, actual_size - 1);
                    cJSON* accessed_item = cJSON_GetArrayItem(array, access_index);
                    success = (accessed_item != NULL);
                    
                    // If we accessed an item, test its type
                    if (accessed_item != NULL) {
                        // Test type checking on the accessed item
                        cJSON_bool is_number = cJSON_IsNumber(accessed_item);
                        cJSON_bool is_string = cJSON_IsString(accessed_item);
                        cJSON_bool is_bool = cJSON_IsBool(accessed_item);
                        cJSON_bool is_array = cJSON_IsArray(accessed_item);
                        
                        // Extract values if appropriate
                        if (is_number) {
                            double num_val = cJSON_GetNumberValue(accessed_item);
                        } else if (is_string) {
                            char* str_val = cJSON_GetStringValue(accessed_item);
                        }
                    }
                }
                
                // Also try to access out-of-bounds items (should return NULL)
                cJSON* out_of_bounds_item = cJSON_GetArrayItem(array, actual_size);
                // This should be NULL, which is expected
            }
            break;
        }
    }

    // Additional array operations that should always be tested
    
    // Test iterating through array items
    for (int i = 0; i < cJSON_GetArraySize(array); i++) {
        cJSON* item = cJSON_GetArrayItem(array, i);
        if (item != NULL) {
            // Minimal operation to ensure item is valid
            cJSON_bool is_valid = cJSON_IsInvalid(item);
        }
    }
    
    // Test adding items to the array with cJSON_AddItemToArray
    if (fdp.remaining_bytes() > 0) {
        cJSON* extra_item = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
        if (extra_item != NULL) {
            cJSON_AddItemToArray(array, extra_item);
        }
    }
    
    // Test detaching an item (if array has items)
    if (cJSON_GetArraySize(array) > 0 && fdp.remaining_bytes() > 0) {
        int detach_index = fdp.ConsumeIntegralInRange<int>(0, cJSON_GetArraySize(array) - 1);
        cJSON* detached = cJSON_DetachItemFromArray(array, detach_index);
        if (detached != NULL) {
            cJSON_Delete(detached);
        }
    }

    // Clean up
    cJSON_Delete(array);

    return 0;
}

} // namespace harness_001
